all: main

CC = clang
override CFLAGS += -g -Wno-everything -pthread -lm

SRCS = $(shell find . -name '.ccls-cache' -type d -prune -o -type f -name '*.c' -print)
HEADERS = $(shell find . -name '.ccls-cache' -type d -prune -o -type f -name '*.h' -print)

main: 
	clean
	gcc main.c -lrt -o main
	./main 1

main-debug: 
	gcc -g producer.c -lrt -o producer
	gcc -g consumer.c -lrt -o consumer
	gdb --args producer 4 10 3

clean:
	rm -f main main-debug