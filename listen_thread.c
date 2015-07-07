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
		if (talk_socket_fd == 0) {
			pthread_t talk_thread_id;
			pthread_create(&talk_thread_id, NULL, talk_thread, 0);
			//pthread_detach(talk_thread_id);
		}else{
			usleep(500);
		}
	}
	
	free(listen_thread_id);
	close(listen_socket_fd);
	pthread_exit((void *)NULL);
	//return (void *)NULL;
}