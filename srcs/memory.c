/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 14:51:39 by arsciand          #+#    #+#             */
/*   Updated: 2021/06/20 14:51:58 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void    free_core(t_core *core)
{
    (void)core;
    // free_opts_args(core->opts_args);
    // ft_strdel(&core->target);
    // free(core);
    // core = NULL;
}

void   exit_routine(t_core *core, int8_t status)
{
    (void)core;
    // free_core(core);
    exit(status);
}
