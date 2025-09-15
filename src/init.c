/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:45:46 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/15 15:23:24 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

static bool    init_conf(t_conf *conf)
{
    if (!conf)
        return (false);
    ft_memset(conf, 0, sizeof(t_conf));
    conf->dest.sin_family = AF_UNSPEC; //must set = AF_INET later!!
    conf->socket_fd = -1;
    conf->pid = getpid();
    conf->opts.packet_size = PACKET_SIZE_DEFAULT;
    conf->opts.packet_type = ICMP_ECHO;
    conf->opts.interval = 1;
    conf->opts.linger = 1;
    return (true);
}

static bool    init_stat(t_stat *stat)
{
    if (!stat)
        return (false);

    ft_memset(stat, 0, sizeof(t_stat));
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
