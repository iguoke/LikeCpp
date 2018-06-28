NC_DIR=-I/usr/include/
LIB_DIR=-L/usr/lib64/
LIB=-lboost_regex

CC=g++ -g
CFLAGS=-Wall
EXE=test

all:
	$(CC) $(CFLAGS) $(EXE).cpp -o $(EXE) $(INC_DIR) $(LIB_DIR) $(LIB)

clean:
	rm -rf *.o $(EXE)
