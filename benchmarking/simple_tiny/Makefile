#
# makefile
#
CC = gcc
CFLAGS = -m64 -pthread -g -no-pie

all:
	$(CC) $(CFLAGS) simple_tiny.c csapp.c -o simple_tiny

test:

.PHONY : clean
clean:
	find . -type f -executable -print0 | xargs -0 rm -f --

# vim:ft=make
#