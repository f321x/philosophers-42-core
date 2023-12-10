/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ***REMOVED*** <***REMOVED***@student.***REMOVED***.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 18:49:20 by ***REMOVED***             #+#    #+#             */
/*   Updated: 2023/12/10 19:02:04 by ***REMOVED***            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
