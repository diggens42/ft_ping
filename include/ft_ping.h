/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:45:57 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/07 01:03:39 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

#include "headers.h"
#include "defines.h"
#include "structs.h"
#include "../libft/libft.h"


//functions

int main(int argc, char **argv);

bool    init(t_ping *ping);
bool    parse(int argc, char **argv, t_conf *conf);
bool	resolve_hostname(t_conf *conf);
bool    create_socket(int socket_fd);

void    ft_error(const char *msg);
void    print_use(const char *exec_name);
void	print_help(void);

#endif