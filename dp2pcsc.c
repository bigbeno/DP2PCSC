#include "client.h"
static struct sockaddr_in listen_addr_in;

static char inputbuf[INPUT_BUFSIZE];

int main (int argc, char *argv[])
{
	client_shutdown = 0;
	init_connector();
	init_socket();
	init_show();
/*	sleep(1);*/
/*	client_shutdown = 1;*/
/*	sleep(3);*/
	while(!client_shutdown){
		input();
	}
	return 0;
}//end main-function

void init_socket()
{
	listen_socket_fd = socket(PF_INET,SOCK_STREAM,0);//PF_INET->TCP/IP Protocol Family,SOCK_STREAM->TCP
	int optval = 1;
	setsockopt(listen_socket_fd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(SO_REUSEADDR));//enable port multiplexing
	memset(&listen_addr_in,0,sizeof(listen_addr_in));
	listen_addr_in.sin_family = AF_INET;//AF_INET->TCP/IP Address Family
	listen_addr_in.sin_addr.s_addr = htonl(INADDR_ANY);//wildcard IPv4 address
	listen_addr_in.sin_port = htons(SERVER_PORT);
	bind(listen_socket_fd,(struct sockaddr *)&listen_addr_in,sizeof(listen_addr_in));
	listen(listen_socket_fd,LISTEN_LIST_LENGTH);//set listen list length,begin listen
	listen_thread_id = (pthread_t *)malloc(sizeof(pthread_t));
	
	pthread_create(listen_thread_id, NULL, listen_thread, 0);//begin listen thread

}

int input(){
	int input_bufsize = INPUT_BUFSIZE;
	char *friend_name;
	
	fgets(inputbuf,input_bufsize,stdin);
	
	char *result;
	result = strtok(inputbuf, ":");
	char *message;
	friend_name = result;
	result = strtok(NULL, "\n");
	message = result;
	if (friend_name != NULL && message != NULL) {
		printf("[TEST] %s \n",result);
		printf("[TEST] message %s \n",result);		
		send_message(friend_name,message);
		return TRUE;
	}
	printf("%s","format <name><:><message>\nexit to close it\n");
	return FALSE;	
}

void send_message(char *friend_name,char *message){
	struct friend *this = (struct friend *)malloc(sizeof(struct friend));
	int result = find_connector_by_name(&connectors, friend_name, this);
	printf("findresult  %d \n",result);
	if (result != 0) {//make new connect & add connectors
		socket_fd friend_socket_fd;
		friend_socket_fd = socket(PF_INET, SOCK_STREAM, 0);//PF_INET->TCP/IP Protocol Family,SOCK_STREAM->TCP
		struct sockaddr_in dest_addr;
		char *friend_ip;
		get_friend_address(friend_name, friend_ip);
		dest_addr.sin_family = AF_INET;//AF_INET->TCP/IP Address Family
		//dest_addr.sin_port = htons(DEST_PORT);
		dest_addr.sin_addr.s_addr = inet_addr(friend_ip);
		memset(&(dest_addr.sin_zero), 0, sizeof(dest_addr.sin_zero));
		free(friend_ip);
		int result;
		
		result = connect(friend_socket_fd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr));
		printf("result %d",result);
		if (result) 
			return;
		
		
		
		this->friend_name = (char *)malloc(strlen(friend_name) * sizeof(char));
		memcpy(friend_name, this->friend_name, strlen(friend_name));
		pthread_t talk_thread_id;
		pthread_create(&talk_thread_id, NULL, talk_thread, 0);
		//pthread_detach(talk_thread_id);
		this->friend_thread_id = talk_thread_id;
		this->friend_socket_fd = friend_socket_fd;
		this->state = TALK_RUNNING;
		
		add_connector(&connectors, this);
		free(this);
	}
	
	
	int input_length = strlen(inputbuf);

	for (int i = 0; i < input_length / SEND_BUFSIZE; i += 1) {
		char *sendbuf = (char *)malloc(SEND_BUFSIZE * sizeof(char));
		memset(sendbuf, 0, SEND_BUFSIZE * sizeof(char));
		//TODO
		send(this->friend_socket_fd, sendbuf, strlen(sendbuf), 0);
		free(sendbuf);
	}
	
		
	
	memset(inputbuf,0,input_length);

}


void get_friend_address(char *friend_name, char *friend_ip)
{
	friend_ip = (char *)malloc(SEND_BUFSIZE * sizeof(char));
	strcpy(friend_ip,"172.0.0.1");		
}