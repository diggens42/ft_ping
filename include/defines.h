/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwahl <fwahl@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 06:58:55 by fwahl             #+#    #+#             */
/*   Updated: 2025/09/26 20:18:22 by fwahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
# define DEFINES_H

#define FT_ERROR() fprintf(stderr, "File: %s Line: %d\n", __FILE__, __LINE__)

#define FLAG_VERBOSE    (1U << 0)   // -v
#define FLAG_HELP       (1U << 1)   // -?
// #define FLAG_NOROUTE    (1U << 2)   // -r
#define FLAG_NUMERIC    (1U << 2)   // -n
#define FLAG_QUIET      (1U << 3)   // -q
// #define FLAG_FLOOD      (1U << 5)   // -f
// #define FLAG_DEBUG      (1U << 6)   // -d
// #define FLAG_ROUTE      (1U << 7)   // -R (record route)

#define SET_FLAG(conf, flag)    ((conf)->flags |= (flag)) // bitwise OR turns on flag
#define CLEAR_FLAG(conf, flag)  ((conf)->flags &= ~(flag)) // turns of the flag with bitwise AND with NOT
#define HAS_FLAG(conf, flag)    ((conf)->flags & (flag))   // bitwise AND, checks if flag is set
#define TOGGLE_FLAG(conf, flag) ((conf)->flags ^= (flag))  // bitiwse XOR turns on/off flag

#define HOSTNAME_MAXLEN 256
#define PACKET_SIZE_DEFAULT 56  // packet_size = 56 → gives 64-byte ICMP messages
#define PING_INTERVAL 1
#define PARSE_ERROR -1

#endif
