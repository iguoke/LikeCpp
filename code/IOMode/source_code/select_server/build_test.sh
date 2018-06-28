gcc select_server.c ../common/common.c -D TEST_PERF -o select_server_test -I /usr/include/gperftools/ -I /usr/include/ -L /usr/lib/ -lprofiler -ltcmalloc -lunwind -lpthread 
