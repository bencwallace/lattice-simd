CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -O3 -mavx2

SRCS = base4d.c trans4d.c #trans2dx4.c trans2d.c
OBJS = $(SRCS:.c=.o)
TARGETS = base4d trans4d #trans2dx4

all: $(TARGETS)

base4d: base4d.o
	$(CC) -o base4d base4d.o

base4d.o: base4d.c
	$(CC) -c base4d.c $(CFLAGS)

base4d.s: base4d.c
	$(CC) -S base4d.c $(CFLAGS)

# trans2d: trans2d.o
# 	$(CC) -o trans2d trans2d.o

# trans2d.o: trans2d.c
# 	$(CC) -c trans2d.c $(CFLAGS)

trans4d: trans4d.o
	$(CC) -o trans4d trans4d.o

trans4d.o: trans4d.c
	$(CC) -c trans4d.c $(CFLAGS)

trans4d.s: trans4d.c
	$(CC) -S trans4d.c $(CFLAGS)

# trans2dx4: trans2dx4.o
# 	$(CC) -o trans2dx4 trans2dx4.o

# trans2dx4.o: trans2dx4.c
# 	$(CC) -c trans2dx4.c $(CFLAGS)

clean:
	rm -f $(OBJS) $(TARGETS)
