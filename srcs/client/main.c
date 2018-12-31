#include		<stdlib.h>
#include		<stdint.h>
#include		<stdio.h>
#include		<string.h>
#include		<errno.h>

#include		<unistd.h>
#include		<arpa/inet.h>
#include		<sys/socket.h>

#define USAGE	"%s [[ port ] addr ] -- connects to a necho server and transmits stdin"

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

struct s_opts{
	int		port;
	char	*addr;
};

int	get_args(int ac, char **av, struct s_opts *opts)
{
	if (ac == 3)
	{
		opts->addr = av[2];
	}
	else
	{
		opts->addr = "0.0.0.0";
	}
	if (ac >= 2)
	{
		opts->port = atoi(av[1]);
	}
	else
	{
		opts->port = 80;
	}
	return 0;

}

int	main(int ac, char **av)
{
	struct s_opts		opts = {0, 0};
	int					sock_fd;
	struct sockaddr_in	address;

	if ((get_args(ac, av, &opts)))
		return usage(av[0]);
	if ((sock_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		return error("socket");
	address.sin_family = AF_INET;
	if (inet_aton(opts.addr, &address.sin_addr) == 0)
		return error("inet_aton");
	address.sin_port = htons(opts.port);
	if (connect(sock_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
		return error("connect");
	dprintf(2, "INFO:\tconnected to server at %s:%d\n", opts.addr, opts.port);
	close(sock_fd);
	return (0);
}
