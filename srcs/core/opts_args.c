/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opts_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 14:57:34 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/22 18:53:52 by arsciand         ###   ########.fr       */
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
    t_opts_conf     opts_conf;
    t_opts_args     opts_args;
    t_opt_set_db    *tmp                = NULL;
    t_args_db       *tmp_args_db        = NULL;
    size_t          n_args              = 0;

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

    if (opts_args.all & UNALLOWED_OPT)
    {
        print_unallowed_opt(&opts_args);
        return (set_opts_args_failure(&opts_args));
    }

    if (opts_args.all & H_OPT || (tmp = get_opt_set_db(&opts_args.opt_set, H_OPT_STR)) != NULL)
    {
        print_usage();
        exit_routine(traceroute, EXIT_SUCCESS);
        tmp = NULL;
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
                traceroute->conf.hops = (uint8_t)tmp_hops;
                tmp = NULL;
            }

            else
            {
                dprintf(STDERR_FILENO,
                    "Cannot handle `%s%s' option with arg `%s' (argc %d)\n",
                    opts_args.all & M_OPT ? "-" : "--", tmp->current,
                    tmp->arg, tmp->argc);
                return (set_opts_args_failure(&opts_args));
            }
        }
        else
        {
            dprintf(STDERR_FILENO,
                "Option `%s%s' (argc %d) requires an argument: `%s'\n",
                opts_args.all & M_OPT ? "-" : "--", tmp->current, tmp->argc,
                opts_args.all & M_OPT ? "-m max_ttl" : "--max-hops=max_ttl");
            return (set_opts_args_failure(&opts_args));
        }
    }
    if (opts_args.all & F_OPT || (tmp = get_opt_set_db(&opts_args.opt_set, F_OPT_STR)))
    {
        if (!tmp)
            tmp = get_opt_set_db(&opts_args.opt_set, "f");
        if (tmp->arg)
        {
            if (ft_isnum(tmp->arg))
            {
                int tmp_hop = ft_atoi(tmp->arg);
                if (tmp_hop < MIN_HOPS || tmp_hop > MAX_HOPS_F)
                {
                    dprintf(STDERR_FILENO, "first hop out of range\n");
                    return (set_opts_args_failure(&opts_args));
                }
                traceroute->conf.start_ttl = (uint8_t)tmp_hop;
                tmp = NULL;
            }

            else
            {
                dprintf(STDERR_FILENO,
                    "Cannot handle `%s%s' option with arg `%s' (argc %d)\n",
                    opts_args.all & F_OPT ? "-" : "--", tmp->current,
                    tmp->arg, tmp->argc);
                return (set_opts_args_failure(&opts_args));
            }
        }
        else
        {
            dprintf(STDERR_FILENO,
                "Option `%s%s' (argc %d) requires an argument: `%s'\n",
                opts_args.all & F_OPT ? "-" : "--", tmp->current, tmp->argc,
                opts_args.all & F_OPT ? "-m max_ttl" : "--first=first_ttl");
            return (set_opts_args_failure(&opts_args));
        }
    }

    if (opts_args.all & Q_OPT || (tmp = get_opt_set_db(&opts_args.opt_set, Q_OPT_STR)))
    {
        if (!tmp)
            tmp = get_opt_set_db(&opts_args.opt_set, "q");
        if (tmp->arg)
        {
            if (ft_isnum(tmp->arg))
            {
                int tmp_probes = ft_atoi(tmp->arg);
                if (tmp_probes < MIN_PROBES || tmp_probes > MAX_PROBES)
                {
                    dprintf(STDERR_FILENO, "no more than 10 probes per hop\n");
                    return (set_opts_args_failure(&opts_args));
                }
                traceroute->conf.probes = (uint8_t)tmp_probes;
                tmp = NULL;
            }
            else
            {
                dprintf(STDERR_FILENO,
                    "Cannot handle `%s%s' option with arg `%s' (argc %d)\n",
                    opts_args.all & Q_OPT ? "-" : "--", tmp->current,
                    tmp->arg, tmp->argc + 1);
                return (set_opts_args_failure(&opts_args));

            }
        }
        else
        {
            dprintf(STDERR_FILENO,
                "Option `%s%s' (argc %d) requires an argument: `%s'\n",
                opts_args.all & Q_OPT ? "-" : "--", tmp->current, tmp->argc,
                opts_args.all & Q_OPT ? "-q nqueries" : "--queries=nqueries");
            return (set_opts_args_failure(&opts_args));

        }
    }
    if (!(n_args = ft_lstlen(opts_args.args)))
    {
        dprintf(STDERR_FILENO, "Specify \"host\" missing argument.\n");
        return (set_opts_args_failure(&opts_args));
    }
    if (n_args > 2)
    {
        if ((tmp_args_db = get_arg(&opts_args.args, POSITION(3))) != NULL)
            dprintf(STDERR_FILENO, "Extra arg `%s' (position 3, argc %d)\n",
            tmp_args_db->arg, tmp_args_db->argc);
        return (set_opts_args_failure(&opts_args));
    }
    if (n_args > 1)
    {
        if ((tmp_args_db = get_arg(&opts_args.args, POSITION(2))) != NULL)
        {
            if (ft_isnum(tmp_args_db->arg) != TRUE)
            {
                dprintf(STDERR_FILENO,
                    "Cannot handle \"packetlen\" cmdline arg `%s' on position 2 (argc %d)\n",
                    tmp_args_db->arg,  tmp_args_db->argc);
                return (set_opts_args_failure(&opts_args));
            }
            else
            {
                int tmp_packetlen = ft_atoi(tmp_args_db->arg);
                if (tmp_packetlen < MIN_PACKETLEN)
                    tmp_packetlen = MIN_PACKETLEN;
                if (tmp_packetlen > MAX_PACKETLEN)
                {
                    dprintf(STDERR_FILENO, "too big packetlen %d specified\n", tmp_packetlen);
                    return (set_opts_args_failure(&opts_args));
                }
                traceroute->conf.packetlen = (uint16_t)tmp_packetlen;
            }
        }
        else
            return (set_opts_args_failure(&opts_args));
    }
    tmp_args_db = get_arg(&opts_args.args, POSITION(0));
    if (resolve_target(traceroute, tmp_args_db->arg, tmp_args_db->argc) != FALSE)
        return (set_opts_args_failure(&opts_args));
    traceroute->opts = opts_args.all;
    free_opts_args(&opts_args);
    return (SUCCESS);
}
