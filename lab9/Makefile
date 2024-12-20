.PHONY: all producer consumer clean
all: producer consumer
producer: producer.c
	gcc producer.c -o producer
consumer: consumer.c
	gcc consumer.c -o consumer
clean:
	rm -rf *.o producer consumer