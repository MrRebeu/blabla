/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 02:07:23 by tcaccava          #+#    #+#             */
/*   Updated: 2025/05/07 02:13:21 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

int ft_strlen(char *s)
{
    int i = 0;
    while(s[i])
        i++;
    return (i);
}

char *ft_strdup(char *s)
{
    char *dup;
    int len = ft_strlen(s);
    dup = malloc(sizeof(char) * len);
    if(!dup)
        return NULL;
    int i = 0;
    while(s[i])
    {
        dup[i] = s[i];
        i++;
    }
    dup[i] = '\0';
    return dup;
}