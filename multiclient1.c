#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <pthread.h>
char s[500],c[500];
int sockfd = 0;
pthread_t threads1, threads2;
void *reads(void *arg)
{
	
	while(1)
	{
		read(sockfd,s,500);
			
		printf("%s\n",s);
	}
	return (NULL);	
}
void *writes(void *arg)
{
		
	while(1)
	{
			
		scanf("%s",c);
		write(sockfd,c,500);	
	}
	return(NULL);
}
int main(int argc, char *argv[])
{
    struct sockaddr_in serv_addr; 
    pthread_attr_t pthread_custom_attr1, pthread_custom_attr2;
    pthread_attr_init(&pthread_custom_attr1);
    pthread_attr_init(&pthread_custom_attr2);
    char recvBuff[500];
    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    } 
    memset(recvBuff,'0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 
    memset(&serv_addr, '0', sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5002); 
    printf("Server address used is: %s\n", argv[1]);
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 
	
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
		printf("\n Error : Connect Failed \n");
		return 1;
    }
	pthread_create(&threads1, &pthread_custom_attr1,&reads, (void *)(sockfd));		
	pthread_create(&threads2, &pthread_custom_attr2, &writes, (void *)(sockfd));	
	pthread_join(threads1,NULL);
	pthread_join(threads2,NULL);        	
   return 0;
}
