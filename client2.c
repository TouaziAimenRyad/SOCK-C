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
    

    int size_adr=sizeof(server_adr);

    if (connect(connection_socket,(struct sockaddr *)&server_adr,(socklen_t)size_adr)==0){

        char *psudo=malloc(MAX_NAME);
        char *response1=malloc(MAX_NAME+6);
        int inc ;

        printf("ENTER PSUDO  :");
        scanf("%10s",psudo);
        if (send(connection_socket,psudo,MAX_NAME,0)<0)
        {
            perror("prolem while sending\n");
            close(connection_socket);
            exit(1);
        }
        
        inc=0;
        while (inc < MAX_NAME+6)
        {
            int r=recv(connection_socket,response1,MAX_NAME+6-inc,0);
            if (r==-1)
                {
                    perror("prolem while receiving\n");
                    close(connection_socket);
                    exit(1);
                }
            inc+=r;
        }
        printf("server reply: %s\n",response1);

        if (send(connection_socket,"MAX",3,0)<0)
        {
            perror("prolem while sending\n");  
            close(connection_socket);
            exit(1);         
        }
        
        
        char * res=malloc(4);
        inc=0;
        while (inc<3)
        {
            int r=recv(connection_socket,res,3-inc,0);
            if (r==-1)
                {
                    perror("prolem while receiving\n");
                    close(connection_socket);
                    exit(1);
                }
            inc+=r;
        }
        res[3]='\0';

        if(strcmp(res,"NOP")==0)
        {
            printf("server reply: no max int\n");
        }
        else
        {
            if (strcmp(res,"REP")==0)
            {
            
                int size=MAX_NAME+sizeof(uint32_t)+sizeof(uint16_t);
                void * response=malloc(size);
                inc=0;
                while (inc<size)
                {
                     int r=recv(connection_socket,response,size-inc,0);
                     if (r==-1)
                     {
                        perror("prolem while receiving\n");
                        close(connection_socket);
                        exit(1);
                     }
                     inc+=r;
                }
                struct in_addr ip;
                ip.s_addr=ntohl(*((uint32_t *)(response+10)));
                uint16_t max=ntohs(*(uint16_t *)(response+10+sizeof(uint32_t)));
                char max_psudo[MAX_NAME];
                char * ip_str=inet_ntoa(ip);
                strcpy(max_psudo,(char *)(response));
                printf("server reply : max int : %s  %s  %d \n",max_psudo,ip_str,max);
                
                free(response);response=NULL;
        
            }
            
        }
        
        
        free(psudo);psudo=NULL;
        free(response1);response1=NULL;
        free(res);res=NULL;
        
    }
    
    close(connection_socket);

    return 0;
}
