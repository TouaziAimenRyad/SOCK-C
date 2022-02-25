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
#define  MAX_NAME 10


int main(int argc, char const *argv[])
{   
    assert(argc==2);
    struct sockaddr_in server_adr;
    char * adr_ip="194.254.199.32";//change this based on lulu in ordre to connect from other networks 
    server_adr.sin_family=AF_INET;
    server_adr.sin_port=htons((uint16_t)atoi(argv[1]));
    inet_aton(adr_ip,&server_adr.sin_addr);
    int size=sizeof(server_adr);

    for (int i = 0; i < 5;  i++)
    {
        int connection_socket=socket(PF_INET,SOCK_STREAM,0);

        if (connect(connection_socket,(struct sockaddr *)&server_adr,(socklen_t)size)==0)
        {
        
            char *psudo=malloc(MAX_NAME);
            char *response1=malloc(16);
            char *responseOK=malloc(6);
            int n;
            printf("ENTER PSUDO%d  :",i);
            scanf("%10s",psudo);
            send(connection_socket,psudo,MAX_NAME,0);
            recv(connection_socket,response1,MAX_NAME+6,0);
            printf("server: %s\n",response1);
    
            printf("ENTER NUMBER%d : ",i);
            scanf("%d",&n);
            int size=4+sizeof(uint16_t);
    
            void * message=malloc(size);
            uint16_t num = htons((uint16_t)n);
            strcpy((char *)(message),"INT ");
            *((uint16_t *)(message+4))=num;
            send(connection_socket,message,4+sizeof(uint16_t),0);
    
            recv(connection_socket,responseOK,6,0);
            printf("server: %s\n",responseOK);
    
            free(message);message=NULL;
            free(psudo);psudo=NULL;
            free(response1);response1=NULL;
            free(responseOK);responseOK=NULL;

        }
           close(connection_socket);
    }
    

    


    return 0;
}
