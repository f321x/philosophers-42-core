/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ***REMOVED*** <***REMOVED***@student.***REMOVED***.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 18:49:20 by ***REMOVED***             #+#    #+#             */
/*   Updated: 2023/12/10 20:20:17 by ***REMOVED***            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * @brief Allocates and initializes the monitoring mutexes for a philosopher.
 *
 * This function allocates memory for the `alive_mutex` and `ts_mutex` of the `current_philo` structure,
 * and initializes them using `pthread_mutex_init()`. If any allocation or initialization fails,
 * the function frees the allocated memory and returns `false`.
 *
 * @param current_philo The pointer to the `t_philo_data` structure representing the philosopher.
 * @return `true` if the mutexes are successfully allocated and initialized, `false` otherwise.
 */
bool	alloc_monitoring_mutexes(t_philo_data *current_philo)
{
	current_philo->alive_mutex = malloc(sizeof(pthread_mutex_t));
	if (!current_philo->alive_mutex)
		return (false);
	if (pthread_mutex_init(current_philo->alive_mutex, NULL))
	{
		free(current_philo->alive_mutex);
		return (false);
	}
	current_philo->ts_mutex = malloc(sizeof(pthread_mutex_t));
	if (!current_philo->ts_mutex)
	{
		free(current_philo->alive_mutex);
		return (false);
	}
	if (pthread_mutex_init(current_philo->ts_mutex, NULL))
	{
		free(current_philo->alive_mutex);
		free(current_philo->ts_mutex);
		return (false);
	}
	return (true);
}

/**
 * Sets the passed amount of philosophers' alive status to false.
 *
 * @param phils   The philosophers struct.
 * @param amount  The number of philosophers to set dead.
 */
void	set_philos_dead(t_philos *phils, unsigned int amount)
{
	unsigned int	index;

	index = 0;
	while (index < amount)
	{
		pthread_mutex_lock(phils->ph_arr[index].alive_mutex);
		phils->ph_arr[index].alive = false;
		pthread_mutex_unlock(phils->ph_arr[index++].alive_mutex);
	}
}

/**
 * @brief Destroys the mutexes used in the philosophers program.
 *
 * This function destroys the mutexes used for synchronization in the philosophers program.
 * It takes a pointer to the t_philos struct and the number of philosophers as parameters.
 * It iterates over the mutexes and destroys them using pthread_mutex_destroy().
 * It also frees the memory allocated for the alive_mutex and ts_mutex of each philosopher.
 * Finally, it destroys the printing_mutex and frees the memory allocated for the fork_array.
 *
 * @param args A pointer to the t_philos struct.
 * @param amount The number of philosophers.
 */
void	destroy_mutexes(t_philos *args, unsigned int amount)
{
	unsigned int	index;

	index = 0;
	while (index < amount)
	{
		pthread_mutex_destroy(&(args->fork_array[index]));
		pthread_mutex_destroy(args->ph_arr[index].alive_mutex);
		pthread_mutex_destroy(args->ph_arr[index].ts_mutex);
		free(args->ph_arr[index].alive_mutex);
		free(args->ph_arr[index].ts_mutex);
		index++;
	}
	pthread_mutex_destroy(&(args->printing_mutex));
	free(args->fork_array);
}

/**
 * @brief Kills the threads and frees the allocated memory.
 *
 * This function detaches and frees the threads in the thread_array
 * and frees the memory allocated for the ph_arr and thread_array.
 *
 * @param args The pointer to the t_philos struct.
 * @param amount The number of threads to kill.
 */
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
