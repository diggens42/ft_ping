/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 15:05:29 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/10 01:22:22 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"
#include "defines.h"

typedef struct s_flags
{
    bool                verbose;        // -v flag 
    bool                help;           // -? / --help flag
    bool                bypass_route;   // -r bypass route table
}   t_flags;

typedef struct s_opts
{
    int                 ttl;                    // --ttl flag value --> time to live
    int                 timeout;                // timeout for reply (in ms?)
    int                 packet_size;            // size of ICMP payloads in bytes
    double              interval;               // -i / --interval flag --> interval of seconds in between packages sent
    long long           count;                  // -c / --count=N flag value --> num of echo req to send 
}   t_opts;

typedef struct s_conf
{
    char                *tar;                   // hostname
    char                res_ip[INET_ADDRSTRLEN];// IP after DNS lookup
    struct sockaddr_in  dest;                   // dest address -->  sendto()
    int                 socket_fd;              // raw socket fd
    pid_t               pid;                    // PID for ICMP identifier
    t_flags             flags;
    t_opts              opts;
}   t_conf;


typedef struct s_stat
{
    int                 sent;       // nr of packages sent
    int                 recv;       // nr of packages received
    int                 lost;        // nr of errors/timouts
    double              max_rtt;  // max round trip time (in ms)
    double              min_rtt;  // minimum round trip time (in ms)
    double              sum_rtt;  // sum of all round trip times for avg calc
    struct timeval      start;  // start time ping;
}   t_stat;

typedef struct s_packet
{
    struct icmphdr      header; // ICMP header (type, code, checksum, id, sequence)
    char                data[PACKET_SIZE]; //ICMP payload
}   t_packet;

typedef struct s_ping
{
    t_conf  conf;
    t_stat  stat;
}   t_ping;

typedef enum {
    OPT_NO_ARG,
    OPT_REQUIRED_ARG,
} opt_type;

typedef struct {
    const char  *short_opt;     // e.g., "-v" (NULL if no short form)
    const char  *long_opt;      // e.g., "--verbose"
    opt_type    type;           // OPT_NO_ARG or OPT_REQUIRED_ARG
    bool        *flag_ptr;      // For boolean flags (NULL for value options)
    int         (*parser)(const char *value, t_conf *conf);  // Custom parser function
} t_parse;
