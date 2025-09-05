#ifndef FT_PING_H
#define FT_PING_H

# include <stdio.h>        // printf, fprintf, sprintf, snprintf, perror
# include <stdlib.h>       // exit, malloc, free
# include <unistd.h>       // close, usleep, getpid, getuid
# include <string.h>       // strerror, memset, memcpy, strlen
# include <sys/socket.h>   // socket, setsockopt, sendto
# include <sys/types.h>    // general types (pid_t, size_t, etc.)
# include <netinet/in.h>   // ntohs, htons, sockaddr_in, IPPROTO_xxx
# include <netinet/ip.h>   // struct ip (raw IP header)
# include <netinet/ip_icmp.h> // struct icmp, ICMP_ECHO, etc.
# include <arpa/inet.h>    // inet_ntop, inet_pton
# include <netdb.h>        // getaddrinfo, getnameinfo, freeaddrinfo, gai_strerror
# include <signal.h>       // signal, alarm
# include <sys/time.h>     // gettimeofday, struct timeval
# include <errno.h>        // errno variable, error codes (EAGAIN, EINTR, etc.)



#endif