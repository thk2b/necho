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

static int	necho_server(int listen_fd)
{
	struct sockaddr_in	client_addr;
	socklen_t			client_addrlen;
	int					conn_fd;
	ssize_t				bytes;

	while ((conn_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addrlen)) > 0)
	{
		dprintf(2,
			"INFO:\taccepted connection with %s:%d\n",
			inet_ntoa(client_addr.sin_addr), client_addr.sin_port
		);
		bytes = copy(conn_fd, 1);
		close(conn_fd);
		if (bytes >= 0)
			dprintf(2, "INFO:\tconnection closed after %zd bytes\n", bytes);
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
