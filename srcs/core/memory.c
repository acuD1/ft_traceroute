/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 14:51:39 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/20 15:38:32 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void                            free_traceroute(t_traceroute *traceroute)
{
    ft_strdel(&traceroute->packet);
}

void __attribute__ ((noreturn)) exit_routine(
                                    t_traceroute *traceroute, int8_t status)
{
    free_traceroute(traceroute);
    exit(status);
}
