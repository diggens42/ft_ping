/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 15:05:29 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/13 02:49:27 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "headers.h"
#include "defines.h"

typedef struct s_opts
{
    double              interval;               // -i / --interval flag --> interval of seconds in between packages sent
    uint32_t            count;                  // -c / --count=N flag value --> num of echo req to send 
    uint32_t            timeout;                // -w timeout in seconds
    uint32_t            linger;                 // -W: per packet timeout in secs
    uint16_t            packet_size;            // -s size of ICMP payloads in bytes
    uint8_t             packet_type;            // -t / --type=TYPE flag -->look notes for ICMP types
    uint8_t             pattern[16];            // -p payload pattern
    uint8_t             pattern_len;            // 0-16
    uint8_t             ttl;                    // --ttl flag value --> time to live
    uint8_t             tos;                    // -T type of service
}   t_opts;

typedef struct s_conf
{
    struct sockaddr_in  dest;                   // dest address -->  sendto()
    char                *tar;                   // original target argument
    char                res_ip[INET_ADDRSTRLEN];// IP after DNS lookup
    char                hostname[256];          // resolved hostname
    pid_t               pid;                    // PID for ICMP identifier
    int                 socket_fd;              // raw socket fd
    uint8_t             flags;                  // 8byte bitmask instead of bool flag struct
    t_opts              opts;
}   t_conf;


typedef struct s_stat
{
    struct timeval      start;  // start time ping;
    double              sum_rtt;  // sum of all round trip times for avg calc
    double              max_rtt;  // max round trip time (in ms)
    double              min_rtt;  // minimum round trip time (in ms)
    uint32_t            sent;       // nr of packages sent
    uint32_t            recv;       // nr of packages received
    uint32_t            lost;       // nr of errors/timouts
}   t_stat;

typedef struct s_ping
{
    t_conf  conf;
    t_stat  stat;
}   t_ping;

typedef struct s_packet
{
    struct icmphdr      header; // ICMP header (type, code, checksum, id, sequence)
    char                data[PACKET_SIZE]; //ICMP payload
}   t_packet;

typedef struct s_opt_def
{
    const char  *short_opt;     // e.g., "-v" (NULL if no short form)
    const char  *long_opt;      // e.g., "--verbose"
    bool        has_arg;         // OPT_NO_ARG or OPT_REQUIRED_ARG
    uint8_t     flag_bit;      // which flag to set, 0 means not a flag
    int         (*parser)(const char *val, t_conf *conf);  // Custom parser function
}   t_opt_def;
