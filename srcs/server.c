#include		<necho.h>

#define USAGE	"%s [ port ] -- launch a listening necho server."


int			get_args(int ac, char **av, struct s_server_opts *opts)
{
	if (ac >= 2)
		opts->port = atoi(av[1]);
	else
		opts->port = 8080;
	return (0);
}

static void	sigchld_handler(int sig)
{
	(void)sig;
	while (waitpid(-1, NULL, WNOHANG) > 0)
		;
}

static int	necho_server_child(int conn_fd, struct sockaddr_in *client_addr)
{
	ssize_t				bytes;
	char				header[25] = {0};

	snprintf(header, 24, "%15s:%5d:\t", inet_ntoa(client_addr->sin_addr), client_addr->sin_port);
	dprintf(2, "%sINFO:\taccepted connection\n", header);
	bytes = copy(conn_fd, 1, header, 0);
	close(conn_fd);
	if (bytes >= 0)
		dprintf(2, "%sINFO:\tconnection closed after %zd bytes\n", header, bytes);
	return (bytes != -1);
}

static int	necho_server(int listen_fd)
{
	struct sockaddr_in	client_addr;
	socklen_t			client_addrlen;
	int					conn_fd;
	pid_t				pid;

	signal(SIGCHLD, sigchld_handler);
	while ((conn_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addrlen)) > 0)
	{
		if ((pid = fork()) == -1)
			return error(0, "fork");
		if (pid == 0)
		{
			close(listen_fd);
			_exit(necho_server_child(conn_fd, &client_addr));
		}
		close(conn_fd);
	}
	return (error(0, "accept"));
}

static int	open_listening_socket(struct sockaddr_in *addr, struct s_server_opts *opts)
{
	int	reuse_addr = 1;
	int	listen_fd;

	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return error(0, "socket");
	reuse_addr = 1;
	if ((setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(int))) < 0)
		return error(0, "setsockopt");
	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr = htonl(INADDR_ANY);
	addr->sin_port = htons((int16_t)opts->port);
	if (bind(listen_fd, (struct sockaddr*)addr, sizeof(struct sockaddr_in)) < 0)
		return error(0, "bind");
	if (listen(listen_fd, 1024) < 0)
		return error(0, "listen");
	return (listen_fd);
}

int			main(int ac, char **av)
{
	struct s_server_opts	opts = {0};
	struct sockaddr_in		address;
	int						listen_fd;

	if (get_args(ac, av, &opts))
		return usage(USAGE, av[0]);
	if ((listen_fd = open_listening_socket(&address, &opts)) == 0)
		return (errno);
	dprintf(2,
		"INFO:\twaiting for connections on %s:%d\n",
		inet_ntoa(address.sin_addr), opts.port
	);
	return (necho_server(listen_fd));
}
