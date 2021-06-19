/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/26 15:25:59 by arsciand          #+#    #+#             */
/*   Updated: 2021/06/19 17:29:51 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

# pragma clang diagnostic ignored "-Wreserved-id-macro"
# define _GNU_SOURCE

#include "libft.h"

/* getaddrinfo */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/* errors */
# include <netdb.h>

# define FAILURE                2
# define SUCCESS                0

typedef struct                  s_core
{
    t_opts_args                 *opts_args;
    char                        *target;
}                               t_core;

void     getaddrinfo_error_handler(int8_t status);

#endif
