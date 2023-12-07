/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ***REMOVED*** <***REMOVED***@student.***REMOVED***.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 13:25:26 by codespace         #+#    #+#             */
/*   Updated: 2023/12/06 15:09:34 by ***REMOVED***            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

unsigned long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((((unsigned long)time.tv_sec) * 1000)
		+ (((unsigned long)time.tv_usec) / 1000));
}

void	safe_print(const char *string, unsigned int number, unsigned long ts,
					pthread_mutex_t *print_block)
{
	pthread_mutex_lock(print_block);
	printf("%ld %d %s\n", ts, number, string);
	pthread_mutex_unlock(print_block);
}

static bool	check_philos(t_philos *phils)
{
	unsigned long	current_time;
	unsigned int	index;

	current_time = get_time();
	index = 0;
	while (index < phils->amount)
	{
		if (!(phils->ph_arr[index].alive)
			|| ((((current_time - (phils->ph_arr[index]).lm_ts)
						> phils->time_to_die)) && (phils->ph_arr[index].lm_ts
					> 0 && current_time > phils->ph_arr[index].lm_ts)))
		{
			if (phils->ph_arr[index].eaten_enough)
				continue ;
			safe_print("died", index + 1, get_time()
				- phils->ph_arr[index].start, phils->ph_arr[index].prt_lck);
			phils->ph_arr[index].first_dead = true;
			phils->ph_arr[index].alive = false;
			return (false);
		}
		index++;
	}
	return (true);
}

bool	monitor(t_philos *phils)
{
	unsigned int	index;
	unsigned int	eaten;
	unsigned long	current_time;

	usleep((phils->time_to_die * 1000));
	while (1)
	{
		index = 0;
		eaten = 0;
		while (phils->ph_arr[index].eaten_enough
			&& phils->amount > index)
		{
			index++;
			eaten++;
		}
		if (eaten == phils->amount)
			break ;
		current_time = get_time();
		if (!check_philos(phils))
			break ;
	}
	index = 0;
	while (index < phils->amount)
		phils->ph_arr[index++].alive = false;
	return (true);
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
