#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "21150"    // the port users will be connecting to
#define maxsize 13
int main(int argc, char *argv[])
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    socklen_t addrlen;
    int numbytes,nbytes;
     struct sockaddr_storage their_addr;
socklen_t addr_len;
socklen_t udpt_size;
      struct sockaddr_in udpt;
       int port_size;
      struct sockaddr_in portno;
      
  struct sockaddr_in adr_inet;
       adr_inet.sin_port;

        

     struct hostent *h;
    struct in_addr ipadrs;
    h = gethostbyname("nunki.usc.edu");
    ipadrs = *(struct in_addr *)(h->h_addr);
    if (argc != 3) {
        fprintf(stderr,"Please Enter Your Search (USC, UCLA etc.):\n");
        exit(1);
    }

    FILE *fp;
    char key[12][10], value[12][10];
    int i = 0;
    fp=NULL;
    fp = fopen("client2.txt","r");
    if (!fp)
        return 1;

    while(i<12)
    {
        fscanf(fp, "%s %s", key[i], value[i]);
        i++;
    }
  
    fclose(fp);


 char *val1;
 char*token;
 char val2[maxsize];
const char s2[2] = " ";

 char msg[15];
    strcpy(msg,"GET ");
    int flag= 0;
    for(i = 0; i < 12; i++)
    {
        if(!strcmp(argv[2], key[i]))
        {
            printf("\nThe client 2 has received a request with search word  %s, which maps to %s", key[i], value[i]);
            strcat(msg, value[i]);
            flag = 1;
            break;
        }
    }



    // beej code starts
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client2: socket");
            continue;
        }
        bind(sockfd,(struct sockaddr *)&adr_inet,sizeof(adr_inet));

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client2: failed to bind socket\n");
        return 2;
    }

    if ((numbytes = sendto(sockfd, msg,9, 0,
             p->ai_addr, p->ai_addrlen)) == -1) {
        perror("client2: sendto");
        exit(1);
    }
     udpt_size=sizeof(udpt);       // to get dynamic port
       if((getsockname(sockfd,(struct sockaddr *)&udpt,&udpt_size))==-1)
       {
        perror("getsockname");
       }

    freeaddrinfo(servinfo);
    // beej code ends

    printf("The client2 sends the request %s to the Server1 with port number %d and IP address %s.\n",msg,PORT,inet_ntoa(ipadrs));
   
    printf("The client2's port number is %d and the IP address is %s \n",(unsigned)ntohs(udpt.sin_port),inet_ntoa(ipadrs));
    //addr_len = sizeof their_addr;
    if ((numbytes = recvfrom(sockfd,val2,maxsize-1, 0,p->ai_addr,&addrlen)) == -1) 
    {
        perror("recvfrom");
        exit(1);
    }
  printf("The client2 received the value %s from the Server1 with port number %d and IP address %s.\n",val2,PORT,inet_ntoa(ipadrs));
  printf("The client2's port number is %d and the IP address is %s\n ",(unsigned)ntohs(udpt.sin_port),inet_ntoa(ipadrs));

    return 0;
}

