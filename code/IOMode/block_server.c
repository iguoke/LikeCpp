#include "common.h"
/*
void
deal_client(int filedes) {
	char buffer[MINMSG];
	int nbytes;
	int total_length_recv;
    	const char* status = "OK";
	total_length_recv = 0;
	nbytes = 1;
	while (nbytes > 0) {
		nbytes = recv(filedes, buffer, sizeof(buffer) - 1, 0);
		if (nbytes > 0) {
			total_length_recv += nbytes;
		}
		//buffer[nbytes] = 0;
		//fprintf(stderr, "%s", buffer);
	}

	fprintf(stderr, "%d", total_length_recv);
	if (7 != total_length_recv) {
		__sync_fetch_and_add(&g_fai, 1);
		return;
	}
	
	if (0 > nbytes) {
		perror("1\n");
		__sync_fetch_and_add(&g_fai, 1);
	}
	else {
		nbytes = send(filedes, status, strlen(status), 0);
		if (0 > nbytes) {
			perror("\n");
			__sync_fetch_and_add(&g_fai, 1);
		}
		else {
			__sync_fetch_and_add(&g_suc, 1);
		}
	}	
}
*/

int 
main(void) {
	int listen_sock;
	pthread_t print_thread;
	int err;
	
#ifdef TEST_PERF
	init_gperftools("block_server.prof");
#endif

	err = init_print_thread();
	if (err < 0) {
		perror("create print thread error");
		exit(EXIT_FAILURE);
	}

	listen_sock = make_socket(0);
	if (listen(listen_sock, SOMAXCONN) < 0) {
		perror("listen error");
		exit(EXIT_FAILURE);
	}

	while (1) {
		int new_sock;
		new_sock = accept(listen_sock, NULL, NULL);
		if (new_sock < 0) {
			perror("accept error");
			exit(EXIT_FAILURE);
		}
		request_add(1);
		set_block_filedes_timeout(new_sock);
		if (0 == server_read(new_sock)) {
			server_write(new_sock);
		}
		close(new_sock);
	}
	return 0;
}
