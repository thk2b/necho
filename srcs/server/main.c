#include		<stdlib.h>
#include		<stdint.h>
#include		<stdio.h>
#include		<string.h>
#include		<errno.h>

#include		<unistd.h>
#include		<arpa/inet.h>
#include		<sys/types.h>
#include		<netinet/in.h>
#include		<sys/socket.h>

#define USAGE	"%s [ port ]-- launch a listening necho server."

int	usage(char *name)
{
	printf(USAGE, name);
	return 0;
}

int	error(char *msg)
{
	dprintf(2, "ERROR:\t%s: %s\n", msg, strerror(errno));
	return errno;
}

struct	s_opts
{
	int	port;
};

int	get_args(int ac, char **av, struct s_opts *opts)
{
	if (ac == 2)
	{
		if (strcmp(av[1], "--help") == 0)
			return (1);
		opts->port = atoi(av[1]);
	}
	else
	{
		opts->port = 0;
	}
	return (0);
}

int	necho(int listen_fd)
{
	struct sockaddr_in	client_addr;
	socklen_t			client_addrlen;
	int					conn_fd;

	while ((conn_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addrlen)) > 0)
	{
		dprintf(2,
			"INFO:\taccepted connection with %s:%d",
			inet_ntoa(client_addr.sin_addr), client_addr.sin_port
		);
		close(conn_fd);
	}
	return (error("accept"));
}

int	main(int ac, char **av)
{
	struct s_opts		opts = {0};
	struct sockaddr_in	address;
	int					listen_fd;

	if (get_args(ac, av, &opts))
		return usage(av[0]);
	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return error("socket");
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons((int16_t)opts.port);
	if (bind(listen_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
		return error("bind");
	if (listen(listen_fd, 1024) < 0)
		return error("listen");
	dprintf(2,
		"INFO:\twaiting for connections on %s:%d\n",
		inet_ntoa(address.sin_addr), opts.port
	);
	return (necho(listen_fd));
}