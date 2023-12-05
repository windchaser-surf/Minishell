/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluari <rluari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/12 11:53:02 by vtouffet          #+#    #+#             */
/*   Updated: 2023/11/28 15:50:42 by rluari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	get_base_length(char *base)
{
	int	base_length;
	int	j;

	base_length = 0;
	while (base[base_length])
	{
		if (base[base_length] == '-' || base[base_length] == '+')
			return (0);
		j = base_length + 1;
		while (base[j])
		{
			if (base[base_length] == base[j])
				return (0);
			++j;
		}
		++base_length;
	}
	if (base_length < 2)
		return (0);
	return (base_length);
}

int	check_errors(char *str, char *base)
{
	int	i;
	int	j;
	int	start;

	start = 0;
	while (str[start] != '\0' && (str[start] == ' ' || str[start] == '\t'
			|| str[start] == '\r' || str[start] == '\n' || str[start] == '\v'
			|| str[start] == '\f'))
		start++;
	i = start;
	while (str[i])
	{
		j = 0;
		while (base[j] && (str[i] != base[j]
				|| (str[i] == '-' || str[i] == '+')))
			++j;
		if (str[i] != base[j] && str[i] != '-' && str[i] != '+')
			return (0);
		i++;
	}
	if (i == 0)
		return (0);
	return (1);
}

int	get_nb(char c, char *base)
{
	int	i;

	i = 0;
	while (base[i] && base[i] != c)
		i++;
	return (i);
}

int	ft_atoi_base(char *s, char *base)
{
	int	j;
	int	i;
	int	res;
	int	negative;
	int	base_length;

	if (!(get_base_length(base)) || !check_errors(s, base))
		return (0);
	j = 0;
	base_length = get_base_length(base);
	while (s[j] != '\0' && (s[j] == ' ' || s[j] == '\t' || s[j] == '\r'
			|| s[j] == '\n' || s[j] == '\v' || s[j] == '\f' ))
		j++;
	i = j - 1;
	res = 0;
	negative = 1;
	while (s[++i] && (((s[i] == '-' || s[i] == '+') && i == j)
			|| (s[i] != '-' && s[i] != '+')))
	{
		if (s[i] == '-')
			negative = -1;
		else if (s[i] != '+')
			res = (res * base_length) + (get_nb(s[i], base));
	}
	return (res * negative);
}
