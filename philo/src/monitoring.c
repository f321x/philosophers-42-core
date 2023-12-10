/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ***REMOVED*** <***REMOVED***@student.***REMOVED***.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 18:41:13 by ***REMOVED***             #+#    #+#             */
/*   Updated: 2023/12/10 20:28:22 by ***REMOVED***            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Checks the condition for a philosopher's liveliness.
//
// @param phils The struct containing the philosophers' information.
// @param index The index of the philosopher to check.
// @return true if the philosopher is alive and
// meets the condition, false otherwise.
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

/**
 * @brief Checks the condition of each philosopher in the array.
 *
 * This function iterates through the philosophers array and checks the condition of each philosopher.
 * If a philosopher's condition is not met, it prints a death message, updates the philosopher's state,
 * and returns false. Otherwise, it continues to the next philosopher.
 *
 * @param phils The philosophers struct containing the array of philosophers.
 * @return true if all philosophers' conditions are met, false otherwise.
 */
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

/**
 * @brief Kills all philosophers in the given array.
 *
 * This function sets the 'alive' flag of each philosopher in the array to false,
 * indicating that they should stop their execution.
 *
 * @param phils The structure containing the array of philosophers.
 */
static void kill_all_philosophers(t_philos *phils)
{
	unsigned int index;

	index = 0;
	while (index < phils->amount)
	{
		pthread_mutex_lock(phils->ph_arr[index].alive_mutex);
		phils->ph_arr[index].alive = false;
		pthread_mutex_unlock(phils->ph_arr[index].alive_mutex);
		index++;
	}
}

/**
 * Monitors the state of the philosophers.
 *
 * This function checks the state of the philosophers and returns true if all
 * philosophers have finished eating, or if any philosopher has died.
 * It will always kill every philosopher before returning.
 * @param phils The structure containing the information about the philosophers.
 * @return True if all philosophers have finished eating or if any philosopher has died, false otherwise.
 */
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
