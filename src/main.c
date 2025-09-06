/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:45:39 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/06 23:15:31 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"


int main(int argc, char **argv)
{
    t_ping ping;

    if (!init(&ping.conf, &ping.status))
        return (EXIT_ERROR);
    
    if (!parse(argc, argv, &ping.conf))
    {
        //add cleanup function here
        return (EXIT_FAILURE);
    }
    
    // help flag function
    // resolve hostname
    // create socket
    // signalhandler
    // start time
    // ping loop
    // free all

    return (EXIT_SUCCESS);
}