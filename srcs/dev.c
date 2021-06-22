/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dev.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsciand <arsciand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 15:10:24 by arsciand          #+#    #+#             */
/*   Updated: 2021/06/22 15:48:14 by arsciand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void    debug_core(t_core *core)
{
    dprintf(STDERR_FILENO, "\n\n[DEBUG] -> target |%s|\n", core->target);
    dprintf(STDERR_FILENO, "[DEBUG] -> packetlen |%d|\n", core->packetlen);
    dprintf(STDERR_FILENO, "[DEBUG] -> hops |%d|\n", core->hops);
    dprintf(STDERR_FILENO, "[DEBUG] -> probes |%d|\n", core->probes);
}

void    print_bytes(int bytes, void *msg)
{
    for (int i = 0; i < bytes; i++)
    {
        if (!(i & 15))
            printf("\n%04X:  ", i);
        printf("%02X ", ((unsigned char*)msg)[i]);
    }
    printf("\n");
}
