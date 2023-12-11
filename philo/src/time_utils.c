/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ***REMOVED*** <***REMOVED***@student.***REMOVED***.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 18:29:59 by ***REMOVED***             #+#    #+#             */
/*   Updated: 2023/12/11 13:10:28 by ***REMOVED***            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * @brief Get the current time in milliseconds.
 *
 * This function retrieves the current time using the gettimeofday() function
 * and returns it in milliseconds.
 *
 * @return The current time in milliseconds.
 */
unsigned long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((((unsigned long)time.tv_sec) * 1000)
		+ (((unsigned long)time.tv_usec) / 1000));
}

/**
 * @brief Reads the timestamp from the philosopher's data structure.
 *
 * This function locks the timestamp mutex, reads the timestamp value,
 * and then unlocks the mutex before returning the timestamp.
 *
 * @param philo The philosopher's data structure.
 * @return The timestamp value.
 */
unsigned long	read_ts(t_philo_data *philo)
{
	unsigned long	timestamp;

	pthread_mutex_lock(philo->ts_mutex);
	timestamp = philo->lm_ts;
	pthread_mutex_unlock(philo->ts_mutex);
	return (timestamp);
}

// @brief Writes the current timestamp to the `lm_ts`
// field of the `philo` structure.
//
// This function locks the `ts_mutex` mutex, gets the
// current time using `get_time()`,
// and stores it in the `lm_ts` field of the `philo`
// structure. After writing the timestamp,
// it unlocks the `ts_mutex` mutex.
//
// @param philo A pointer to the `t_philo_data` structure.
void	write_ts(t_philo_data *philo)
{
	pthread_mutex_lock(philo->ts_mutex);
	philo->lm_ts = get_time();
	pthread_mutex_unlock(philo->ts_mutex);
}

void	better_sleep(unsigned long sleep_ms)
{
	struct timeval	current_time;
	struct timeval	time_start;
	unsigned long	elapsed_us;

	sleep_ms *= 1000;
	elapsed_us = 0;
	gettimeofday(&time_start, NULL);
	while (elapsed_us < sleep_ms)
	{
		gettimeofday(&current_time, NULL);
		elapsed_us = (current_time.tv_sec - time_start.tv_sec)
			* 1000000L + (current_time.tv_usec - time_start.tv_usec);
	}
}
