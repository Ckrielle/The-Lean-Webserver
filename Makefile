#
# makefile
#
CC = gcc
CFLAGS = -m64 -pthread -g -no-pie

all:
	$(CC) $(CFLAGS) ./src/main.c ./src/prethreading.c ./src/lean.c -o ./main

benchmark:
	$(CC) $(CFLAGS) ./benchmarking/lean_prethread_csapp/main.c ./benchmarking/lean_prethread_csapp/sbuf.c ./benchmarking/lean_prethread_csapp/csapp.c ./benchmarking/lean_prethread_csapp/lean.c -o ./benchmarking/lean_prethread_csapp/main
	$(CC) $(CFLAGS) ./benchmarking/lean_prethread_custom/main.c ./benchmarking/lean_prethread_custom/prethreading.c ./benchmarking/lean_prethread_custom/lean.c -o ./benchmarking/lean_prethread_custom/main
	$(CC) $(CFLAGS) ./benchmarking/simple_lean/simple_lean.c -o ./benchmarking/simple_lean/main
	$(CC) $(CFLAGS) ./benchmarking/simple_tiny/simple_tiny.c ./benchmarking/simple_tiny/csapp.c -o ./benchmarking/simple_tiny/main
	$(CC) $(CFLAGS) ./benchmarking/tiny_prethread_csapp/main.c ./benchmarking/tiny_prethread_csapp/sbuf.c ./benchmarking/tiny_prethread_csapp/tiny.c ./benchmarking/tiny_prethread_csapp/csapp.c -o ./benchmarking/tiny_prethread_csapp/main
	$(CC) $(CFLAGS) ./benchmarking/tiny_prethread_custom/main.c ./benchmarking/tiny_prethread_custom/prethreading.c ./benchmarking/tiny_prethread_custom/tiny.c ./benchmarking/tiny_prethread_custom/csapp.c -o ./benchmarking/tiny_prethread_custom/main

.PHONY : clean
clean:
	find . -type f -executable -print0 | xargs -0 rm -f --

# vim:ft=make
#
