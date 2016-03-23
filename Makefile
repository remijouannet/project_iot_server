all: *.c
	cc ${CFLAGS} -Wall -Werror $^ -o main 
run:
	./main
clean:
	rm -f main
	rm -f *.o
