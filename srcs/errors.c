/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 17:16:39 by arsciand          #+#    #+#             */
/*   Updated: 2021/11/18 14:49:32 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

// void     getaddrinfo_error_handler(t_core *core, int8_t status)
// {
//     (void)core;
//     (void)status;
//     // t_lst    *arg = get_arg(&core->opts_args->args, 1);
//     // switch (status)
//     // {
//     //     case EAI_ADDRFAMILY:
//     //         dprintf(STDERR_FILENO, "%s: Address family for hostname not supported\n", ((t_args_db *)arg->content)->arg);
//     //         break ;
//     //     case EAI_AGAIN:
//     //         dprintf(STDERR_FILENO,
//     //             "%s: Temporary failure in name resolution\n", ((t_args_db *)arg->content)->arg);
//     //         break ;
//     //     case EAI_NODATA:
//     //         dprintf(STDERR_FILENO,
//     //             "%s: No address associated with hostname\n", ((t_args_db *)arg->content)->arg);
//     //         break ;
//     //     case EAI_NONAME:
//     //         dprintf(STDERR_FILENO,
//     //             "%s: Name or service not known\n", ((t_args_db *)arg->content)->arg);
//     //         break ;
//     //     case EAI_SYSTEM:
//     //         dprintf(STDERR_FILENO,
//     //             "%s: System error\n", ((t_args_db *)arg->content)->arg);
//     //         break ;
//     // }
//     // dprintf(STDERR_FILENO, "Cannot handle \"host\" cmdline arg `%s' on position 1 (argc %d)\n",  ((t_args_db *)arg->content)->arg,  ((t_args_db *)arg->content)->argc);
//     exit_routine(core, FAILURE);
// }
