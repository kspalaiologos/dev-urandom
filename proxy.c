#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/epoll.h>
#include <unistd.h>
int set_nonblocking(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1) return -1;
  flags |= O_NONBLOCK;
  if (fcntl(fd, F_SETFL, flags) == -1) return -1;
  return 0;
}
int create_server_socket(int port) {
  int sockfd;
  struct sockaddr_in server_addr;
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket"); exit(1);
  }
  int opt = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
    perror("setsockopt"); exit(1);
  }
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);
  if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
    perror("bind"); exit(1);
  }

  if (listen(sockfd, 10) == -1) {
    perror("listen"); exit(1);
  }
  return sockfd;
}
int connect_to_remote_server(const char * hostname, int port) {
  int sockfd;
  struct sockaddr_in remote_addr;
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket"); return -1;
  }
  memset(&remote_addr, 0, sizeof(remote_addr));
  remote_addr.sin_family = AF_INET;
  remote_addr.sin_port = htons(port);
  if (inet_pton(AF_INET, hostname, &remote_addr.sin_addr) <= 0) {
    perror("inet_pton"); close(sockfd); return -1;
  }
  if (connect(sockfd, (struct sockaddr *)&remote_addr, sizeof(remote_addr)) == -1) {
    perror("connect"); close(sockfd); return -1;
  }
  return sockfd;
}
struct thread_info {
  int client_sockfd, remote_sockfd;
};
#define MAX_EVENTS 16
void thread_func(void *arg) {
  struct thread_info *info = (struct thread_info *)arg;
  int client_sockfd = info->client_sockfd;
  int remote_sockfd = info->remote_sockfd;
  char buf[BUFSIZ];
  int epoll_fd, n, sent;
  struct epoll_event ev, events[MAX_EVENTS];
  epoll_fd = epoll_create1(0);
  if (epoll_fd == -1) {
    perror("epoll_create1"); goto err;
  }
  ev.events = EPOLLIN;  ev.data.fd = client_sockfd;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_sockfd, &ev) == -1) {
    perror("epoll_ctl: client_sockfd"); goto err;
  }
  ev.events = EPOLLIN;  ev.data.fd = remote_sockfd;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, remote_sockfd, &ev) == -1) {
    perror("epoll_ctl: remote_sockfd"); goto err;
  }
  for (;;) {
    int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    if (nfds == -1) {
      if (errno == EINTR) continue;
      perror("epoll_wait");  break;
    }
    for (int i = 0; i < nfds; i++) {
      int sockfd = events[i].data.fd;
      if (events[i].events & EPOLLIN) {
        n = recv(sockfd, buf, sizeof(buf), 0);
        if (n == -1) { perror("recv"); break; }
        if (n == 0) goto err;
        int dest_sockfd = (sockfd == client_sockfd) ? remote_sockfd : client_sockfd, ret;
        for (sent = 0; sent < n; sent += ret) {
          if ((ret = send(dest_sockfd, buf + sent, n - sent, 0)) == -1) {
            if (errno != EAGAIN & errno != EWOULDBLOCK)
              perror("send");
            break;
          }
        }
      }
    }
  }
err:
  close(client_sockfd);
  close(remote_sockfd);
  if (epoll_fd != -1) close(epoll_fd);
  free(info);
}
int main(int argc, char * argv[]) {
  if (argc != 4) {
    fprintf(stderr, "Usage: %s <local_port> <remote_host> <remote_port>\n", argv[0]);
    return 1;
  }
  signal(SIGPIPE, SIG_IGN);
  int local_port = atoi(argv[1]);
  const char * remote_host = argv[2];
  int remote_port = atoi(argv[3]);
  int server_sockfd = create_server_socket(local_port);
  for (;;) {
    int client_sockfd = accept(server_sockfd, NULL, NULL);
    set_nonblocking(client_sockfd);
    int remote_sockfd = connect_to_remote_server(remote_host, remote_port);
    if (remote_sockfd == -1) {
      printf("Failed to connect to remote server\n");
      close(client_sockfd);  continue;
    }
    set_nonblocking(remote_sockfd);
    struct thread_info * info = malloc(sizeof(struct thread_info));
    info->client_sockfd = client_sockfd;
    info->remote_sockfd = remote_sockfd;
    pthread_t thread;
    pthread_create(&thread, NULL, thread_func, info);
  }
}
