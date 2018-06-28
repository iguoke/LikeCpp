
all: client block_server 

client:client.o common.o
	cc -o client client.o common.o  -lpthread

block_server:block_server.o common.o
	cc -o block_server block_server.o common.o -lpthread

common.o:common.c common.h
	cc -c common.c

client.o:client.c common.h
	cc -c client.c
block_server.o:block_server.c common.h
	cc -c block_server.c

clean:
	rm *.o

