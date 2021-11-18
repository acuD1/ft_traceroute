/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opts_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 14:57:34 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/18 17:04:54 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

static uint8_t  set_opts_args_failure(t_opts_args *opts_args)
{
    free_opts_args(opts_args);
    return (FAILURE);
}

uint8_t  set_opts_args(t_traceroute *traceroute, int argc, char **argv)
{
    (void)argv;
    t_opts_conf     opts_conf;
    t_opts_args     opts_args;
    t_opt_set_db    *tmp                = NULL;
    (void)tmp;

    ft_memset(&opts_conf, 0, sizeof(t_opts_conf));
    ft_memset(&opts_args, 0, sizeof(t_opts_args));
    opts_conf.allowed_opt               = ALLOWED_OPT;
    opts_conf.allowed_opt_arg           = ALLOWED_OPT_ARG;
    opts_conf.allowed_opt_tab           = ALLOWED_OPT_TAB;
    opts_conf.allowed_opt_tab_arg       = ALLOWED_OPT_TAB_ARG;
    opts_conf.allowed_opt_assign        = ALLOWED_OPT_ASSIGN;
    opts_conf.allowed_opt_assign_tab    = ALLOWED_OPT_ASSIGN_TAB;

    if (argc < 2)
    {
        print_usage();
        exit_routine(traceroute, EXIT_SUCCESS);
    }

    if (ft_get_opts_args(&opts_args, &opts_conf, argc, argv) != SUCCESS)
        return (set_opts_args_failure(&opts_args));
    debug_opts_args(&opts_args);

    if (opts_args.all & UNALLOWED_OPT)
    {
        print_unallowed_opt(&opts_args);
        return (set_opts_args_failure(&opts_args));
    }

    if (opts_args.all & H_OPT || (tmp = get_opt_set_db(&opts_args.opt_set, H_OPT_STR)) != NULL)
    {
        print_usage();
        exit_routine(traceroute, EXIT_SUCCESS);
    }

    if (opts_args.all & VV_OPT)
    {
        print_version();
        exit_routine(traceroute, EXIT_SUCCESS);
    }

    if (opts_args.all & M_OPT || (tmp = get_opt_set_db(&opts_args.opt_set, M_OPT_STR)))
    {
        if (!tmp)
            tmp = get_opt_set_db(&opts_args.opt_set, "m");
        if (tmp->arg)
        {
            if (ft_isnum(tmp->arg))
            {
                int tmp_hops = ft_atoi(tmp->arg);
                if (tmp_hops < MIN_HOPS || tmp_hops > MAX_HOPS)
                {
                    if (tmp_hops == 0)
                        dprintf(STDERR_FILENO, "first hop out of range\n");
                    else
                        dprintf(STDERR_FILENO, "max hops cannot be more than 255\n");
                    return (set_opts_args_failure(&opts_args));
                }
                traceroute->conf.hops = (uint32_t)tmp_hops;
            }
            else
            {
                dprintf(STDERR_FILENO, "Cannot handle `%s%s' option with arg `%s' (argc %d)\n", opts_args.all & M_OPT ? "-" : "--", tmp->current, tmp->arg, tmp->argc);
                return (set_opts_args_failure(&opts_args));
            }
        }
        else
        {
            dprintf(STDERR_FILENO, "Option `%s%s' (argc %d) requires an argument: `%s'\n",  opts_args.all & M_OPT ? "-" : "--", tmp->current, tmp->argc,  opts_args.all & M_OPT ? "-m max_ttl" : "--max-hops=max_ttl");
            return (set_opts_args_failure(&opts_args));
        }
    }
    // if (core->opts_args->all & Q_OPT || (tmp_opt = get_opt_set_db(&core->opts_args->opt_set, Q_OPT_ARRAY)))
    // {
    //     if (!tmp_opt)
    //         tmp_opt = get_opt_set_db(&core->opts_args->opt_set, "q");
    //     if (tmp_opt->arg)
    //     {
    //         if (ft_is_number(tmp_opt->arg))
    //         {
    //             core->probes = ft_atoi(tmp_opt->arg);
    //             if (core->probes < MIN_PROBES || core->probes > MAX_PROBES)
    //             {
    //                 dprintf(STDERR_FILENO, "no more than 10 probes per hop\n");
    //                 exit_routine(core, FAILURE);
    //             }
    //         }
    //         else
    //         {
    //             dprintf(STDERR_FILENO, "Cannot handle `%s%s' option with arg `%s' (argc %d)\n", core->opts_args->all & Q_OPT ? "-" : "--", tmp_opt->current, tmp_opt->arg, tmp_opt->argc + 1);
    //             exit_routine(core, FAILURE);
    //         }
    //     }
    //     else
    //     {
    //         dprintf(STDERR_FILENO, "Option `%s%s' (argc %d) requires an argument: `%s'\n", core->opts_args->all & Q_OPT ? "-" : "--", tmp_opt->current, tmp_opt->argc, core->opts_args->all & Q_OPT ? "-q nqueries" : "--queries=nqueries");
    //         exit_routine(core, FAILURE);
    //     }
    // }
    // if (!(args_len = ft_lstlen(core->opts_args->args)))
    // {
    //     dprintf(STDERR_FILENO, "Specify \"host\" missing argument.\n");
    //     exit_routine(core, FAILURE);
    // }
    // if (args_len > 2)
    // {
    //     tmp_arg = ((t_args_db *)get_arg(&core->opts_args->args, POSITION(3))->content);
    //     dprintf(STDERR_FILENO, "Extra arg `%s' (position 3, argc %d)\n", tmp_arg->arg, tmp_arg->argc);
    //     exit_routine(core, FAILURE);
    // }
    // if (args_len > 1)
    // {
    //     tmp_arg = ((t_args_db *)get_arg(&core->opts_args->args, POSITION(2))->content);
    //     if (ft_is_number(tmp_arg->arg) != TRUE)
    //     {
    //         dprintf(STDERR_FILENO, "Cannot handle \"packetlen\" cmdline arg `%s' on position 2 (argc %d)\n", tmp_arg->arg,  tmp_arg->argc);
    //         exit_routine(core, FAILURE);
    //     }
    //     else
    //     {
    //         core->packetlen = ft_atoi(tmp_arg->arg);
    //         if (core->packetlen < MIN_PACKETLEN)
    //             core->packetlen = MIN_PACKETLEN;
    //         if (core->packetlen > MAX_PACKETLEN)
    //         {
    //             dprintf(STDERR_FILENO, "too big packetlen %d specified\n", core->packetlen);
    //             exit_routine(core, FAILURE);
    //         }
    //     }
    // }
    free_opts_args(&opts_args);
    return (SUCCESS);
}
