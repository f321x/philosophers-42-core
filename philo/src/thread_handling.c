/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ***REMOVED*** <***REMOVED***@student.***REMOVED***.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 09:11:08 by codespace         #+#    #+#             */
/*   Updated: 2023/12/06 14:31:56 by ***REMOVED***            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	destroy_mutexes(t_philos *args, unsigned int amount)
{
	unsigned int	index;

	index = 0;
	while (index < amount)
	{
		pthread_mutex_destroy(&(args->fork_array[index]));
		index++;
	}
	pthread_mutex_destroy(&(args->printing_mutex));
	free(args->fork_array);
}

void	kill_threads(t_philos *args, unsigned int amount)
{
	unsigned int	index;

	index = 0;
	while (index < amount)
	{
		pthread_detach(args->thread_array[index]);
		index++;
	}
	free(args->ph_arr);
	free(args->thread_array);
}

bool	allocate_threads(t_philos *args)
{
	unsigned int	index;

	index = 0;
	args->thread_array = malloc(sizeof(pthread_t) * args->amount);
	if (!args->thread_array)
		return (false);
	args->fork_array = malloc(sizeof(pthread_mutex_t) * args->amount);
	if (!args->fork_array)
	{
		free(args->thread_array);
		return (false);
	}
	while (index < args->amount)
	{
		if (pthread_mutex_init(&(args->fork_array[index]), NULL))
		{
			free(args->thread_array);
			destroy_mutexes(args, index);
			return (false);
		}
		index++;
	}
	return (true);
}

static void	create_philos(t_philos *args, unsigned int index)
{
	t_philo_data	current_philo;

	current_philo.time_to_die = args->time_to_die;
	current_philo.time_to_eat = args->time_to_eat;
	current_philo.time_to_sleep = args->time_to_sleep;
	current_philo.amount = args->amount;
	current_philo.min_eat_number = args->min_eat_number;
	current_philo.number = index + 1;
	current_philo.start = args->start;
	current_philo.fork_array = args->fork_array;
	current_philo.alive = true;
	current_philo.eaten_enough = false;
	current_philo.first_dead = false;
	current_philo.prt_lck = &(args->printing_mutex);
	args->ph_arr[index] = current_philo;
}

bool	spawn_philosophers(t_philos *args)
{
	unsigned int	index;

	index = 0;
	args->ph_arr = malloc(sizeof(t_philo_data) * args->amount);
	if (!allocate_threads(args) || !args->ph_arr)
	{
		if (args->ph_arr)
			free(args->ph_arr);
		return (false);
	}
	pthread_mutex_init(&(args->printing_mutex), NULL);
	args->start = get_time();
	while (index < args->amount)
	{
		create_philos(args, index);
		if (pthread_create(&(args->thread_array[index]), NULL, philo,
				&(args->ph_arr[index])))
		{
			kill_threads(args, index);
			destroy_mutexes(args, args->amount);
			return (false);
		}
		index++;
	}
	return (monitor(args));
}
