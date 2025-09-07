/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:45:46 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/07 23:38:15 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

static bool    init_conf(t_conf *conf)
{
    if (!conf)
        return (false);
    
    ft_memset(conf, 0, sizeof(t_conf));
    ft_memset(conf->res_ip, 0, INET_ADDRSTRLEN);
    conf->tar = NULL;
    conf->dest.sin_family = AF_UNSPEC; //must set = AF_INET later!!
    conf->dest.sin_port = 0;
    conf->dest.sin_addr.s_addr = INADDR_ANY; //must set later
    conf->packet_size = PACKET_SIZE;
    conf->socket_fd = -1;
    conf->timeout = 1;
    conf->pid = getpid();
    conf->verbose = false;
    conf->help = false;
    conf->bypass_route = false;
    conf->ttl = 0;

    return (true);
}

static bool    init_status(t_status *status)
{
    if (!status)
        return (false);

    ft_memset(status, 0, sizeof(t_status));
    status->sent = 0;
    status->recv = 0;
    status->err = 0;
    status->max_rtt = 0.0;
    status->min_rtt = 0.0;
    status->sum_rtt = 0.0;
    status->start.tv_sec = 0;
    status->start.tv_usec = 0;

    return (true);
}

bool    init(t_ping *ping)
{
    if (!init_conf(&ping->conf))
    {
        ft_error("init: failed to init config struct");
        return (false);
    }

    if (!init_status(&ping->status))
    {
        ft_error("init: failed to init status struct");
        return (false);
    }

    return (true);
}
