/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 14:57:08 by arsciand          #+#    #+#             */
/*   Updated: 2021/06/21 11:25:31 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

uint8_t     init_core(t_core *core)
{
    struct  addrinfo    hints;
    struct  addrinfo    *res                        = NULL;
    char                buff_ipv4[INET_ADDRSTRLEN];
    int8_t              status                      = 0;

    ft_memset(&hints, 0, sizeof(struct addrinfo));
    ft_memset(&buff_ipv4, 0, INET_ADDRSTRLEN);

    /* Preparing getaddrinfo struct */
    hints.ai_flags      = AI_V4MAPPED | AI_ADDRCONFIG;
    hints.ai_family     = AF_INET;
    hints.ai_protocol   = IPPROTO_UDP;

    if ((status = (int8_t)getaddrinfo(((t_args_db *)get_arg(&core->opts_args->args, 1)->content)->arg, NULL, &hints, &res)) != SUCCESS)
        getaddrinfo_error_handler(core, status);

    /**/
    #pragma clang diagnostic ignored "-Wcast-align"
    /**/

    /* Filling up ipv4 stringg addresse */
    if (!(inet_ntop(res->ai_family,
        &((struct sockaddr_in *)res->ai_addr)->sin_addr, buff_ipv4,
        sizeof(buff_ipv4))))
        exit_routine(core, FAILURE);

    if (!(core->target = ft_strdup(buff_ipv4)))
        exit_routine(core, FAILURE);

    /* Setting up sockaddr storage structure */
	((struct sockaddr_in *)&core->target_addr)->sin_addr.s_addr = ((struct sockaddr_in*)res->ai_addr)->sin_addr.s_addr;
    ((struct sockaddr_in *)&core->target_addr)->sin_port        = htons(core->dst_port);
    ((struct sockaddr_in *)&core->target_addr)->sin_family      = res->ai_family;

    /* Cleaning */
    for (struct addrinfo *tmp = NULL; res; res = tmp)
    {
        tmp = res->ai_next;
        free(res);
    }

    return (SUCCESS);
}
