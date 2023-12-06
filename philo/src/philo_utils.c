/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <***REMOVED***@student.***REMOVED***.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 13:25:26 by codespace         #+#    #+#             */
/*   Updated: 2023/12/06 12:11:29 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
// 		elapsed_us = (current_time.tv_sec - time_start.tv_sec) * 1000000L + (current_time.tv_usec - time_start.tv_usec);
// 	}
// }

unsigned long	get_time()
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (((unsigned long)time.tv_sec) * 1000)
			+ (((unsigned long)time.tv_usec) / 1000);
}

void	safe_print(const char *string, unsigned int number, unsigned long timestamp,
					pthread_mutex_t *print_block)
{
	pthread_mutex_lock(print_block);
	printf("%ld %d %s\n", timestamp, number, string);
	pthread_mutex_unlock(print_block);
}

bool	monitor(philos_t *all_philos)
{
	unsigned int	index;
	unsigned int	eaten;
	unsigned long	current_time;
	bool			all_alive;

	all_alive = true;
	// eaten = 0;
	usleep((all_philos->time_to_die * 1000) / 2);
	while (all_alive)
	{
		index = 0;
		eaten = 0;
		while (all_philos->philo_data_array[index].eaten_enough && all_philos->amount > index)
		{
			index++;
			eaten++;
		}
		if (eaten == all_philos->amount)
			break;
		// else
		// 	printf("eaten amount: %d\n", eaten);
		index = 0;
		current_time = get_time();
		while (index < all_philos->amount)
		{
			if (!(all_philos->philo_data_array[index].alive) || ((((current_time - (all_philos->philo_data_array[index]).last_meal_ts) > all_philos->time_to_die)) && (all_philos->philo_data_array[index].last_meal_ts > 0 && current_time > all_philos->philo_data_array[index].last_meal_ts)))
				{
					if (all_philos->philo_data_array[index].eaten_enough)
						continue;
					all_alive = false;
					safe_print("died", index + 1, get_time() - all_philos->philo_data_array[index].start_time, all_philos->philo_data_array[index].print_mutex);
					all_philos->philo_data_array[index].first_dead = true;
					all_philos->philo_data_array[index].alive = false;
					break ;
				}
			index++;
		}
	}
	index = 0;
	while (index < all_philos->amount)
		all_philos->philo_data_array[index++].alive = false;
	return (true);
}

// bool	start_monitor(philos_t	*all_philos)
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
