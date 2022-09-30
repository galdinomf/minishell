/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   after_fork.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgaldino <mgaldino@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 14:49:28 by daeidi-h          #+#    #+#             */
/*   Updated: 2022/09/30 09:21:26 by mgaldino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	await_all_children(int children_count)
{
	int	i;
	int	wstatus;
//	int	waited_pid;

	i = -1;
	while (++i <= children_count)
//		waited_pid = waitpid(-1, &wstatus, 0);
		waitpid(-1, &wstatus, 0);
	return ;
}

void	after_fork(int n_cmd, int **pipes, pid_t *pid)
{
	close_pipes_main(n_cmd, pipes);
	await_all_children(n_cmd);
	close(pipes[0][1]);
	close(pipes[n_cmd][0]);
	free_ptr((void *) &pid);
	free_ptr((void **) pipes);
}