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
#define MAX_NAME 10

int main(int argc, char const *argv[])
{ 
    assert(argc==2);
    int connection_socket=socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in server_adr;
    char * adr_ip="127.0.0.1";//change this based on lulu in ordre to connect from other networks 
    server_adr.sin_family=AF_INET;
    server_adr.sin_port=htons((uint16_t)atoi(argv[1]));
    inet_aton(adr_ip,&server_adr.sin_addr);
    int size=sizeof(server_adr);

    if (connect(connection_socket,(struct sockaddr *)&server_adr,(socklen_t)size)==0){
    
        char *psudo=malloc(MAX_NAME);
        char *response1=malloc(16);
      
        printf("ENTER PSUDO  :");
        scanf("%10s",psudo);
        send(connection_socket,psudo,MAX_NAME,0);
        recv(connection_socket,response1,MAX_NAME+6,0);
        printf("server: %s\n",response1);

        send(connection_socket,"MAX\0",4,0);
        int size=MAX_NAME+sizeof(uint32_t)+sizeof(uint16_t)+4;
        void * response=malloc(size);
        recv(connection_socket,response,size,0);
        if(strcmp((char *)response,"NOP\0")==0)
        {
            printf("server: no max int\n");
        }
        else
        {
            struct in_addr ip;
            ip.s_addr=ntohl(*((uint32_t *)(response+1+3+10)));
            uint16_t max=ntohs(*(uint16_t *)(response+1+3+10+sizeof(uint32_t)));
            char max_psudo[MAX_NAME];
            char * ip_str=inet_ntoa(ip);
            strcpy(max_psudo,(char *)(response+3));
            printf("server : max int : %s  %s  %d \n",max_psudo,ip_str,max);
        }
        //print the response don't forget the uint16 will be in big endian

    }

    return 0;
}
