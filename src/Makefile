# Kompiler a flagy
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic

# Ciele
TARGET = game
OBJS = main.o world.o fruit.o snake.o

# Predvolený cieľ
all: $(TARGET)

# Preklad programu
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Preklad jednotlivých modulov
main.o: main.c world.h fruit.h
	$(CC) $(CFLAGS) -c main.c

world.o: world.c world.h
	$(CC) $(CFLAGS) -c world.c

fruit.o: fruit.c fruit.h world.h
	$(CC) $(CFLAGS) -c fruit.c

snake.o: snake.c snake.h world.h 
	$(CC) $(CFLAGS) -c snake.c 

# Vyčistenie preložených súborov
clean:
	rm -f $(OBJS) $(TARGET)

