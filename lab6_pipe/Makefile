.PHONY: all mypipe run clean
all: run
mypipe: main.c
	gcc main.c -o mypipe
run: mypipe
	./mypipe
clean:
	rm -f mypipe