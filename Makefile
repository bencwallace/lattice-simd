CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -mavx2 -O3

SRCS = trans4d.c #trans2dx4.c trans2d.c
OBJS = $(SRCS:.c=.o)
TARGETS = trans4d trans2dx4

all: $(TARGETS)

# trans2d: trans2d.o
# 	$(CC) -o trans2d trans2d.o

# trans2d.o: trans2d.c
# 	$(CC) -c trans2d.c $(CFLAGS)

trans4d: trans4d.o
	$(CC) -o trans4d trans4d.o

trans4d.o: trans4d.c
	$(CC) -c trans4d.c $(CFLAGS)

# trans2dx4: trans2dx4.o
# 	$(CC) -o trans2dx4 trans2dx4.o

# trans2dx4.o: trans2dx4.c
# 	$(CC) -c trans2dx4.c $(CFLAGS)

clean:
	rm -f $(OBJS) $(TARGETS)
