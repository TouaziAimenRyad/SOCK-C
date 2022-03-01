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
#define  MAX_NAME 10


int main(int argc, char const *argv[])
{   
    assert(argc==2);
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
    server_adr.sin_addr=(**addresses);
    

    int size=sizeof(server_adr);

    for (int i = 0; i < 5;  i++)
    {
        int connection_socket=socket(PF_INET,SOCK_STREAM,0);
    
        if (connect(connection_socket,(struct sockaddr *)&server_adr,(socklen_t)size)==0)
        {
    
        
            char *psudo=malloc(MAX_NAME);
            char *response1=malloc(16);
            char *responseOK=malloc(6);
            uint16_t n;
            printf("ENTER PSUDO%d  :",i);
            scanf("%10s",psudo);
            if (send(connection_socket,psudo,MAX_NAME,0)<0)
            {
                perror("prolem while sending\n");
            }
            
            if (recv(connection_socket,response1,MAX_NAME+6,0)<0)
            {
                perror("prolem while receiving\n");
            }
            printf("server reply : %s\n",response1);
    
            printf("ENTER NUMBER%d : ",i);
            scanf("%hd",&n);
    
            void * message=malloc(4+sizeof(uint16_t));
            uint16_t num = htons((uint16_t)n);
            strcpy((char *)(message),"INT\0");
            *((uint16_t *)(message+4))=num;
            if (send(connection_socket,message,4+sizeof(uint16_t),0)<0)
            {
                perror("prolem while sending\n");
            }
            
            if (recv(connection_socket,responseOK,6,0)<0)
            {
                perror("prolem while receiving\n");
            }
            
            
            printf("server reply : %s\n",responseOK);
    
            free(message);message=NULL;
            free(psudo);psudo=NULL;
            free(response1);response1=NULL;
            free(responseOK);responseOK=NULL;

        }
           close(connection_socket);
    }
    

    


    return 0;
}
