#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <netdb.h> 

#define MAX_NAME 10

int main(int argc, char const *argv[])
{ 
    assert(argc==2);
    int connection_socket=socket(PF_INET,SOCK_STREAM,0);

    struct sockaddr_in server_adr;
    struct hostent *server;
    server=gethostbyname("lulu.informatique.univ-paris-diderot.fr"); 
    if(server==NULL)
    {
        perror("couldn't find the host\n");
        exit(1);
    }
    
    struct in_addr **addresses=(struct in_addr**)server->h_addr_list;
    server_adr.sin_family=AF_INET;
    server_adr.sin_port=htons((uint16_t)atoi(argv[1]));
    server_adr.sin_addr=(**(addresses));

    printf("connecting to Address : %s\n",inet_ntoa(**addresses));
    

    int size=sizeof(server_adr);

    if (connect(connection_socket,(struct sockaddr *)&server_adr,(socklen_t)size)==0){

        char *psudo=malloc(MAX_NAME);
        char *response1=malloc(MAX_NAME+6);
      
        printf("ENTER PSUDO  :");
        scanf("%10s",psudo);
        if (send(connection_socket,psudo,MAX_NAME,0)<0)
        {
            perror("prolem while sending\n");
        }
        
        if (recv(connection_socket,response1,MAX_NAME+6,0)<0)
        {
            perror("prolem while recieving\n");
        }
        printf("server reply: %s\n",response1);

        if (send(connection_socket,"MAX\0",4,0)<0)
        {
            perror("prolem while sending\n");           
        }
        
        

        int size=MAX_NAME+sizeof(uint32_t)+sizeof(uint16_t)+4;
        void * response=malloc(size);
        if ( recv(connection_socket,response,size,0)<0)
        {
            perror("prolem while recieving\n");
        }
        
       

        if(strcmp((char *)response,"NOP\0")==0)
        {
            printf("server reply: no max int\n");
        }
        else
        {
            struct in_addr ip;
            ip.s_addr=ntohl(*((uint32_t *)(response+1+3+10)));
            uint16_t max=ntohs(*(uint16_t *)(response+1+3+10+sizeof(uint32_t)));
            char max_psudo[MAX_NAME];
            char * ip_str=inet_ntoa(ip);
            strcpy(max_psudo,(char *)(response+3));
            printf("server reply : max int : %s  %s  %d \n",max_psudo,ip_str,max);
        }

    }
    close(connection_socket);

    return 0;
}
