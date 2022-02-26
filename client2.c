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
    server=gethostbyname("lucy.informatique.univ-paris-diderot.fr"); 
    if(server==NULL)
    {
        perror("couldn't find the host\n");
        exit(1);
    }
    
    struct in_addr **addresses=(struct in_addr**)server->h_addr_list;
    server_adr.sin_family=AF_INET;
    server_adr.sin_port=htons((uint16_t)atoi(argv[1]));
    server_adr.sin_addr=(**(addresses));

    printf("Address : %s\n",inet_ntoa(**addresses));
    printf("Address2 : %s\n",inet_ntoa(server_adr.sin_addr));

    int size=sizeof(server_adr);

    if (connect(connection_socket,(struct sockaddr *)&server_adr,(socklen_t)size)==0){
        printf("fghjkjhgf");
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

    }else{printf("hhhhhhhhhhhhhhhhh");}
    close(connection_socket);

    return 0;
}
