/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ***REMOVED*** <***REMOVED***@student.***REMOVED***.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 09:11:08 by codespace         #+#    #+#             */
/*   Updated: 2023/12/10 20:18:04 by ***REMOVED***            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * @brief Allocates threads and mutexes for the philosophers simulation.
 *
 * This function allocates memory for an array of pthread_t objects to represent the threads
 * and an array of pthread_mutex_t objects to represent the forks. It also initializes the mutexes.
 *
 * @param args The pointer to the t_philos struct containing the simulation arguments.
 * @return true if the allocation and initialization is successful, false otherwise.
 */
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

/**
 * Creates a philosopher with the given arguments and initializes its data.
 * Then it stores the philosopher in the array of philosophers in the allocated
 * heap.
 * @param args The arguments for creating the philosopher.
 * @param index The index of the philosopher in the array.
 * @return Returns true if the philosopher is successfully created, false otherwise.
 */
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

/**
 * @brief Initializes arrays and mutexes for the philosophers simulation.
 *
 * This function initializes the array of philosopher data and allocates memory for it.
 * It also calls the allocate_threads function to create the threads for each philosopher.
 * Additionally, it initializes a mutex for printing purposes.
 *
 * @param args A pointer to the t_philos struct containing the simulation data.
 * @return true if the initialization is successful, false otherwise.
 */
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

/**
 * Spawns philosophers (allocates & initializes) and starts their threads.
 *
 * @param args The pointer to the t_philos struct containing the necessary arguments.
 * @return Returns true if the philosophers were successfully spawned and monitored, false otherwise.
 */
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
