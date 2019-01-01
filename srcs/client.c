#include		<necho.h>

#define USAGE	"%s [ port [ addr ]] -- connects to a necho server and transmits stdin"

int	get_args(int ac, char **av, struct s_client_opts *opts)
{
	if (ac == 3)
		opts->addr = av[2];
	else
		opts->addr = "0.0.0.0";
	if (ac >= 2)
		opts->port = atoi(av[1]);
	else
		opts->port = 8080;
	return 0;

}

int	main(int ac, char **av)
{
	struct s_client_opts	opts = {0, 0};
	int						sock_fd;
	struct sockaddr_in		address;
	ssize_t					bytes;

	if ((get_args(ac, av, &opts)))
		return usage(USAGE, av[0]);
	if ((sock_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		return error(errno, "socket");
	address.sin_family = AF_INET;
	if (inet_aton(opts.addr, &address.sin_addr) == 0)
		return error(errno, "inet_aton");
	address.sin_port = htons(opts.port);
	if (connect(sock_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
		return error(errno, "connect");
	dprintf(2, "INFO:\tconnected to server at %s:%d\n", opts.addr, opts.port);
	bytes = copy(0, sock_fd, NULL);
	if (bytes >= 0)
		dprintf(2, "INFO:\tsent %zd bytes\n", bytes);
	close(sock_fd);
	dprintf(2, "INFO:\tclosed connection\n");
	return (0);
}
