/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 13:19:29 by arsciand          #+#    #+#             */
/*   Updated: 2021/06/19 19:05:47 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

# define UNALLOWED_OPT          1ULL << 63
# define ALLOWED_OPT            "vhabc"
# define ALLOWED_OPT_ARG        "abc"
# define TEST1                  "test1"
# define TEST2                  "test2"
# define ALLOWED_OPT_TAB        ((const char *[]){TEST1, TEST2, NULL})
# define ALLOWED_OPT_TAB_ARG     ((const char *[]){TEST2, NULL})

void     print_unallowed_opt(t_opts_args *opts_args)
{
    if (opts_args->invalid)
        fprintf(stderr,
            "ft_traceroute: unrecognized option '--%s'\n", opts_args->invalid);
    else
        fprintf(stderr,
            "ft_traceroute: invalid option -- '%c'\n", (char)(opts_args->all % 128));
}

uint8_t  get_opts_args_handler(t_core *core, int argc, char **argv)
{
    t_opts_conf opts_conf;

    ft_memset(&opts_conf, 0, sizeof(t_opts_conf));
    opts_conf.allowed_opt = ALLOWED_OPT;
    opts_conf.allowed_opt = ALLOWED_OPT_ARG;
    opts_conf.allowed_opt_tab = ALLOWED_OPT_TAB;
    opts_conf.allowed_opt_tab_arg = ALLOWED_OPT_TAB_ARG;
    if (argc < 2)
    {
        return (FAILURE);
    }
    if (!(core->opts_args = ft_get_opts_args(argc, argv, &opts_conf)))
        return (FAILURE);
    if (core->opts_args->all & UNALLOWED_OPT)
    {
        print_unallowed_opt(core->opts_args);
        return (FAILURE);
    }
    return (SUCCESS);
}

int     main(int argc, char *argv[])
{
    t_core  core;

    ft_memset(&core, 0, sizeof(core));

    if (get_opts_args_handler(&core, argc, argv) != SUCCESS)
        return (FAILURE);

    debug_opts_args(core.opts_args);
    return (SUCCESS);
}
