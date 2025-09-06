/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:45:57 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/06 03:42:10 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include <stdbool.h>
# include "../libft/libft.h"

// defines

#define PACKET_SIZE 56   // packet_size = 56 → gives 64-byte ICMP messages

#define EXIT_ERROR 2



//
// structs
//

typedef struct s_ping_conf
{
    char                *tar;           // hostname
    char                *res_ip;        // IP after DNS lookup
    struct sockaddr_in  dest;           // dest address -->  sendto()
    int                 packet_size;    // size of ICMP payload in bytes
    int                 socket_fd;      // raw socket fd
    int                 timeout;        // timeout for reply (in ms?)
    bool                verbose;        // -v flag
    bool                help;           // -? on windows (-h/ --help on linux/macOS) flag
    pid_t               pid;            // PID for ICMP identifier
    
}   t_ping_conf;


typedef struct s_ping_status
{
    int sent;       // nr of packages sent
    int recv;       // nr of packages received
    int err;        // nr of errors/timouts
    double max_rtt;  // max round trip time (in ms)
    double min_rtt;  // minimum round trip time (in ms)
    double sum_rtt;  // sum of all round trip times for avg calc
    struct timeval start;  // start time ping;
}   t_ping_status;

typedef struct s_packet
{
    struct icmphdr  header; // ICMP header (type, code, checksum, id, sequence)
    char   data[PACKET_SIZE]; //ICMP payload
} t_packet;


// globals

extern t_ping_conf      g_config;
extern t_ping_status	g_stats;
extern int				g_run;


//
// functions
//

int main(int argc, char **argv);

bool    init(t_ping_conf *conf, t_ping_status *status);
bool    parse(int argc, char **argv, t_ping_conf *conf);

void    ft_error(const char *msg);
void    print_use(const char *exec_name);
void	print_help(void);

#endif