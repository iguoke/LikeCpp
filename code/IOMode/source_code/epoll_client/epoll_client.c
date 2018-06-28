#include "../common/common.h"
#include <sys/epoll.h>

#define MAXREQUESTCOUNT 100000

#define SOCKET_LIST_COUNT 1024

static int g_total = 0;
static int g_max_total = 0;

void* send_data(void* arg) {
	int wait_time;
	int epfd, nfds;
	int client_sock;
	struct epoll_event ev, events[SOCKET_LIST_COUNT];
	int in_events, out_events;
	int index;

	wait_time = *(int*)arg;
	in_events = EPOLLIN | EPOLLET;
	out_events = EPOLLOUT | EPOLLET;
	
	epfd = epoll_create1(0);

	for (index = 0; index < 100; index++) {
		client_sock = make_client_socket();
		connect_server(client_sock);
		set_nonblock(client_sock);
		request_add(1);
		
		ev.data.fd = client_sock;
		ev.events = in_events;
		epoll_ctl(epfd, EPOLL_CTL_ADD, client_sock, &ev);
		
		client_write(client_sock);
		ev.events = in_events;
		epoll_ctl(epfd, EPOLL_CTL_MOD, client_sock, &ev);
	}	

	while (1) {
		nfds = epoll_wait(epfd, events, sizeof(events), 500);
		usleep(wait_time);
		for (index = 0; index < nfds; ++index) {
			int fd = events[index].data.fd;
			if (events[index].events & EPOLLIN) {
				if (0 != client_read(fd)) {
					close(fd);
					epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
				}
				else {
					events[index].events = out_events;
					epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &events[index]);
				}
			}
			else if (events[index].events & EPOLLOUT) {
				client_write(fd);
				events[index].events = in_events;
				epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &events[index]);
			}
		}
	}
	close(epfd);
}

int 
main(int argc, char* argv[]) {
	int thread_count;
	int index;
	int err;
	int wait_time;
	pthread_t thread_id;

	if (argc < 3) {
		fprintf(stderr, "error! example: client 10 50\n");
		return 0;
	}

	err = init_print_thread();
	if (err < 0) {
		perror("create print thread error");
		exit(EXIT_FAILURE);
	}

	thread_count = atoi(argv[1]);
	wait_time = atoi(argv[2]);
	
	g_max_total = MAXREQUESTCOUNT;
	if (argc > 3) {
		g_max_total = atoi(argv[3]);
	}	

	for (index = 0; index < thread_count; index++) {
		err = pthread_create(&thread_id, NULL, send_data, &wait_time);
		if (err != 0) {
			perror("can't create send thread");
			exit(EXIT_FAILURE);
		}
		
	}

	wait_print_thread();
	return 0;
}
