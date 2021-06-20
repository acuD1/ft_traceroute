/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opts_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 14:57:34 by arsciand          #+#    #+#             */
/*   Updated: 2021/06/20 14:57:54 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

uint8_t  get_opts_args_handler(t_core *core, int argc, char **argv)
{
    t_opts_conf opts_conf;
    size_t      args_len    = 0;

    ft_memset(&opts_conf, 0, sizeof(t_opts_conf));
    opts_conf.allowed_opt = ALLOWED_OPT;
    opts_conf.allowed_opt_tab = ALLOWED_OPT_TAB;
    if (argc < 2)
    {
        print_usage();
        exit_routine(core, SUCCESS);
    }
    if (!(core->opts_args = ft_get_opts_args(argc, argv, &opts_conf)))
        return (FAILURE);
    if (core->opts_args->all & UNALLOWED_OPT)
    {
        print_unallowed_opt(core->opts_args);
        exit_routine(core, FAILURE);
    }
    if (core->opts_args->all & H_OPT || get_opt_set_arg(&core->opts_args->opt_set, H_OPT_ARRAY, &opts_conf))
    {
        print_usage();
        exit_routine(core, SUCCESS);
    }
    args_len = ft_lstlen(core->opts_args->args);
    if (args_len > 2)
    {
        t_list   *arg = get_arg(&core->opts_args->args, 3);
        fprintf(stderr, "Extra arg `%s' (position 3, argc %d)\n", ((t_args_db *)arg->content)->arg, ((t_args_db *)arg->content)->argc);
        exit_routine(core, FAILURE);
    }
    if (args_len > 1)
    {
        t_list   *arg = get_arg(&core->opts_args->args, 2);
        if (ft_is_number(((t_args_db *)arg->content)->arg) != TRUE)
        {
            fprintf(stderr, "Cannot handle \"packetlen\" cmdline arg `%s' on position 2 (argc %d)\n", ((t_args_db *)arg->content)->arg,  ((t_args_db *)arg->content)->argc);
            exit_routine(core, FAILURE);
        }
        else
            core->packetlen = ft_atoi(((t_args_db *)arg->content)->arg);
    }
    return (SUCCESS);
}
