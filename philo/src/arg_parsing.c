/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ***REMOVED*** <***REMOVED***@student.***REMOVED***.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 11:42:56 by codespace         #+#    #+#             */
/*   Updated: 2023/12/10 20:26:04 by ***REMOVED***            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Parses the command line arguments and initializes the t_philos structure.
// @param argc The number of command line arguments.
// @param argv An array of strings containing the command line arguments.
// @param args A pointer to the t_philos structure to be initialized.
// @return true if the arguments are successfully parsed
// and the structure is initialized, false otherwise.

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

/**
 * Calculates the length of a string.
 *
 * @param string The string to calculate the length of.
 * @return The length of the string.
 */
size_t	ft_strlen(const char *string)
{
	size_t	counter;

	counter = 0;
	while (string[counter] != '\0')
		counter++;
	return (counter);
}

/**
 * @brief Checks if a character is a digit.
 *
 * This function checks if the given character is a digit (0-9).
 *
 * @param c The character to be checked.
 * @return 1 if the character is a digit, 0 otherwise.
 */
static int	ft_isdigit(int c)
{
	if ((c >= '0' && c <= '9'))
		return (1);
	else
		return (0);
}

// @brief Converts a string to a long long integer.
//
// This function parses the input string and converts it to a
// long long integer.
// It skips leading whitespace characters and
// handles both positive and negative numbers.
//
// @param str The input string to be converted.
// @return The converted long long integer value.

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

// Checks if a string represents a positive numeric value.
//
// @param string The string to be checked.
// @return true if the string represents a
// positive numeric value, false otherwise.
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
