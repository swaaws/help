#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
 
int main(int argc, char *argv[])
{
        struct sockaddr_in6 saddr, maddr;
        struct ipv6_mreq mreq;
        char buf[1400];
        ssize_t len;
        int sd, fd, on = 1, flag = 0, hops = 255, ifidx = 0;
        fd_set fds;

        if (argc < 3) {
                printf("\nUsage: %s <address> <port>\n\nExample: %s ff02::9999 9999\n\n", argv[0], argv[0]);
                return 1;
        }

        sd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
        if (sd < 0) {
                return 1;
        }
 
        if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) {
                return 1;
        }

        if (setsockopt(sd, IPPROTO_IPV6, IPV6_MULTICAST_IF, &ifidx, sizeof(ifidx))) {
                return 1;
        }

        if (setsockopt(sd, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &hops, sizeof(hops))) {
                return 1;
        }

        if (setsockopt(sd, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &on, sizeof(on))) {
                return 1;
        }
 
        memset(&saddr, 0, sizeof(saddr));
        saddr.sin6_family = AF_INET6;
        saddr.sin6_port = htons(atoi(argv[2]));
        saddr.sin6_addr = in6addr_any;
 
        if (bind(sd, (struct sockaddr *) &saddr, sizeof(saddr))) {
                return 1;
        }
 
        memset(&maddr, 0, sizeof(maddr));
        inet_pton(AF_INET6, argv[1], &maddr.sin6_addr);

        memcpy(&mreq.ipv6mr_multiaddr, &maddr.sin6_addr, sizeof(mreq.ipv6mr_multiaddr));
        mreq.ipv6mr_interface = ifidx;

        if (setsockopt(sd, IPPROTO_IPV6, IPV6_JOIN_GROUP, (char *) &mreq, sizeof(mreq))) {
                return 1;
        }
        FD_ZERO(&fds);
        FD_SET(sd, &fds);
        fd = open("/dev/stdout", O_WRONLY, NULL);
        if (fd < 0) {
                return 1;
        }

        while (1) {
                len = read(sd, buf, 1400);
                buf[len] = '\0';

                if (!len) {
                        break;
                } else if (len < 0) {
                        return 1;
                } else {
                        len = write(fd, buf, len);
                        flag++;
                }
        }
        close(sd);
        close(fd);
        return 0;
}