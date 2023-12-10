/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ***REMOVED*** <***REMOVED***@student.***REMOVED***.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 18:41:13 by ***REMOVED***             #+#    #+#             */
/*   Updated: 2023/12/10 19:09:34 by ***REMOVED***            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static bool	philo_condition(t_philos *phils, unsigned int index)
{
	unsigned long	current_time;
	unsigned long	current_ts;

	current_time = get_time();
	current_ts = read_ts(&(phils->ph_arr[index]));
	if (!(check_alive(&(phils->ph_arr[index])))
		|| ((((current_time - current_ts
						> phils->time_to_die)) && current_ts
				> 0 && current_time > current_ts)))
	{
		return (false);
	}
	return (true);
}

static bool	check_philos(t_philos *phils)
{
	unsigned int	index;

	index = 0;
	while (index < phils->amount)
	{
		if (!(philo_condition(phils, index)))
		{
			if (phils->ph_arr[index].eaten_enough)
				continue ;
			safe_print("died", index + 1, get_time()
				- phils->ph_arr[index].start, phils->ph_arr[index].prt_lck);
			pthread_mutex_lock(phils->ph_arr[index].alive_mutex);
			phils->ph_arr[index].first_dead = true;
			phils->ph_arr[index].alive = false;
			pthread_mutex_unlock(phils->ph_arr[index].alive_mutex);
			return (false);
		}
		index++;
	}
	return (true);
}

static void	kill_all_philosophers(t_philos *phils)
{
	unsigned int	index;

	index = 0;
	while (index < phils->amount)
	{
		pthread_mutex_lock(phils->ph_arr[index].alive_mutex);
		phils->ph_arr[index].alive = false;
		pthread_mutex_unlock(phils->ph_arr[index].alive_mutex);
		index++;
	}
}

bool	monitor(t_philos *phils)
{
	unsigned int	index;
	unsigned int	eaten;

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
		if (!check_philos(phils))
			break ;
	}
	kill_all_philosophers(phils);
	return (true);
}
