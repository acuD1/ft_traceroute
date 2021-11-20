/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_traceroute.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 15:06:14 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/20 11:37:16 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void    exec_traceroute(t_traceroute *traceroute)
{
    dprintf(STDOUT_FILENO,
        "ft_traceroute to %s (%s), %d hops max, %d byte packets\n",
        traceroute->buff_target,
        ft_strlen(traceroute->buff_ip)
            ? traceroute->buff_ip : traceroute->buff_target,
        traceroute->conf.hops, traceroute->conf.packetlen);
}
