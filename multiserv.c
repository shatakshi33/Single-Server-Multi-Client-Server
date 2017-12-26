
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <pthread.h>
char s[500],c[500];
int connfd[2] = {0},i,temp;
pthread_t threads;

void *reads(void *arg)
{	
		
		while(1)
		{
			temp =  (intptr_t) arg;
			read(temp,c,500);
			for(i=0;i<2;i++)
			{
				if(connfd[i] != temp)
				{
				write(connfd[i],c,500);		
				}
			}
		}
		return (NULL);	
}


int main(int argc, char *argv[])
{
    int listenfd = 0;
    int *p;
    char sendBuff[500];
    struct sockaddr_in serv_addr; 
    pthread_attr_t pthread_custom_attr;
    pthread_attr_init(&pthread_custom_attr);
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 	
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5002); 
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
    listen(listenfd, 10); 
    while(1)
    {
	for(i=0;i<2;i++)
	{
		connfd[i] = accept(listenfd, (struct sockaddr*)NULL, NULL);
		printf("client ready .....\n");
    		printf("client requested.....\n");
		pthread_create(&threads, &pthread_custom_attr,&reads, (void*)(connfd[i]));	
    	
  	}  
	
	pthread_join(threads,NULL);
	
	for(i=0;i<2;i++)
	{
	close(connfd[i]);
	}
	
    }
	
}
