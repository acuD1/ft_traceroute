/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   default.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/20 12:08:07 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/20 12:08:42 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void    print_init(t_traceroute *traceroute)
{
    dprintf(STDOUT_FILENO,
        "ft_traceroute to %s (%s), %d hops max, %d byte packets\n",
        traceroute->buff_target, ft_strlen(traceroute->buff_ip)
            ? traceroute->buff_ip : traceroute->buff_target,
        traceroute->conf.hops, traceroute->conf.packetlen);
}
