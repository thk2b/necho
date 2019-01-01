SERVER = server
CLIENT = client

CC = gcc
INC = -I inc
FLAGS = -Wall -Wextra -Werror -Ofast -fsanitize=address

SERVER_SRCS = $(addprefix srcs/, server.c lib.c)
SERVER_OBJS = $(SERVER_SRCS:.c=.o)
CLIENT_SRCS = $(addprefix srcs/, client.c lib.c)
CLIENT_OBJS = $(CLIENT_SRCS:.c=.o)

all: $(SERVER) $(CLIENT)

$(CLIENT): $(CLIENT_OBJS)
	$(CC) $(FLAGS) $(INC) $(CLIENT_OBJS) -o $(CLIENT)

$(SERVER): $(SERVER_OBJS)
	$(CC) $(FLAGS) $(INC) $(SERVER_OBJS) -o $(SERVER)

%.o: %.c
	$(CC) $(FLAGS) $(INC) -c $< -o $@

clean:
	rm -f $(SERVER_OBJS) $(CLIENT_OBJS)

fclean: clean
	rm -f $(CLIENT) $(SERVER)

re: fclean all
