/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ***REMOVED*** <***REMOVED***@student.***REMOVED***.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 18:29:59 by ***REMOVED***             #+#    #+#             */
/*   Updated: 2023/12/10 19:08:04 by ***REMOVED***            ###   ########.fr       */
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

unsigned long	read_ts(t_philo_data *philo)
{
	unsigned long	timestamp;

	pthread_mutex_lock(philo->ts_mutex);
	timestamp = philo->lm_ts;
	pthread_mutex_unlock(philo->ts_mutex);
	return (timestamp);
}

void	write_ts(t_philo_data *philo)
{
	pthread_mutex_lock(philo->ts_mutex);
	philo->lm_ts = get_time();
	pthread_mutex_unlock(philo->ts_mutex);
}
