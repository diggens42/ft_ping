/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 15:05:29 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/07 18:43:52 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"
#include "defines.h"

typedef struct s_conf
{
    char                *tar;                   // hostname
    char                res_ip[INET_ADDRSTRLEN];// IP after DNS lookup
    struct sockaddr_in  dest;                   // dest address -->  sendto()
    int                 packet_size;            // size of ICMP payload in bytes
    int                 socket_fd;              // raw socket fd
    int                 timeout;                // timeout for reply (in ms?)
    
    //flags
    bool                verbose;                // -v flag
    bool                help;                   // -? on windows (-h/ --help on linux/macOS) flag
    bool                bypass_route;           // -r bypass route table
    int                 ttl;                    // -ttl flag --> time to live 
    pid_t               pid;                    // PID for ICMP identifier
}   t_conf;


typedef struct s_status
{
    int sent;       // nr of packages sent
    int recv;       // nr of packages received
    int err;        // nr of errors/timouts
    double max_rtt;  // max round trip time (in ms)
    double min_rtt;  // minimum round trip time (in ms)
    double sum_rtt;  // sum of all round trip times for avg calc
    struct timeval start;  // start time ping;
}   t_status;

typedef struct s_packet
{
    struct icmphdr  header; // ICMP header (type, code, checksum, id, sequence)
    char   data[PACKET_SIZE]; //ICMP payload
}   t_packet;

typedef struct s_ping
{
    t_conf      conf;
    t_status    status;
}   t_ping;