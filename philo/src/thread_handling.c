/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ***REMOVED*** <***REMOVED***@student.***REMOVED***.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 09:11:08 by codespace         #+#    #+#             */
/*   Updated: 2023/12/10 19:05:13 by ***REMOVED***            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

static bool	create_philos(t_philos *args, unsigned int index)
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
	if (!alloc_monitoring_mutexes(&current_philo))
		return (false);
	args->ph_arr[index] = current_philo;
	return (true);
}

static bool	init_arrays(t_philos *args)
{
	args->ph_arr = malloc(sizeof(t_philo_data) * args->amount);
	if (!allocate_threads(args) || !args->ph_arr)
	{
		if (args->ph_arr)
			free(args->ph_arr);
		return (false);
	}
	if (pthread_mutex_init(&(args->printing_mutex), NULL))
	{
		destroy_mutexes(args, args->amount);
		free(args->ph_arr);
		free(args->thread_array);
		return (false);
	}
	return (true);
}

bool	spawn_philosophers(t_philos *args)
{
	unsigned int	index;

	if (!init_arrays(args))
		return (false);
	index = 0;
	args->start = get_time();
	while (index < args->amount)
	{
		if (!create_philos(args, index)
			|| pthread_create(&(args->thread_array[index]), NULL, philo,
				&(args->ph_arr[index])))
		{
			set_philos_dead(args, index);
			usleep(100);
			kill_threads(args, index);
			destroy_mutexes(args, args->amount);
			return (false);
		}
		index++;
	}
	return (monitor(args));
}
