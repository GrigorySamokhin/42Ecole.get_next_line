/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmeizo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 16:16:36 by gmeizo            #+#    #+#             */
/*   Updated: 2019/04/26 16:16:48 by gmeizo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int			ft_lst_read(const int fd, char **cur_list)
{
	int		var;
	char	buff[BUFF_SIZE + 1];
	char	*temp;

	while ((var = read(fd, buff, BUFF_SIZE)))
	{
		buff[var] = '\0';
		temp = *cur_list;
		if (!(*cur_list = ft_strjoin(*cur_list, buff)))
			return (-1);
		free(temp);
		if (ft_strchr(*cur_list, '\n'))
			break ;
	}
	return (var);
}

t_list		*ft_lst_check(t_list **main_list, const int fd)
{
	t_list		*temp;

	temp = *main_list;
	while (temp)
	{
		if (temp->content_size == (size_t)fd)
			return (temp);
		temp = temp->next;
	}
	temp = ft_lstnew("", fd);
	ft_lstadd(main_list, temp);
	return (temp);
}

int			ft_gnl_cpy(char **line, char *list)
{
	int			var;

	var = 0;
	while (list[var] != '\n' && list[var] != '\0')
		var++;
	*line = ft_strndup(list, var);
	return (var);
}

void		ft_delete(t_list **main_list, const int fd)
{
	t_list *cur_list;
	t_list *temp_list;

	cur_list = *main_list;
	if ((*main_list == 0) || cur_list->next == NULL)
		return ;
	temp_list = cur_list->next;
	while (temp_list)
	{
		if (temp_list->content_size == (size_t)fd)
		{
			cur_list->next = temp_list->next;
			if (temp_list->content == NULL)
				return ;
			free(&temp_list->content);
			free(temp_list);
			return ;
		}
		cur_list = temp_list;
		temp_list = temp_list->next;
	}
}

int			get_next_line(const int fd, char **line)
{
	static t_list	*main_list;
	t_list			*cur_list;
	char			*temp;
	int				var;
	char			buff[1];

	if (fd < 0 || read(fd, buff, 0) < 0 || !line ||
		(!(cur_list = ft_lst_check(&main_list, fd))))
		return (-1);
	var = ft_lst_read(fd, (char **)&cur_list->content);
	temp = cur_list->content;
	if (!*temp && !var)
		return (0);
	var = ft_gnl_cpy(line, (char *)cur_list->content);
	if (temp[var] != '\0')
	{
		cur_list->content = ft_strdup(&((cur_list->content)[var + 1]));
		free(temp);
	}
	else
	{
		temp[0] = '\0';
		ft_delete(&main_list, fd);
	}
	return (1);
}
