#include <necho.h>

ssize_t	copy(int from, int to, char *header, int append_newline)
{
	char	buf[BUFSIZE] = {0};
	ssize_t	nread, tread;

	tread = 0;
	do
	{
		if ((nread = read(from, buf, BUFSIZE)) < 0)
			return error(-1, "read");
		tread += nread;
		if (header && nread > 0)
			write(to, header, strlen(header));
		if (write(to, buf, nread) != nread)
			return error(-1, "write");
	}
	while (nread > 0);
	if (append_newline && buf[nread] != '\n')
		if (write(to, "\n", 1) != 1)
			return error(-1, "write");
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
