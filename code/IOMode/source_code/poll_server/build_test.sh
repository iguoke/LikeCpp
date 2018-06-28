gcc poll_server.c ../common/common.c -D TEST_PERF -o poll_server_test -I /usr/include/gperftools/ -I /usr/include/ -L /usr/lib/ -lprofiler -lunwind -lpthread 
