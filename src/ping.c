/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 05:56:17 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/07 02:04:37 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

// int ping_loop()
// {
//     while (g_run) {
//     // 1. Prepare ICMP packet
//     // 2. Send packet using sendto()
//     // 3. Receive reply using recvmsg()
//     // 4. Parse and validate reply
//     // 5. Calculate RTT
//     // 6. Display results
//     // 7. Update statistics
//     // 8. Wait 1 second (usleep(1000000))
// }
// }

static bool recv_ping(t_conf *conf, t_status *status)
{
    
}

static bool send_ping(t_conf *conf, t_status *status, int seq)
{
    
}

void    ping(t_ping *ping)
{
    int seq = 1;
    
    while (g_run)
    {
        if (!send_ping(&ping->conf, &ping->status, &seq))
        {
            if (ping->conf.verbose)
                fprintf(stderr, "ping loop: failed to send packet\n");
        }
        
        recv_ping(&ping->conf, &ping->status);
        seq++;
        
        if (g_run)
        {
            alarm(PING_INTERVAL);
            pause(); // not sure if allowed checking later
        }
    }
}