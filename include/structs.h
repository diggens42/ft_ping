/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 15:05:29 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/06 15:06:20 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"
#include "defines.h"

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

