/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:45:39 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/14 23:46:19 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

volatile sig_atomic_t   g_run = 1;

int main(int argc, char **argv)
{
    t_ping ping;

    if (!init(&ping))
        return (EXIT_FAILURE);

    if (!parse(argc, argv, &ping.conf))
        return (EXIT_FAILURE);

    if (HAS_FLAG(&ping.conf, FLAG_HELP))
    {
        print_help();
        return (EXIT_SUCCESS);
    }

    if (!resolve_hostname(&ping.conf))
        return (EXIT_FAILURE);
    
    if (!create_socket(&ping.conf))
        return (EXIT_FAILURE);
    
    init_sig_handlers();

    ft_ping(&ping);
    
    print_stats(&ping);

    if (ping.conf.socket_fd >= 0)
        close(ping.conf.socket_fd);

    return (ping.stat.recv > 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
