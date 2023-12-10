/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ***REMOVED*** <***REMOVED***@student.***REMOVED***.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 13:25:26 by codespace         #+#    #+#             */
/*   Updated: 2023/12/10 18:47:20 by ***REMOVED***            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	safe_print(const char *string, unsigned int number, unsigned long ts,
					pthread_mutex_t *print_block)
{
	pthread_mutex_lock(print_block);
	printf("%ld %d %s\n", ts, number, string);
	pthread_mutex_unlock(print_block);
}

bool	check_alive(t_philo_data *philo)
{
	if (!philo->alive_mutex || pthread_mutex_lock(philo->alive_mutex))
		return (false);
	if (!(philo->alive))
	{
		pthread_mutex_unlock(philo->alive_mutex);
		return (false);
	}
	else
	{
		pthread_mutex_unlock(philo->alive_mutex);
		return (true);
	}
}

// bool	start_monitor(t_philos	*all_philos)
// {
// 	pthread_t	monitor_thread;

// 	if (pthread_create(&monitor_thread, NULL, monitor, all_philos))
// 	{
// 		kill_threads(all_philos, all_philos->amount);
// 		destroy_mutexes(all_philos, all_philos->amount);
// 		return (false);
// 	}
// 	return (true);
// }

// void better_sleep(unsigned long sleep_ms)
// {
// 	struct timeval	current_time;
// 	struct timeval	time_start;
// 	unsigned long	elapsed_us;

// 	sleep_ms *= 1000;
// 	elapsed_us = 0;
// 	gettimeofday(&time_start, NULL);
// 	while (elapsed_us < sleep_ms)
// 	{
// 		gettimeofday(&current_time, NULL);
// 		elapsed_us = (current_time.tv_sec - time_start.tv_sec)
			// * 1000000L + (current_time.tv_usec - time_start.tv_usec);
// 	}
// }
