#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum e_type{
	PIPE,
	COMMA,
	END
}	t_type;

void	cd(char *path, int ac)
{
	if (ac > 2)
	{
		write(2, "Error too many arguments cd", 24);
		return ;
	}
	else if (ac == 2)
		if (chdir(path) != 0)
			write(2, "Error crash chdir", 24);
}

//function next_pipe

//function next_comma

//function is pipe

bool	is_pipe(char *str)
{
	if (str && str[0] == '|' && !str[1])
		return (true);
	return (false);
}

bool	is_comma(char *str)
{
	if (str && str[0] == ';' && !str[1])
		return (true);
	return (false);
}
//function is comma

t_type	till_next(int *i, char **av, char *buff[10000], int ac)
{
	int	j;

	j = 0;
	while (*i < ac)
	{
		printf("%s %i", av[*i], *i);
		if (is_pipe(av[*i]))
		{
			buff[j] = 0;
			(*i)++;
			return (PIPE);
		}
		else if (is_comma(av[*i]))
		{
			buff[j] = 0;
			(*i)++;
			return (COMMA);
		}
		buff[j] = av[*i];
		j++;
		(*i)++;
	}
	buff[j] = 0;
	return (END);
}

void	reset_buff(char *buff[10000])
{
	int	i;

	i = 0;
	while (i < 10000)
	{
		buff[i] = 0;
		i++;
	}
}

void	reset_pid(pid_t pidd[10000])
{
	int	i;

	i = 0;
	while (i < 10000)
	{
		pidd[i] = 0;
		i++;
	}
}

void	wait_process(pid_t pidd[10000])
{
	int	i;

	i = 0;
	while (pidd[i] != 0)
	{
		write(2, "uay\n", 4);
		waitpid(pidd[i], NULL, 0);
		i++;
	}
}

pid_t	exec_pipe(char *buff[10000], char **env, int pipe_fd[2])
{
	pid_t	ret;

	ret = fork();
	if (ret == 0)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		close(pipe_fd[0]);
		execve(buff[0], buff, env);
		exit(0);
		return (ret);
	}
	else
	{
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (ret);
	}
	exit(0);
}

pid_t	exec(char *buff[10000], char **env)
{
	pid_t	ret;

	ret = fork();
	if (ret == 0)
	{
		if (execve(buff[0], buff, env) != 0)
			write(2, "EROOOR", 6);
		exit(0);
		return (ret);
	}
	else
		return (ret);
}

int	main(int ac, char **av, char **env)
{
	int		i;
	char	*buff[10000];
	pid_t	pidd[10000];
	int		pipe_fd[2];
	int		stdou;
	int		j;

	i = 1;
	j = 0;
	t_type	next;
	int		stdi;
	stdou = dup(STDOUT_FILENO);
	stdi = dup(STDIN_FILENO);
	while (i < ac)
	{
		next = till_next(&i, av, buff, ac);
		if (next == END)
		{
			dup2(STDIN_FILENO, stdi);
			pidd[j] = exec(buff, env);
			j++;
			wait_process(pidd);
			printf("ONE :%i", next);
			break ;
		}
		else if (next == PIPE)
		{
			dup2(STDIN_FILENO, stdi);
			pipe(pipe_fd);
			pidd[j] = exec_pipe(buff, env, pipe_fd);
			j++;
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			close(stdi);
			close(stdou);
		}
		else if (next == COMMA)
		{
			dup2(STDIN_FILENO, stdi);
			exec(buff, env);
			pidd[j] = exec(buff, env);
			j++;
			wait_process(pidd);
			reset_pid(pidd);
			j = 0;
		}
		reset_buff(buff);
	}
	close(stdou);
}

//function exec()

//function dup

//function reset fd

//faire gaffe au free
