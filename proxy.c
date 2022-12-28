#include <stdio.h>
#include <curl/curl.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/time.h>
#include <dirent.h>
#include <pthread.h>
#include <stdbool.h>
#define MAX 800
#define port 5555
#define SA struct sockaddr

void get_page( char* URL){
  
    CURL *curl;
    CURLcode response;

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, URL);

        response = curl_easy_perform(curl);

        if(response != CURLE_OK) {
            fprintf(stderr, "Request failed: %s\n", curl_easy_strerror(response));
        } /*else {
            printf(response);*/
        }
        curl_easy_cleanup(curl);

}


void handle_request(int fd)
{
	printf("Server loading to connect to remote server...\n");
	struct sockaddr_in remote_server_structure;
	struct hostent* host;
	char ip[20];
	int remote_socket_fd;
	char input_buffer[710];
	char method[300];
	char url[300];
	char final_link[300];
	char* temp=NULL;
	char version[300];
	char* sample = NULL;
	int i=0;
	int n=0;
	bool port_present = false;
	recv(fd,input_buffer,700,0);
	printf("The request recieved by client is : %s\n", input_buffer);
	sscanf(input_buffer,"%s %s %s",method,url,version);
	
	printf("The method is : %s\n", method);
	printf("The url is : %s\n", url);
	printf("The version is : %s\n", version);
	
	get_page(url);
	
}



int main()
{
	int sockfd , connfd, len;
	struct sockaddr_in servaddr, cli;
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		printf("socket creation failed.....\n");
		exit(0);
	}
	else
	printf("socket successfully created...\n");
	bzero(&servaddr, sizeof(servaddr));
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(port);
	
	
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
	    }
	else
	printf("Socket successfully binded..\n");
	
	if((listen(sockfd, 1)) != 0)
	{
		printf("listen failed...\n");
		exit(0);
	}
	else
	{
		printf("server listening...\n");
	}
	len = sizeof(cli);
	while(1)
	{
	    printf("Server in waiting State\n");
		connfd  = accept(sockfd, (SA*) &cli, &len);
		if(fork() == 0)
		{
			printf("Connected to client with fd: %d\n",connfd);
			close(sockfd);
			handle_request(connfd);
			close(connfd);
			exit(0);
		}
		close(client_fd);
		
	}
	
	
}
