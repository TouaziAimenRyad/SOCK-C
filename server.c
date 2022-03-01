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
#define MAX_ELEM 20

pthread_mutex_t verrou=PTHREAD_MUTEX_INITIALIZER ;
struct caller
{
    int * sock;
    struct sockaddr_in * caller_adr;

};

struct data
{
    uint32_t ip;
    uint16_t num;
    char name[MAX_NAME]; // problems with the name
};

struct data store[MAX_ELEM];
int num_insered=0;



struct data largest(struct data arr[], int n);
void * maxint(void * s);

int main(int argc, char const *argv[])
{
    assert(argc==2);
    int server_socket=socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in server_adr;
    server_adr.sin_family=AF_INET;
    server_adr.sin_port=htons((uint16_t)atoi(argv[1]));
    server_adr.sin_addr.s_addr=htonl(INADDR_ANY); // INADDR_ANY USED WHEN WE DON'T KNOW THE ADR OF OUR MACHINE IT TAKES IT AUTOMATICALLY
    
    if(bind(server_socket,(struct sockaddr*)&server_adr,sizeof(server_adr))==0)
    {
        if (listen(server_socket,0)==0)
        { 
           while (1)
           {
                struct sockaddr_in client_addr;
                int size =sizeof(struct sockaddr_in);
                
                while (1)
                {
                    int *client_socket=malloc(sizeof(int));
                    *client_socket=accept(server_socket,(struct sockaddr *)&client_addr,(socklen_t *)&size);
                    
                    if (*client_socket>=0)
                    {
                        struct caller *client=malloc(sizeof(struct caller));
                        client->caller_adr=&client_addr;
                        client->sock=client_socket;
                        pthread_t th;
                        pthread_create(&th,NULL,maxint,client);
                        //maxint((void *)client);
                    }
                }
           }
           
            
        }
    }
    return 0;
}



void * maxint(void * s)
{
   struct caller  client= *((struct caller *)s);
   int  client_sock=*client.sock;
   
   char psudo[MAX_NAME];
   int client_mesg_size=3+1+sizeof(uint16_t);

   if (recv(client_sock,psudo,MAX_NAME,0)<0)
   {
       perror("prolem while recieving\n");
   }
   char * save_buff=malloc(MAX_NAME);
   strcpy(save_buff,psudo);//this is used to save the name i need it cause for some reason the psudo is gettin emptied some how after receving the msg  
   printf("%s request connection as a client\n",psudo);

   char hello[6+MAX_NAME]="HELLO ";
   strcat(hello,psudo);
   if (send(client_sock,hello,MAX_NAME+6,0)<0)
   {
      perror("prolem while sending\n");
   }
   
   void* client_message=malloc(client_mesg_size);
   if (recv(client_sock,client_message,client_mesg_size,0)<0)
   {
       perror("prolem while recieving\n");
   }
   
   char request[4];
   strcpy(request,(char*)client_message);
 

  if(strcmp(request,"MAX\0")==0)
  {
     pthread_mutex_lock(&verrou);
     if (num_insered>0)
     {
       struct data max=largest(store,num_insered);

       int max_message_size=sizeof(uint16_t)+sizeof(uint32_t)+3+10+1;
       void * message_max=malloc(max_message_size);

       char rep[MAX_NAME+3+1]="REP";
       strcat(rep,max.name);
       strcat(rep,"\0");

       strcpy((char *)message_max ,rep);
       *((uint32_t *)(message_max+3+10+1))=htonl(max.ip);
       *((uint16_t *)(message_max+10+3+1+sizeof(uint32_t)))=htons(max.num);
       
       if (send(client_sock,message_max,max_message_size,0)<0)
       {
            perror("prolem while sending\n");
       }
       
       

     }
     else
     { 
         if (send(client_sock,"NOP\0",4,0)<0)
         {
        
           perror("prolem while sending\n");

         }
     
       
     }
     
     pthread_mutex_unlock(&verrou);
  }



  if (strcmp(request,"INT ")==0)
  {
      uint16_t num=(*(uint16_t *)(client_message+4));
      if (send(client_sock,"INTOK\0",6,0)<0)
      {
          perror("prolem while sending\n");
      }
      
      struct data x;
      x.ip=client.caller_adr->sin_addr.s_addr;
      strcpy(x.name,save_buff); 
      x.num=ntohs(*((uint16_t *)(client_message+4)));
      
      pthread_mutex_lock(&verrou);
      if (num_insered<MAX_ELEM)
      {
         store[num_insered]=x;
         num_insered++;
      }
      pthread_mutex_unlock(&verrou);
      free(client_message);client_message=NULL;
  }

  free(save_buff);save_buff=NULL;

    
   
    return NULL;

}



struct data largest(struct data arr[], int n)
{
    int i;
    
    struct data reslt=arr[0];
    uint16_t max = arr[0].num;
 
    
    for (i = 1; i < n; i++)
    {
       if (arr[i].num > max)
        {
            max = arr[i].num;
            reslt=arr[i];
        }
    }
 
           
 
    return reslt;
}