/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:45:39 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/07 18:30:53 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

volatile sig_atomic_t   g_run = 1;

int main(int argc, char **argv)
{
    t_ping ping;

    if (!init(&ping))
        return (EXIT_ERROR);
    
    if (!parse(argc, argv, &ping.conf))
    {
        //cleanup()
        return (EXIT_FAILURE);
    }

    if (ping.conf.help)
    {
        print_help();
        //cleanup()
        return (EXIT_SUCCESS);
    }

    if (!resolve_hostname(&ping.conf))
    {
        //cleanup()
        return (EXIT_FAILURE);
    }
    
    if (!create_socket(&ping.conf))
    {
        //cleanup()
        return (EXIT_FAILURE);
    }
    
    init_sig_handlers();
    gettimeofday(&ping.status.start, NULL);
    ft_ping(&ping);
    
    //cleanup()

    return (EXIT_SUCCESS);
}