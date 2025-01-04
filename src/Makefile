# Kompilátor a flagy
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Súbory
SRCS = main.c snake.c fruit.c world.c input.c common.c
OBJS = $(SRCS:.c=.o)
TARGET = game

# Ciele
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

