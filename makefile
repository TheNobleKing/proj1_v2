all : client.c server.c
	gcc client.c -o client.o
	gcc server.c -o server.o
clean: 
	rm *.o

run: client.o server.o
	./server.o &
	./client.o &
