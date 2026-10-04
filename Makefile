NAME = us
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude

SRCS = src/main.c src/io.c src/bytes.c src/checksum.c src/header.c src/record.c src/ring.c src/select.c src/json.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c include/usensor.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
