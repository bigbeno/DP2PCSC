#include "listen_thread.h"

void *listen_thread(void *arg)
{
	pthread_detach(pthread_self());
	while (!client_shutdown) {
		socket_fd talk_socket_fd;
		struct sockaddr client_addr_in;
		socklen_t client_addr_in_len = sizeof(struct sockaddr);
		talk_socket_fd = accept(listen_socket_fd,
						(struct sockaddr *)&client_addr_in,
						&client_addr_in_len);
		//printf("[talk_socket_fd]%d\n",talk_socket_fd);
		if (talk_socket_fd > 2) {//0 stdin; 1 stdout; 2 stderr
			pthread_t talk_thread_id;
		//	printf("[LISTEN___LISTEN]\n");
			pthread_create(&talk_thread_id, NULL, talk_thread, (void *)&talk_socket_fd);
			//pthread_detach(talk_thread_id);
		}else{
			usleep(500);
		}
	}
	
	pthread_exit((void *)NULL);
	//return (void *)NULL;
}



