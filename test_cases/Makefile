all: main

CC = clang
override CFLAGS += -g -Wno-everything -pthread -lm

SRCS = $(shell find . -name '.ccls-cache' -type d -prune -o -type f -name '*.c' -print)
HEADERS = $(shell find . -name '.ccls-cache' -type d -prune -o -type f -name '*.h' -print)

main: 
	clean
	gcc main.c -lrt -o main
	./main 4

main-debug: 
	gcc -g main.c -lrt -o main
	gdb --args main 4

clean:
	rm -f main main-debug