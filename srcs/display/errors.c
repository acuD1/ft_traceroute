/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 14:37:35 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/19 15:58:26 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void    print_unallowed_opt(t_opts_args *opts_args)
{
    if (opts_args->invalid)
        dprintf(STDERR_FILENO, "Bad option `--%s' (argc %d)\n",
            opts_args->invalid, opts_args->argc);
    else
        dprintf(STDERR_FILENO, "Bad option `-%c' (argc %d)\n",
            (char)(opts_args->all % 128), opts_args->argc);
}

void    getnameinfo_error_handler(t_traceroute *traceroute, int status)
{
    char    *error  = NULL;

    switch (status)
    {
        case EAI_AGAIN:
            error =
                "The name could not be resolved at this time. Try again later";
            break ;
        case EAI_FAIL:
            error = "A nonrecoverable error occurred.";
            break ;
        case EAI_MEMORY:
            error = "Out of memory.";
            break ;
        case EAI_SYSTEM:
            error = "System error";
            break ;
        default:
            error = "Unknown error";
            break;
    }
    dprintf(STDERR_FILENO, "ft_traceroute: getnameinfo(): %s\n", error);
    exit_routine(traceroute, FAILURE);
}

void    getaddrinfo_error_handler(char *target, int argc, int status)
{
    switch (status)
    {
        case EAI_ADDRFAMILY:
            dprintf(STDERR_FILENO,
                "%s: Address family for hostname not supported\n", target);
            break ;
        case EAI_AGAIN:
            dprintf(STDERR_FILENO,
                "%s: Temporary failure in name resolution\n", target);
            break ;
        case EAI_NODATA:
            dprintf(STDERR_FILENO,
                "%s: No address associated with hostname\n", target);
            break ;
        case EAI_NONAME:
            dprintf(STDERR_FILENO,
                "%s: Name or service not known\n", target);
            break ;
        case EAI_SYSTEM:
            dprintf(STDERR_FILENO,
                "%s: System error\n", target);
            break ;
    }
    dprintf(STDERR_FILENO,
        "Cannot handle \"host\" cmdline arg `%s' on position 1 (argc %d)\n", target, argc);
}
