CC = gcc
CFLAGS = -Wall -Wextra -std=c11

SRCS = main.c snake.c fruit.c world.c
OBJS = $(SRCS:.c=.o)
TARGET = game

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

