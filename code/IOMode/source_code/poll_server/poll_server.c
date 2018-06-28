#include "../common/common.h"
#include <sys/poll.h>

#define FDS_COUNT 40960

/*
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>

#define PORT 5555
#define MAXMSG 512
#define MINMSG 8

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
deal_client2(int filedes) {
	char buffer[MINMSG];
 	struct timeval tv_out, tv_in;
	int nbytes;
    	const char* status = "OK";
	int total_len_recv;
	int twice = 0;

	tv_out.tv_sec = 1;
    	tv_out.tv_usec = 0;
	setsockopt(filedes, SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out));

	tv_in.tv_sec = 1;
    	tv_in.tv_usec = 0;
	setsockopt(filedes, SOL_SOCKET, SO_SNDTIMEO, &tv_in, sizeof(tv_in));
	
	total_len_recv = 0;
	set_nonblock(filedes);
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
		return;
	}

	
	if (0 > nbytes) {
		__sync_fetch_and_add(&g_fai, 1);
	}
	else {
		nbytes = send(filedes, status, strlen(status), 0);
		if (0 > nbytes) {
			__sync_fetch_and_add(&g_fai, 1);
		}
		else {
			__sync_fetch_and_add(&g_suc, 1);
		}
	}	
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
	int listen_sock;
	struct pollfd fds[FDS_COUNT];
	int timeout;
	int cur_fds_count;
	int rc;
	int index;
	int expect_events;
	int error_events;

	int err;

#ifdef TEST_PERF
	init_gperftools("poll_server.prof");
#endif

	err = init_print_thread();
	if (err < 0) {
		perror("create print thread error");
		exit(EXIT_FAILURE);
	}

	memset(fds, 0, sizeof(fds));

	/* Create the socket and set it up to accept connections. */
	listen_sock = make_socket(1);
	if (listen(listen_sock, SOMAXCONN) < 0) {
		perror("listen error");
		exit(EXIT_FAILURE);
	}
	//set_nonblock(listen_sock);

	error_events = POLLERR | POLLNVAL;
	//expect_events = POLLIN | POLLOUT;
	expect_events = POLLIN;
	
	fds[0].fd = listen_sock;
	fds[0].events = POLLIN;
	cur_fds_count = 1;

	timeout = (500);	

	while (1) {
		rc = poll(fds, cur_fds_count, timeout);
		if (rc < 0) {
			perror("poll error\n");
			exit(EXIT_FAILURE);
		};
		if (rc == 0) {
			continue;
			//perror("poll timeout\n");
		};
		
		int cur_fds_count_temp = cur_fds_count;
		for (index = 0; index < cur_fds_count_temp; ++index) {
			total_loop_count();
			if (fds[index].revents == 0) {
				continue;
			}
			if (fds[index].revents & error_events) {
				perror("revents error");
				if (fds[index].fd == listen_sock) {
					perror("listen sock error");
					exit(EXIT_FAILURE);
				}
				else {
					close(fds[index].fd);
					cur_fds_count--;
					if (index < cur_fds_count) {
						memcpy(&fds[index], &fds[cur_fds_count], sizeof(fds[cur_fds_count]));
						memset(&fds[cur_fds_count], 0, sizeof(fds[cur_fds_count]));
						index--;
					}
					cur_fds_count_temp--;
					continue;
				}	
			}
			if (!(fds[index].revents & fds[index].events)) {
				continue;
			}
			deal_socket_count();
			if (fds[index].fd == listen_sock) {
				int new_sock;
				new_sock = accept(listen_sock, NULL, NULL);
				if (new_sock < 0) {
					//perror("accept error");
					if (errno != EWOULDBLOCK) {
						continue;
					}
					exit(EXIT_FAILURE);
				}
				else {
					request_add(1);
					//set_block_filedes_timeout(new_sock);
					if (cur_fds_count + 1 < sizeof(fds)) {
						fds[cur_fds_count].fd = new_sock;
						fds[cur_fds_count].events = expect_events;
						cur_fds_count++;
					}
				}
			}
			else {
				if (0 == server_read(fds[index].fd)) {
					server_write(fds[index].fd);
				}
				//close(fds[index].fd);
				//cur_fds_count--;
				//if (index < cur_fds_count) {
				//	memcpy(&fds[index], &fds[cur_fds_count], sizeof(fds[cur_fds_count]));
				//	memset(&fds[cur_fds_count], 0, sizeof(fds[cur_fds_count]));
				//	index--;
				//}
				//cur_fds_count_temp--;
			}
		}
	}
	return 0;
}
