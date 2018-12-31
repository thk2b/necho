SERVER = server
CLIENT = client

CC = gcc
FLAGS = -Wall -Wextra -Werror -Ofast -fsanitize=address

all: $(SERVER) $(CLIENT)

$(CLIENT):
	$(CC) $(FLAGS) srcs/client/main.c -o $(CLIENT)

$(SERVER):
	$(CC) $(FLAGS) srcs/server/main.c -o $(SERVER)

clean:

fclean:
	rm -f $(CLIENT) $(SERVER)

re: fclean all
