#include <necho.h>

ssize_t	copy(int from, int to)
{
	char	buf[BUFSIZE] = {0};
	ssize_t	nread, tread;

	tread = 0;
	do
	{
		if ((nread = read(from, buf, BUFSIZE)) < 0)
			return error(-1, "read");
		tread += nread;
		if (write(to, buf, nread) != nread)
			return error(-1, "write");
	}
	while (nread > 0);
	return tread;
}

int		usage(char *str, char *name)
{
	printf(str, name);
	return 0;
}

int		error(int ret, char *msg)
{
	dprintf(2, "ERROR:\t%s: %s\n", msg, strerror(errno));
	return ret;
}
