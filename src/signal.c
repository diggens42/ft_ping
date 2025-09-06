/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 01:10:40 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/07 01:41:24 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

static void sig_handler(int sig)
{
    if (sig == SIGINT || sig == SIGQUIT)
    {
        g_run = false;
        alarm(0); // cancel alarms
    }
    else if (sig == SIGALRM)
    {
        //handled in ping_loop --> alarm for next ping
    }
}

void    init_sig_handlers(void)
{
    signal(SIGINT, sig_handler);
    signal(SIGQUIT, sig_handler);
    signal(SIGALRM, sig_handler);
}