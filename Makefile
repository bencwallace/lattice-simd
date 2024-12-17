CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -O3 -mavx2

TARGETS := base2d base4d trans4d

all: $(TARGETS)

$(TARGETS): %: %.o
	$(CC) $(CFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) *.o
