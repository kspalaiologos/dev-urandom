// NTP client

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "common.h"

#define ENDIAN_SWAP32(d) ((d >> 24) | ((d & 0x00ff0000) >> 8) | ((d & 0x0000ff00) << 8) | ((d & 0x000000ff) << 24))

struct ntp {
	u8 flags, stratum, poll, precision;
	u32 root_delay, root_dispersion;
	u8 referenceID[4];
	u32 ref_ts_sec, ref_ts_frac, origin_ts_sec, origin_ts_frac;
	u32 recv_ts_sec, recv_ts_frac, trans_ts_sec, trans_ts_frac;
} __attribute__((__packed__));

int main(int argc, char *argv[]) {
	char * port = "123";
	struct addrinfo hints, * res, * ap;
	socklen_t addrlen = sizeof(struct sockaddr_storage);
	struct ntp packet;
	i32 server_sock, error;
	time_t total_secs;
	struct tm * now;
	ASSERT(argc > 1, "%s <server> [port]\n", argv[0]);
	if (argc > 2) port = argv[2];
	memset(&packet, 0, sizeof(struct ntp));
	packet.flags = 0xE3; // Default NTP version, client mode
	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_DGRAM;
	ASSERT((error = getaddrinfo(argv[1], port, &hints, &res)) == 0, "getaddrinfo: %s\n", gai_strerror(error));
	for (ap = res; ap; ap = ap->ai_next)
		if ((server_sock = socket(ap->ai_family, ap->ai_socktype, ap->ai_protocol)) != -1) break;
	ASSERT(ap, "socket(): %s\n", strerror(errno));
	ASSERT(sendto(server_sock, &packet, sizeof(struct ntp), 0, ap->ai_addr, addrlen) != -1, "sendto(): %s\n", strerror(errno));
	ASSERT(sizeof(struct ntp) == recvfrom(server_sock, &packet, sizeof(struct ntp), 0, ap->ai_addr, &addrlen), "recvfrom(): %s\n", strerror(errno));
	freeaddrinfo(res);
	packet.recv_ts_sec = ENDIAN_SWAP32(packet.recv_ts_sec);
	printf("LI: %u\n", packet.flags >> 6);
	printf("VN: %u\n", (packet.flags & 0x3f) >> 3);
	printf("mode: %u\n", packet.flags & 0x7);
	printf("stratum: %u\n", packet.stratum);
	printf("poll: %u\n", packet.poll);
	printf("precision: %u\n", packet.precision);
	printf("root delay: %u\n", ENDIAN_SWAP32(packet.root_delay));
	printf("root dispersion: %u\n", ENDIAN_SWAP32(packet.root_dispersion));
	printf("reference ID: %u.%u.%u.%u\n", packet.referenceID[0], packet.referenceID[1], packet.referenceID[2], packet.referenceID[3]);
	printf("reference timestamp: %u.%u\n", ENDIAN_SWAP32(packet.ref_ts_sec), ENDIAN_SWAP32(packet.ref_ts_frac));
	printf("origin timestamp: %u.%u\n", ENDIAN_SWAP32(packet.origin_ts_sec), ENDIAN_SWAP32(packet.origin_ts_frac));
	printf("receive timestamp: %u.%u\n", packet.recv_ts_sec, ENDIAN_SWAP32(packet.recv_ts_frac));
	printf("transmit timestamp: %u.%u\n", ENDIAN_SWAP32(packet.trans_ts_sec), ENDIAN_SWAP32(packet.trans_ts_frac));
	printf("UNIX time: %lu\n", total_secs = packet.recv_ts_sec - 2208988800L); // Unix time starts at 1970, NTP at 1900
	now = localtime(&total_secs);
	printf("%02d/%02d/%d %02d:%02d:%02d\n", now->tm_mday, now->tm_mon+1, now->tm_year+1900, now->tm_hour, now->tm_min, now->tm_sec);
}
