.PHONY: all myfifo run clean
all: run
myfifo: main.c
	gcc main.c -o myfifo
run: myfifo
	./myfifo
clean:
	rm -f myfifo