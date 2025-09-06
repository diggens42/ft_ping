/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:45:39 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/06 03:44:01 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

t_ping_conf     g_conf;
t_ping_status   g_status;
bool            g_run = true;

int main(int argc, char **argv)
{
    if (!init(&g_conf, &g_status))
        return (EXIT_ERROR);
    
    if (!parse(argc, argv, &g_conf))
    {
        //add cleanup function here
        return (EXIT_FAILURE);
    }
    

    return (EXIT_SUCCESS);
}