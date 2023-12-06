/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ***REMOVED*** <***REMOVED***@student.***REMOVED***.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 11:42:56 by codespace         #+#    #+#             */
/*   Updated: 2023/12/06 14:31:56 by ***REMOVED***            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	parse_arguments(int argc, char **argv, t_philos *args)
{
	int	index;

	index = 1;
	if (argc < 5 || argc > 6)
		return (false);
	while (argv[index])
	{
		if (!ft_is_positive_numeric(argv[index]))
			return (false);
		index++;
	}
	if (ft_atol(argv[1]) <= 32750)
		args->amount = (unsigned int)ft_atol(argv[1]);
	else
		return (false);
	args->time_to_die = (unsigned long)ft_atol(argv[2]);
	args->time_to_eat = (unsigned long)ft_atol(argv[3]);
	args->time_to_sleep = (unsigned long)ft_atol(argv[4]);
	if (argc == 6)
		args->min_eat_number = (long long)ft_atol(argv[5]);
	else
		args->min_eat_number = -1;
	return (true);
}

size_t	ft_strlen(const char *string)
{
	size_t	counter;

	counter = 0;
	while (string[counter] != '\0')
		counter++;
	return (counter);
}

static int	ft_isdigit(int c)
{
	if ((c >= '0' && c <= '9'))
		return (1);
	else
		return (0);
}

long long	ft_atol(const char *str)
{
	long long	integer;
	int			seen_sign;

	integer = 0;
	seen_sign = 1;
	while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\v'
		|| *str == '\f' || *str == '\r')
		str++;
	if (*str == '-')
	{
		seen_sign *= -1;
		str++;
	}
	else if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		integer = integer * 10 + (*str - '0');
		str++;
	}
	return (integer * (long long)seen_sign);
}

bool	ft_is_positive_numeric(char *string)
{
	char	*string_start;

	string_start = string;
	if (*string == '+')
		string++;
	if (*string == '\0')
		return (false);
	while (ft_isdigit(*string))
		string++;
	if (*string == '\0')
	{
		if (ft_strlen(string_start) > 11 || ft_atol(string_start) > 4294967295)
			return (false);
		else
			return (true);
	}
	else
		return (false);
}
