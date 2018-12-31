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

#define USAGE	"%s [[ port] ip ] -- launch a listening necho server."
#define BUFSIZE	4096
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

struct s_opts
{
	int		port;
	char	*ip;
};

int	get_args(int ac, char **av, struct s_opts *opts)
{
	if (ac == 3)
	{
		opts->ip = av[2];
	}
	else
	{
		opts->ip = "0.0.0.0";
	}
	if (ac >= 2)
	{
		opts->port = atoi(av[1]);
	}
	else
	{
		opts->port = 0;
	}
	return (0);
}

int	handle_connection(int fd)
{
	char				buf[BUFSIZE] = {0};
	ssize_t				nread, tread;

	tread = 0;
	dprintf(2, "INFO:\tstart of transaction\n");
	do
	{
		nread = read(fd, buf, BUFSIZE);
		tread += nread;
		write(1, buf, nread);
	}
	while (nread > 0);
	if (nread == -1)
		return error("read");
	dprintf(2, "INFO:\tend of transaction [%zd bytes]\n", tread);
	return (0);
}

int	necho_server(int listen_fd)
{
	struct sockaddr_in	client_addr;
	socklen_t			client_addrlen;
	int					conn_fd;


	while ((conn_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addrlen)) > 0)
	{
		dprintf(2,
			"INFO:\taccepted connection with %s:%d\n",
			inet_ntoa(client_addr.sin_addr), client_addr.sin_port
		);
		handle_connection(conn_fd);
		close(conn_fd);
		dprintf(2, "INFO:\tclosed connection\n");
	}
	return (error("accept"));
}

int	main(int ac, char **av)
{
	struct s_opts		opts = {0, 0};
	struct sockaddr_in	address;
	int					listen_fd,
						reuse_addr;

	if (get_args(ac, av, &opts))
		return usage(av[0]);
	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return error("socket");
	reuse_addr = 1;
	if ((setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(int))) < 0)
		return error("setsockopt");
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
	return (necho_server(listen_fd));
}
