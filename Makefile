CC = gcc

shell : main.o builtin.o history.o execute.o pipe.o
	$(CC) main.o builtin.o history.o execute.o pipe.o -o shell

main.o : main.c header.h
	$(CC) -c main.c

builtin.o : builtin.c header.h
	$(CC) -c builtin.c

history.o : history.c header.h
	$(CC) -c history.c

execute.o : execute.c header.h
	$(CC) -c execute.c

pipe.o : pipe.c header.h
	$(CC) -c pipe.c

clean :
	rm -f *.o shell
