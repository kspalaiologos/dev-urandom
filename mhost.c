// mhost: display the ip address associated with a hostname
#include <common.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char *argv[]) {
    ASSERT(argc == 2, "Usage: %s <hostname>\n", argv[0]);
    struct hostent *host = gethostbyname(argv[1]);
    ASSERT(host != NULL, "Error: Unable to resolve hostname\n");
    struct in_addr **addr_list = (struct in_addr **)host->h_addr_list;
    for (int i = 0; addr_list[i]; i++)
        printf("%s\n", inet_ntoa(*addr_list[i]));
}

