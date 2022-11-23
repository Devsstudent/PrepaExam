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
	{
		printf("%s %i", path, ac);
		if (chdir(path) != 0)
			write(2, "Error crash chdir", 24);
	}
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
	//	printf("%s %i", av[*i], *i);
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
		waitpid(pidd[i], NULL, 0);
		i++;
	}
}

int	nb_arg(char *buff[10000])
{
	int	i;

	i = 0;
	while (buff[i])
		i++;
	return (i);
}

bool	check_cd(char *buff[10000])
{
	if (buff[0] && buff[0][0] == 'c' && buff[0][1] == 'd' && !buff[0][2])
		return (true);
	return (false);
}

pid_t	exec_pipe(char *buff[10000], char **env, int pipe_fd[2], int *tmp_fd)
{
	pid_t	ret;

	ret = fork();
	if (ret == 0)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		close(pipe_fd[0]);
		if (check_cd(buff))
		{
			cd(buff[1], nb_arg(buff));
			exit(0);
		}
		execve(buff[0], buff, env);
		exit(0);
		return (ret);
	}
	else
	{
		*tmp_fd = pipe_fd[0];
		close(pipe_fd[1]);
		return (ret);
	}
	exit(0);
}

pid_t	exec(char *buff[10000], char **env, bool piped, int tmp_fd)
{
	pid_t	ret;

	if (check_cd(buff) && !piped)
	{
		cd(buff[1], nb_arg(buff));
		return (0);
	}
	ret = fork();
	if (ret == 0)
	{
		if (piped)
		{
			dup2(tmp_fd, STDIN_FILENO);
			close(tmp_fd);
		}
		if (check_cd(buff))
		{
			cd(buff[1], nb_arg(buff));
			exit(0);
		}
		if (execve(buff[0], buff, env) != 0)
			write(2, "EROOOR", 6);
		exit(0);
		return (ret);
	}
	else
	{
		if (piped)
			close(tmp_fd);
		return (ret);
	}
}

int	main(int ac, char **av, char **env)
{
	int		i;
	char	*buff[10000];
	pid_t	pidd[10000];
	int		pipe_fd[2];
	int		tmp_fd;
	int		stdou;
	int		j;
	bool	piped;
	t_type	next;
	int		stdi;
	char	arr[1000];

	i = 1;
	j = 0;
	piped = false;
	stdou = dup(STDOUT_FILENO);
	stdi = dup(STDIN_FILENO);
	getcwd(arr, 1000);
	printf("%s\n", arr);
	chdir("test");
	getcwd(arr, 1000);
	printf("%s\n", arr);
//	while (i < ac)
//	{
//		next = till_next(&i, av, buff, ac);
//		if (next == END)
//		{
//			pidd[j] = exec(buff, env, piped, tmp_fd);
//			j++;
//			wait_process(pidd);
//			//printf("ONE :%i", next);
//			break ;
//		}
//		else if (next == PIPE)
//		{
//			piped = true;
//			pipe(pipe_fd);
//			pidd[j] = exec_pipe(buff, env, pipe_fd, &tmp_fd);
//			j++;
//			tmp_fd = pipe_fd[0];
//			close(pipe_fd[1]);
//		}
//		else if (next == COMMA)
//		{
//			exec(buff, env, pipe_fd, tmp_fd);
//			pidd[j] = exec(buff, env, piped, tmp_fd);
//			j++;
//			piped = false;
//			wait_process(pidd);
//			reset_pid(pidd);
//			j = 0;
//		}
//		reset_buff(buff);
//	}
//	close(stdou);
}

//function exec()

//function dup

//function reset fd

//faire gaffe au free
