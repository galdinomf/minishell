/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_open_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgaldino <mgaldino@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 22:55:34 by daeidi-h          #+#    #+#             */
/*   Updated: 2022/10/04 16:45:31 by mgaldino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	fork_open_exec( char *cmd, int n_cmd, t_pids_pipes *aux)
{
	t_cmd	*cmd_table;
	int	have_file;

	have_file = 0;
//	signal(SIGINT, SIG_IGN);
	aux->pids[n_cmd] = fork();
	if (aux->pids[n_cmd] < 0)
		error("fork", 0);
	if (aux->pids[n_cmd] == 0)
	{
		signal_for_child();
		cmd_table = make_cmd_table(cmd);
		close_pipes(aux->total_cmd, aux->pipes, n_cmd);
		open_fds(cmd_table->redirections, aux, n_cmd, &have_file);
		if(n_cmd != 0 || have_file == 1 || have_file == 3)
			dup2(aux->pipes[n_cmd][0], STDIN_FILENO);
		if (n_cmd != (aux->total_cmd - 1) || have_file > 1)
			dup2(aux->pipes[(n_cmd + 1)][1], STDOUT_FILENO);
		close(aux->pipes[n_cmd][0]);
		close(aux->pipes[(n_cmd + 1)][1]);
		free_ptr((void **) aux->pipes);
		if (!is_builtin(cmd_table->cmd_and_args))
			exec_cmd(cmd_table->cmd_and_args);
		return (run_builtin(cmd_table->cmd_and_args));
	}
}

static int	len_hash(void)
{
	int		i;
	int		len;
	t_list	*aux;

	i = -1;
	len = 0;
	while (g_data->hash_table[++i])
	{
		aux = (g_data->hash_table[i]);
		while (aux)
		{
			if (((t_item *) aux->content)->key != NULL)
				len++;
			aux = aux->next;
		}
	}
	return (len);
}

static void	join_key_value(t_item *item, char	**join, int *j)
{	
	if (item->key != NULL)
	{
		*join = ft_strjoin(item->key, "=");
		*join = ft_strnjoin(*join, item->value, ft_strlen(item->value));
		*j = *j + 1;
	}
}

static char	**convert_hash_arr(void)
{
	t_list	*aux;
	int		i;
	int		len;
	char	**environ;
	int		j;

	i = -1;
	len = len_hash();
	environ = malloc(sizeof(char *) * (len + 1));
	environ[len] = NULL;
	i = -1;
	j = 0;
	while (g_data->hash_table[++i])
	{
		aux = (g_data->hash_table[i]);
		while (aux)
		{
			join_key_value((t_item *) aux->content, &environ[j], &j);
			aux = aux->next;
		}
	}
	return (environ);
}

void	exec_cmd(char **args)
{
	char		*cmd_path;
	t_list		*temp;
	char		**environ;

	environ = convert_hash_arr();
	temp = find_entry("PATH", g_data->hash_table);
	if (*args[0] == '/' || *args[0] == '.' || *args[0] == '~')
		cmd_path = find_absolute_path(args[0]);
	else
	{
		if (temp == NULL)
		{
			ft_putstr_fd("Error: path not found\n", STDERR_FILENO);
			exit(EXIT_FAILURE);
		}
		cmd_path = get_path(&args[0], ((t_item *)temp->content)->value);
	}
	if (execve(cmd_path, args, environ) == -1)
	{
		write(1, cmd_path, ft_strlen(cmd_path));
		write(1, ": no such file or directory\n", 28);
		free(cmd_path);
		free_split((void **) environ);
		free(environ);
	}
}
