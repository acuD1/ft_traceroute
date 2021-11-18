/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 14:37:35 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/18 14:38:32 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void     print_unallowed_opt(t_opts_args *opts_args)
{
    if (opts_args->invalid)
        dprintf(STDERR_FILENO, "Bad option `--%s' (argc %d)\n",
            opts_args->invalid, opts_args->argc);
    else
        dprintf(STDERR_FILENO, "Bad option `-%c' (argc %d)\n",
            (char)(opts_args->all % 128), opts_args->argc);
}
