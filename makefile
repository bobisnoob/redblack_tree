

rb.o: rb.c
	gcc -g -o  rb.o rb.c && ./rb.o

clean:
	rm rb.o
