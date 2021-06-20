/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 17:16:39 by arsciand          #+#    #+#             */
/*   Updated: 2021/06/20 14:53:17 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

// void     getaddrinfo_error_handler(int8_t status)
// {
//     switch (status)
//     {
//         case EAI_ADDRFAMILY:
//             fprintf(stderr,
//                 "ft_ping: %s: Address family for hostname not supported\n",
//                 g_core->opts_args->args[0]);
//             break ;
//         case EAI_AGAIN:
//             fprintf(stderr,
//                 "ft_ping: %s: Temporary failure in name resolution\n",
//                 g_core->opts_args->args[0]);
//             break ;
//         case EAI_NODATA:
//             fprintf(stderr,
//                 "ft_ping: %s: No address associated with hostname\n",
//                 g_core->opts_args->args[0]);
//             break ;
//         case EAI_NONAME:
//             fprintf(stderr,
//                 "ft_ping: %s: Name or service not known\n",
//                 g_core->opts_args->args[0]);
//             break ;
//         case EAI_SYSTEM:
//             fprintf(stderr,
//                 "ft_ping: %s: System error\n",
//                 g_core->opts_args->args[0]);
//             break ;
//     }
//     exit_routine(FAILURE);
// }
