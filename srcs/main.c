/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 13:19:29 by arsciand          #+#    #+#             */
/*   Updated: 2021/06/20 14:58:23 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

int     main(int argc, char *argv[])
{
    t_core  core;

    ft_memset(&core, 0, sizeof(core));
    core.packetlen = DEFAULT_PACKETLEN;

    if (get_opts_args_handler(&core, argc, argv) != SUCCESS)
        return (EXIT_FAILURE);

        /* Check if ft_ping executed as root */
    if (getuid())
    {
        fprintf(stderr, "ft_traceroute: socket: Operation not permitted\n");
        exit_routine(&core, FAILURE);
    }

    debug_opts_args(core.opts_args);
    free_core(&core);
    return (EXIT_SUCCESS);
}
