#pragma once

# include		<stdlib.h>
# include		<stdint.h>
# include		<stdio.h>
# include		<string.h>
# include		<errno.h>
# include		<unistd.h>
# include		<arpa/inet.h>
# include		<sys/socket.h>
# include		<signal.h>

# define BUFSIZE	4069

/*
**	client
*/

struct	s_client_opts{
	int		port;
	char	*addr;
};

/*
**	server
*/

struct	s_server_opts
{
	int		port;
};

/*
**	lib
*/

int		usage(char *str, char *name);
int		error(int ret, char *msg);
ssize_t	copy(int from, int to, char *header, int append_final_newline);
