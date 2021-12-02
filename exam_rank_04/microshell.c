#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define fatalMsg            "error: fatal"
#define fatalExecMsg        "error: cannot execute "
#define cdBadArgMsg         "error: cd: bad arguments"
#define cdBadDirMsg         "error: cd: cannot change directory to "

#define err(errMsg, path)   write(STDERR_FILENO, errMsg, ft_strlen(errMsg)); \
							write(STDERR_FILENO, path, ft_strlen(path)); \
							write(STDERR_FILENO, "\n", 1);

#define fatalErr            err(fatalMsg, "")
#define fatalExecErr(path)  err(fatalExecMsg, path)
#define cdBadArgErr         err(cdBadArgMsg, "")
#define cdBadDirErr(path)   err(cdBadDirMsg, path)

int  ft_strlen(char *str)
{
	int i = 0;

	while (str[i] != '\0')
		i++;
	return i;
}

int	find_nb_cmd(char **av)
{
	int nb_cmd = 1;

	while (*av != NULL && strcmp(*av, ";") != 0)
	{
		if (strcmp(*av++, "|") == 0)
			nb_cmd++;
	}
	return (nb_cmd);
}

int	find_nb_args(char **av)
{
	int nb_args = 0;

	while (*av != NULL && strcmp(*av, ";") != 0 && strcmp(*av++, "|") != 0)
		nb_args++;
	return nb_args;
}

void	exec_cd(char **av, int nb_args)
{
	if (nb_args != 2)
	{
		cdBadArgErr;
	}
	else if (chdir(av[1]) == -1)
	{
		cdBadDirErr(av[1]);
	}
}

void	exec_cmd(char **av, int nb_args, char **env)
{
	char *args[nb_args + 1];

	args[nb_args] = NULL;
	for(int i = 0; i < nb_args; i++)
		args[i] = av[i];
	if (execve(args[0], args, env) == -1)
	{
		fatalExecErr(args[0]);
		exit(EXIT_FAILURE);
	}
}

int	run_cmd(char **av, int nb_cmd, char **env)
{
	int fd_pipe[2];
	int fd_in_cmd = -1;
	int nb_args_sum = 0;

	for(int i = 0; i < nb_cmd ; i++)
	{
		if (i != 0)
		{
			if (fd_in_cmd != -1)
				close(fd_in_cmd);
			fd_in_cmd = fd_pipe[0];
		}

		if (i < nb_cmd - 1)
		{
			if (pipe(fd_pipe) == -1)
			{
				fatalErr;
				exit(EXIT_FAILURE);
			}
		}

		int nb_args = find_nb_args(av + nb_args_sum);

		if (strcmp(av[nb_args_sum], "cd") == 0)
		{
			exec_cd(av + nb_args_sum, nb_args);
			goto Jump;
		}

		pid_t pid = fork();

		if (pid == -1)
		{
			fatalErr;
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			if (i != 0 && dup2(fd_in_cmd, STDIN_FILENO) == -1)
			{				
				fatalErr;
				exit(EXIT_FAILURE);
			}
			if (i < nb_cmd - 1 && dup2(fd_pipe[1], STDOUT_FILENO) == -1)
			{
				fatalErr;
				exit(EXIT_FAILURE);
			}
			exec_cmd(av + nb_args_sum, nb_args, env);
		}

		int status = 0;
		waitpid(pid, &status, 0);

		Jump:
		if(i == nb_cmd - 1 && nb_cmd > 1)
			close(fd_in_cmd);
		if(i < nb_cmd - 1)
			close(fd_pipe[1]);
		nb_args_sum += i < nb_cmd - 1 ? nb_args + 1 : nb_args;

	}
	return (nb_args_sum);
}

int main(__attribute__ ((unused)) int ac, char **av, char **env)
{
	for (int i = 1; av[i] != NULL ; i++)
	{
		if (strcmp(av[i], ";") != 0)
		{
			int nb_cmd = find_nb_cmd(av + i);
			i += run_cmd(av + i, nb_cmd, env) - 1;
		}
	}
	return (EXIT_SUCCESS);
}
