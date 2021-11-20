/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 14:15:46 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/19 15:02:03 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void    init_traceroute(t_traceroute *traceroute)
{
    ft_memset(traceroute, 0, sizeof(t_traceroute));

    traceroute->conf.probes     = DEFAULT_PROBES;
    traceroute->conf.packetlen  = DEFAULT_PACKETLEN;
    traceroute->conf.hops       = DEFAULT_HOPS;
}
