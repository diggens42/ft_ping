/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 06:58:55 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/10 00:31:48 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
# define DEFINES_H

#define FT_ERROR() fprintf(stderr, "File: %s Line: %d\n", __FILE__, __LINE__)

#define PACKET_SIZE 56   // packet_size = 56 → gives 64-byte ICMP messages
#define PING_INTERVAL 1
#define EXIT_ERROR 2
#define PARSE_ERROR -1

#endif
