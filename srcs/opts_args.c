/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opts_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 14:57:34 by arsciand          #+#    #+#             */
/*   Updated: 2021/06/23 18:48:37 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

uint8_t  get_opts_args_handler(t_core *core, int argc, char **argv)
{
    (void)core;
    (void)argc;
    (void)argv;
    // t_opts_conf     opts_conf;
    // t_opt_set_db    *tmp_opt    = NULL;
    // t_args_db       *tmp_arg    = NULL;
    // size_t          args_len    = 0;

    // ft_memset(&opts_conf, 0, sizeof(t_opts_conf));
    // opts_conf.allowed_opt           = ALLOWED_OPT;
    // opts_conf.allowed_opt_arg       = ALLOWED_OPT_ARG;
    // opts_conf.allowed_opt_tab       = ALLOWED_OPT_TAB;
    // opts_conf.allowed_opt_tab_arg   = ALLOWED_OPT_TAB_ARG;
    // if (argc < 2)
    // {
    //     print_usage();
    //     exit_routine(core, SUCCESS);
    // }
    // if (!(core->opts_args = ft_get_opts_args(argc, argv, &opts_conf)))
    //     return (FAILURE);
    // if (core->opts_args->all & UNALLOWED_OPT)
    // {
    //     print_unallowed_opt(core->opts_args);
    //     exit_routine(core, FAILURE);
    // }
    // if (core->opts_args->all & V_OPT || get_opt_set_db(&core->opts_args->opt_set, V_OPT_ARRAY))
    // {
    //     print_version();
    //     exit_routine(core, SUCCESS);
    // }
    // if (get_opt_set_db(&core->opts_args->opt_set, H_OPT_ARRAY))
    // {
    //     print_usage();
    //     exit_routine(core, SUCCESS);
    // }
    // if (core->opts_args->all & M_OPT || (tmp_opt = get_opt_set_db(&core->opts_args->opt_set, M_OPT_ARRAY)))
    // {
    //     if (!tmp_opt)
    //         tmp_opt = get_opt_set_db(&core->opts_args->opt_set, "m");
    //     if (tmp_opt->arg)
    //     {
    //         if (ft_is_number(tmp_opt->arg))
    //         {
    //             core->hops = ft_atoi(tmp_opt->arg);
    //             if (core->hops < MIN_HOPS || core->hops > MAX_HOPS)
    //             {
    //                 if (core->hops == 0)
    //                     dprintf(STDERR_FILENO, "first hop out of range\n");
    //                 else
    //                     dprintf(STDERR_FILENO, "max hops cannot be more than 255\n");
    //                 exit_routine(core, FAILURE);
    //             }
    //         }
    //         else
    //         {
    //             dprintf(STDERR_FILENO, "Cannot handle `%s%s' option with arg `%s' (argc %d)\n", core->opts_args->all & M_OPT ? "-" : "--", tmp_opt->current, tmp_opt->arg, tmp_opt->argc + 1);
    //             exit_routine(core, FAILURE);
    //         }
    //     }
    //     else
    //     {
    //         dprintf(STDERR_FILENO, "Option `%s%s' (argc %d) requires an argument: `%s'\n",  core->opts_args->all & M_OPT ? "-" : "--", tmp_opt->current, tmp_opt->argc, core->opts_args->all & M_OPT ? "-m max_ttl" : "--max-hops=max_ttl");
    //         exit_routine(core, FAILURE);
    //     }
    // }
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
    return (SUCCESS);
}
