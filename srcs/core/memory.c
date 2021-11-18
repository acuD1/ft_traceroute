/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 14:51:39 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/18 14:49:13 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void                            free_traceroute(t_traceroute *traceroute)
{
    (void)traceroute;
    // free_opts_args(core->opts_args);
    // ft_strdel(&core->target);
    // free(core);
    // core = NULL;
}

void __attribute__ ((noreturn)) exit_routine(
                                    t_traceroute *traceroute, int8_t status)
{
    free_traceroute(traceroute);
    exit(status);
}
