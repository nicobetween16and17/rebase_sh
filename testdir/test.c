#include <stdio.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <dirent.h>
# include <sys/wait.h>
# include <limits.h>
# include <errno.h>
# include <signal.h>
#include <string.h>

int main(int ac, char **av, char **env)
{
	int fd_in;
	int fd_out;
	int pid;
	char **cmd = av + 1;
	char c;
	if (!cmd[0])
		exit(printf("no command\n"));
	fd_in = open("infile.txt", O_RDONLY);
	printf("fd = %d\n", fd_in);
	fd_out = open("outfile.txt", O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (ac >= 2)
	{


		pid = fork();
		if (pid < -1)
			exit(printf("error\n"));
		if (pid == 0)
		{
			dup2(fd_in, 0);
			close(fd_in);
			dup2(fd_out, 1);
			close(fd_out);
			execve("/bin/cat", cmd, env);
		}
		else
			wait(&pid);
	}
	return (0);
}