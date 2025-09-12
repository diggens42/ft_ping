/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:45:46 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/12 23:41:20 by fwahl            ###   ########.fr       */
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
    conf->socket_fd = -1;
    conf->pid = getpid();

    conf->flags = 0;
    conf->opts.packet_size = PACKET_SIZE;
    conf->opts.timeout = 1;
    conf->opts.interval = 1;
    conf->opts.ttl = 0;
    conf->opts.count = 0;
    
    return (true);
}

static bool    init_stat(t_stat *stat)
{
    if (!stat)
        return (false);

    ft_memset(stat, 0, sizeof(t_stat));
    stat->sent = 0;
    stat->recv = 0;
    stat->lost = 0;
    stat->max_rtt = 0.0;
    stat->min_rtt = 0.0;
    stat->sum_rtt = 0.0;
    stat->start.tv_sec = 0;
    stat->start.tv_usec = 0;

    return (true);
}

bool    init(t_ping *ping)
{
    if (!init_conf(&ping->conf) || !init_stat(&ping->stat))
    {
        FT_ERROR();
        return (false);
    }

    return (true);
}
