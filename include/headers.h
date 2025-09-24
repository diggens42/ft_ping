/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 06:51:10 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/24 17:23:51 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADERS_H
# define HEADERS_H

// #ifndef _POSIX_C_SOURCE
// # define _POSIX_C_SOURCE 200112L
// # define _POSIX_C_SOURCE 199309L
// #endif

# include <stdio.h>        // printf, fprintf, sprintf, snprintf, perror
# include <stdlib.h>       // exit, malloc, free
# include <unistd.h>       // close, usleep, getpid, getuid
# include <string.h>       // strerror, memset, memcpy, strlen
# include <sys/socket.h>   // socket, setsockopt, sendto
# include <sys/types.h>    // general types (pid_t, size_t, etc.)
# include <netinet/ip.h>   // iphdr struct
# include <netinet/in.h>   // ntohs, htons, sockaddr_in, IPPROTO_xxx // // struct ip (raw IP header)
# include <netinet/ip_icmp.h> // struct icmp, ICMP_ECHO, etc.
# include <arpa/inet.h>    // inet_ntop, inet_pton
# include <netdb.h>        // getaddrinfo, getnameinfo, freeaddrinfo, gai_strerror
# include <signal.h>       // signal, alarm
# include <sys/time.h>     // gettimeofday, struct timeval
# include <errno.h>        // errno variable, error codes (EAGAIN, EINTR, etc.)
# include <stddef.h>       // offsetof macro
# include <stdbool.h>
# include <time.h>
# include <math.h>

#endif
