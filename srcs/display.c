/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 14:55:15 by arsciand          #+#    #+#             */
/*   Updated: 2021/06/20 15:48:54 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void     print_unallowed_opt(t_opts_args *opts_args)
{
    (void)opts_args;
    // if (opts_args->invalid)
    //     dprintf(STDERR_FILENO,
    //         "Bad option `--%s' (argc %d)\n", opts_args->invalid, opts_args->argc);
    // else
    //     dprintf(STDERR_FILENO,
    //         "Bad option `-%c' (argc %d)\n", (char)(opts_args->all % 128), opts_args->argc);
}

/* Build informations */
void     print_version(void)
{
    dprintf(STDOUT_FILENO, "ft_traceroute: v.%s-%s-%s-%s\n",
        BUILD_VERSION_STRING,
        BUILD_RELEASE_STRING,
        BUILD_PATCH_STRING,
        BUILD_DATE_STRING);
}

void     print_usage(void)
{
    dprintf(STDERR_FILENO, "Usage: ft_traceroute [-h] [--help] <destination>\n");
}
