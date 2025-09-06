/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:45:57 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/06 15:07:21 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

#include "headers.h"
#include "defines.h"
#include "structs.h"
#include "../libft/libft.h"

// globals
extern t_ping_conf      g_config;
extern t_ping_status	g_stats;
extern int				g_run;

//functions

int main(int argc, char **argv);

bool    init(t_ping_conf *conf, t_ping_status *status);
bool    parse(int argc, char **argv, t_ping_conf *conf);

void    ft_error(const char *msg);
void    print_use(const char *exec_name);
void	print_help(void);

#endif