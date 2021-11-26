/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 14:15:46 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/23 21:56:32 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void    init_traceroute(t_traceroute *traceroute)
{
    ft_memset(traceroute, 0, sizeof(t_traceroute));

    traceroute->conf.probes         = DEFAULT_PROBES;
    traceroute->conf.packetlen      = DEFAULT_PACKETLEN;
    traceroute->conf.hops           = DEFAULT_HOPS;
    traceroute->conf.protocol       = DEFAULT_PROTOCOL;
    traceroute->conf.queries        = DEFAULT_QUERIES;
    traceroute->conf.start_probe    = DEFAULT_START_PROBE;
    traceroute->conf.start_ttl      = DEFAULT_START_HOPS;
}
