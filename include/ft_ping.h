/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:45:57 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/10 01:33:56 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

#include "headers.h"
#include "defines.h"
#include "structs.h"
#include "../libft/libft.h"

//globals
extern volatile sig_atomic_t g_run;

//functions

int main(int argc, char **argv);

bool    init(t_ping *ping);
bool    parse(int argc, char **argv, t_conf *conf);
bool	resolve_hostname(t_conf *conf);
bool    create_socket(t_conf *conf);
void    init_sig_handlers(void);
void    ft_ping(t_ping *ping);

void	print_help(void);
double	get_ms(struct timeval *sent, struct timeval *recv);

#endif