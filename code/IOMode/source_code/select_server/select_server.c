 #include  "../common/common.h"

/*
int
read_from_client_nonblock(int filedes) {
	char buffer[MINMSG];
	int nbytes;
	fprintf(stderr, "\nread once\n");
	int twice = 0;

	set_nonblock(filedes);
	while (1) {
		nbytes = read(filedes, buffer, MINMSG - 1);
		if (nbytes < 0) {
			if (EAGAIN == errno || EWOULDBLOCK == errno || EINTR == errno) {
				// end of file
				errno = 0;
				if (twice) {
					perror("read client end");
					return -1;
				}
				twice = !twice;
				sleep(1);
				continue;
			}
			perror("read client error");
			exit(EXIT_FAILURE);
		}
		else if (nbytes >= 0) {
			buffer[nbytes] = 0;
			fprintf(stderr, "%s", buffer);
		}
	}
}

int
read_from_client_block(int filedes) {
	char buffer[MAXMSG];
	int nbytes;
	fprintf(stderr, "\nread once\n");

	nbytes = read(filedes, buffer, MAXMSG - 1);
	if (nbytes < 0) {
		perror("read client error");
		return -1;
	}
	else if (nbytes == 0) {
		errno = 0;
		perror("read client nothing");
		return 0;
	}
	else  {
		buffer[nbytes] = 0;
		fprintf(stderr, "%s", buffer);
		return -1;
	}
}

int
read_from_client_nonblock2(int filedes) {
	char buffer[MINMSG];
	int nbytes;
	fprintf(stderr, "\nread once\n");

	set_nonblock(filedes);
	nbytes = read(filedes, buffer, MINMSG - 1);
	if (nbytes < 0) {
		if (EAGAIN == errno || EWOULDBLOCK == errno || EINTR == errno) {
			errno = 0;
			return 1;
		}
		perror("read client error");
		exit(EXIT_FAILURE);
	}
	else if (nbytes >= 0) {
		buffer[nbytes] = 0;
		fprintf(stderr, "%s", buffer);
		return nbytes;
	}
}

int
read_from_client_block2(int filedes) {
	char buffer[MINMSG];
	int nbytes;
	fprintf(stderr, "\nread once\n");

	nbytes = read(filedes, buffer, MINMSG - 1);
	if (nbytes < 0) {
		perror("read client error");
		exit(EXIT_FAILURE);
	}
	else if (nbytes == 0) {
		errno = 0;
		perror("read client end");
		return 0;
	}
	else  {
		buffer[nbytes] = 0;
		fprintf(stderr, "%s", buffer);
		return nbytes;
	}
}

void
deal_client(int filedes) {
	char buffer[MAXMSG];
 	struct timeval tv_out, tv_in;
	int nbytes;
    	const char* status = "OK";

	tv_out.tv_sec = 1;
    	tv_out.tv_usec = 0;
	setsockopt(filedes, SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out));

	tv_in.tv_sec = 1;
    	tv_in.tv_usec = 0;
	setsockopt(filedes, SOL_SOCKET, SO_SNDTIMEO, &tv_in, sizeof(tv_in));
	
	nbytes = 1;
	while (nbytes > 0) {
		nbytes = recv(filedes, buffer, MINMSG - 1, 0);
		buffer[nbytes] = 0;
		fprintf(stderr, "%s", buffer);
	}
	
	if (0 > nbytes) {
		perror("1\n");
		//__sync_fetch_and_add(&g_fai, 1);
	}
	else {
		nbytes = send(filedes, status, strlen(status), 0);
		if (0 > nbytes) {
			perror("\n");
			//__sync_fetch_and_add(&g_fai, 1);
		}
		else {
			//__sync_fetch_and_add(&g_suc, 1);
		}
	}	
}

*/


void deal_socket(int index, int listen_sock, fd_set* active_fd_set, fd_set* read_fd_set) {
	if (listen_sock == index) {
		/* Connection request on original socket. */
		int new_sock;
		new_sock = accept(listen_sock, NULL, NULL);
		if (new_sock < 0) {
			perror("accept error");
			exit(EXIT_FAILURE);
		}
		request_add(1);
		FD_SET(new_sock, active_fd_set);
	} else {
		if (0 == server_read(index)) {
			server_write(index);
		}
		close(index);
		FD_CLR(index, active_fd_set);
	}
}

void deal_request(int listen_sock, fd_set* active_fd_set, fd_set* read_fd_set) {
	int index = 0;
	for (index = 0; index < FD_SETSIZE; ++index) {
		if (FD_ISSET(index, read_fd_set)) {
			deal_socket(index, listen_sock, active_fd_set, read_fd_set);
		}
	}
}

int 
main(void) {
	int listen_sock;
	fd_set active_fd_set, read_fd_set;
	int index;
	struct timeval timeout;
	int err;

	err = init_print_thread();
	if (err < 0) {
		perror("create print thread error");
		exit(EXIT_FAILURE);
	}

#ifdef TEST_PERF
	init_gperftools("select_server.prof");
#endif


	/* Create the socket and set it up to accept connections. */
	listen_sock = make_socket(1);
	if (listen(listen_sock, SOMAXCONN) < 0) {
		perror("listen error");
		exit(EXIT_FAILURE);
	}
	FD_ZERO(&active_fd_set);
	FD_SET(listen_sock, &active_fd_set);

	/* Initialize the set of active sockets. */
	while (1) {
		timeout.tv_sec = 0;
		timeout.tv_usec = 500;

		/* Service all the sockets with input pending. */
		read_fd_set = active_fd_set;
		switch(select(FD_SETSIZE, &read_fd_set, NULL, NULL, &timeout)) {
			case -1 : {
				perror("select error\n");
				exit(EXIT_FAILURE);
			}break;
			case 0 : {
				//perror("select timeout\n");
			}break;
			default: {
				deal_request(listen_sock, &active_fd_set, &read_fd_set);
			}
		}
	}
	return 0;
}
