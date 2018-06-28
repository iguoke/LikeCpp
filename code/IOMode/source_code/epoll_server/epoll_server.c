#include "../common/common.h"
#include <sys/epoll.h>

#define SOCKET_LIST_COUNT 40960

/*
#define PORT 5555
#define MAXMSG 512
#define MINMSG 8
#define FDS_COUNT 512

static int g_suc = 0;
static int g_fai = 0;

void
set_nonblock(int fd) {
	int flags = fcntl(fd, F_GETFL);
	if (flags == -1) {
		perror("get fd flags error");
		exit(EXIT_FAILURE);
	}
	flags |= O_NONBLOCK;
	flags = fcntl(fd, F_SETFL, flags);
	if (flags == -1) {
		perror("set fd flags error");
		exit(EXIT_FAILURE);
	}
}

int
make_socket(uint16_t port) {
	int listen_sock = -1;
	int rc = -1;
	int on = 1;
	struct sockaddr_in name;
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock < 0) {
		perror("create socket error");
		exit(EXIT_FAILURE);
	}

	rc = setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
	if (rc < 0) {
		perror("setsockopt error");
		exit(EXIT_FAILURE);
	}

	rc = ioctl(listen_sock, FIONBIO, (char*)&on);
	if (rc < 0) {
		perror("ioctl failed");
		exit(EXIT_FAILURE);
	}

	name.sin_family = AF_INET;
	name.sin_port = htons(port);
	name.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(listen_sock, (struct sockaddr*)&name, sizeof(name)) < 0) {
		perror("bind error");
		exit(EXIT_FAILURE);
	}
	return listen_sock;
}

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
set_block_filedes_timeout(int filedes) {
 	struct timeval tv_out, tv_in;

	tv_out.tv_sec = 1;
    	tv_out.tv_usec = 0;
	setsockopt(filedes, SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out));

	tv_in.tv_sec = 1;
    	tv_in.tv_usec = 0;
	setsockopt(filedes, SOL_SOCKET, SO_SNDTIMEO, &tv_in, sizeof(tv_in));
	
	set_nonblock(filedes);
}

int
write_client(int filedes) {
	int nbytes;
    	const char* status = "OK";
	int total_len_send;
	int twice = 0;
	int index;
	
	total_len_send = 0;
	index = 0;
	while (1) {
		nbytes = send(filedes, status + index, strlen(status) - index, 0);
		if (nbytes < 0) {
			if (EAGAIN == errno || EWOULDBLOCK == errno || EINTR == errno) {
				errno = 0;
				if (twice) {
					nbytes = 0;
					break;
				}
				twice = !twice;
				usleep(1);
				continue;
			}
			break;
		}
		else if (nbytes >= 0) {
			total_len_send += nbytes;
		}
		
		if (total_len_send == strlen(status)) {
			break;
		}
	}

	if (total_len_send == strlen(status)) {
		__sync_fetch_and_add(&g_suc, 1);
		return -1;
	}
	else {
		__sync_fetch_and_add(&g_fai, 1);
		return 0;
	}
}

int
read_client(int filedes) {
	char buffer[MINMSG];
	int nbytes;
    	const char* status = "OK";
	int total_len_recv;
	int twice = 0;

	total_len_recv = 0;
	while (1) {
		nbytes = recv(filedes, buffer, MINMSG - 1, 0);
		if (nbytes < 0) {
			if (EAGAIN == errno || EWOULDBLOCK == errno || EINTR == errno) {
				errno = 0;
				if (twice) {
					nbytes = 0;
					//perror("read client end");
					break;
				}
				twice = !twice;
				usleep(1);
				continue;
			}
			break;
		}
		if (nbytes == 0) {
			//fprintf(stderr, "read end\n");
			break;
		}
		else if (nbytes > 0) {
			total_len_recv += nbytes;
			//buffer[nbytes] = 0;
			//fprintf(stderr, "%s", buffer);
		}
	}
	
	if (total_len_recv != 7) {
		__sync_fetch_and_add(&g_fai, 1);
		return -1;
	}
	
	return 0;
}

void* print_count(void* arg) {
	struct timeval cur_time;
	int index = 0;
	while (1) {
		sleep(1);
		gettimeofday(&cur_time, NULL);
		fprintf(stderr, "%d\t%ld\t%d\t%d\n", index, cur_time.tv_usec, g_suc, g_fai);
		index++;
	}
}
*/

int deal_socket_count() {
	return 2;
}

int total_loop_count() {
	return 1;
}

int 
main(void) {
	struct epoll_event ev, events[SOCKET_LIST_COUNT];
	int epfd, nfds;
	int listen_sock;
	int index;
	int in_events, out_events;

	int err;

	err = init_print_thread();
	if (err < 0) {
		perror("create print thread error");
		exit(EXIT_FAILURE);
	}

	epfd = epoll_create1(0);
	in_events = EPOLLIN | EPOLLET;
	out_events = EPOLLOUT | EPOLLET;
	
	/* Create the socket and set it up to accept connections. */
	listen_sock = make_socket(1);
	ev.data.fd = listen_sock;
	ev.events = EPOLLIN;
	epoll_ctl(epfd, EPOLL_CTL_ADD, listen_sock, &ev);

	if (listen(listen_sock, SOMAXCONN) < 0) {
		perror("listen error");
		exit(EXIT_FAILURE);
	}

	while (1) {
		nfds = epoll_wait(epfd, events, sizeof(events), 500);
		for (index = 0; index < nfds; ++index) {
			total_loop_count();
			int fd = events[index].data.fd; 
			if (fd == listen_sock) {
				int new_sock;
				deal_socket_count();
				new_sock = accept(fd, NULL, NULL);
				if (new_sock < 0) {
					if (errno == EMFILE) {
					}
					else {
						perror("accept error");
						//if (errno != EWOULDBLOCK) {
						//	continue;
						//}
						exit(EXIT_FAILURE);
					}
				}
				else {
					request_add(1);
					//set_block_filedes_timeout(new_sock);
					ev.data.fd = new_sock;
					ev.events = in_events;
					epoll_ctl(epfd, EPOLL_CTL_ADD, new_sock, &ev);
				}
			}
			else {
				if (events[index].events & EPOLLIN) {
					deal_socket_count();
					if (0 != server_read(fd)) {
						close(fd);
						epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
					}
					else {
						events[index].events = out_events;
						epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &events[index]);
					}
				}
				else if (events[index].events & EPOLLOUT) {
					deal_socket_count();
					server_write(fd);
					events[index].events = in_events;
					epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &events[index]);
					//close(fd);
					//epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
				}
			}
		}
	}
	close(epfd);
	return 0;
}
