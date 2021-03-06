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


// get sockaddr , IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

#define PORTNO2 "22150" // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once 

#define userport "21150"    // the port users will be connecting to
#define MAXBUFLEN 100
#define maxsize 10

int main(void)
{
      int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes,nobytes;
    struct sockaddr_storage their_addr;
    char buf[MAXBUFLEN];
    struct hostent *h;
    int rv1;
    char s1[INET6_ADDRSTRLEN];
    char key[12][10], value[12][10];
    int i=0;
    socklen_t addr_len,sin_size;
    char s[INET6_ADDRSTRLEN];

    int sockfd1, numbytes1;  
    char response[MAXDATASIZE];
    struct addrinfo hints1, *servinfo1, *p1;
    
    struct in_addr ipadrs;
    h = gethostbyname("nunki.usc.edu");
    ipadrs = *(struct in_addr *)(h->h_addr);
    socklen_t udpt_size;
    struct sockaddr_in udpt; 
    struct sockaddr_in adr_inet;
    adr_inet.sin_family = AF_INET;
    adr_inet.sin_addr.s_addr = htonl(INADDR_ANY);
    adr_inet.sin_port = 0;

    int port_size;
    struct sockaddr_in portno;


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP
    memset(&hints1, 0, sizeof hints1);
    hints1.ai_family = AF_UNSPEC;
    hints1.ai_socktype = SOCK_STREAM;
    char* getbuf=(char *) malloc (20);
   

    FILE *fp;
    fp = fopen("server1.txt","r");
    if (!fp)
        return 1;

    while(i<4)
    {
        fscanf(fp, "%s %s", key[i], value[i]);
        i++;
    }

    int count = 4;
    i=0;
   
    fclose(fp);

  

       //beej code
    if ((rv = getaddrinfo("nunki.usc.edu", userport, &hints, &servinfo)) != 0) {
        fprintf(stderr, " getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) {
            perror("server1: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server1: bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "server1: failed to bind socket\n");  //  beej code
        return 2;
    }

    freeaddrinfo(servinfo);

    addr_len = sizeof their_addr;

    
   
  // dynamic udp port
     udpt_size=sizeof(udpt);
       if((getsockname(sockfd,(struct sockaddr *)&udpt,&udpt_size))==-1)
       {
        perror("getsockname");
       } 

    printf("The Server 1 has UDP port number %d and IP address %s\n",(unsigned)ntohs(udpt.sin_port),inet_ntoa(ipadrs));

  
   while(1)
   {  

    if ((numbytes = recvfrom(sockfd,getbuf,9, 0,(struct sockaddr *)&their_addr, &addr_len)) == -1) 
    {
        perror("recvfrom");
        exit(1);
    }
    getbuf [9]='\0';
    
      
  
    printf("The Server1 has received a request with %s from Client1 with port number %d and IP address %s\n",getbuf,(int)ntohs(((struct sockaddr_in *)(&their_addr))->sin_port),inet_ntoa(ipadrs));
    char sendmsg[15];
     strcpy(sendmsg,getbuf);
     char *request = (char *) malloc (10);
        strcpy(request, strtok(getbuf, " "));
        char *k = (char *) malloc (10);
        strcpy(k, strtok(NULL, " "));

        int flag = 0;
        char temp[10];

        if(!strcmp(request, "GET"))
        {
            for(i = 0; i < count; i++)    // string comparison
            {
                if(!strcmp(k, key[i]))
                {
                  
                    flag = 1;
                    strcpy(temp,value[i]);
                    break;
                }
            }
        }    
     char msg[15];
     strcpy(msg,"POST ");
     strcat(msg, temp);  

            
    if(flag==1)
    {
    printf("The Server 1 sends the reply %s to Client with port number %d and IP address %s.",temp,(int)ntohs(((struct sockaddr_in *)(&their_addr))->sin_port),inet_ntoa(ipadrs));
  
    if ((nobytes=sendto(sockfd,msg, strlen(msg), 0, (struct sockaddr *)&their_addr,(socklen_t)addr_len)) == -1) {
        perror("client1: sendto");
        exit(1);
    }
    }
    else
    {       // beej code
        
            if ((rv1 = getaddrinfo("nunki.usc.edu", PORTNO2, &hints1, &servinfo1)) != 0) {  // server 2 connection
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv1));
        return '1';
    }

    // loop through all the results and connect to the first we can
    for(p1 = servinfo1; p1 != NULL; p1 = p1->ai_next) {
        if ((sockfd1= socket(p1->ai_family, p1->ai_socktype,
                p1->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd1, p1->ai_addr, p1->ai_addrlen) == -1) {
            close(sockfd1);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p1 == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return '2';
    }

    inet_ntop(p1->ai_family, get_in_addr((struct sockaddr *)p1->ai_addr),
            s1, sizeof s1);

    freeaddrinfo(servinfo1); // all done with this structure
    
    if (send(sockfd1, sendmsg, 9, 0) == -1)
                perror("send");
             if((getsockname(sockfd,(struct sockaddr *)&portno,(socklen_t*)&port_size))==-1)  //to find dynamic port no(beej code)
       {
        perror("getsockname");
       }

    printf("The Server1 sends the request %s to the Server2\n",sendmsg);
    printf("The TCP port number is %d and IP address is %s\n",(unsigned)ntohs(portno.sin_port),inet_ntoa(portno.sin_addr));

    if ((numbytes1 = recv(sockfd1, response, 12, 0)) == -1) {      // receiving request
        perror("recv");
        exit(1);
    }

    response[numbytes1] = '\0';

    printf("The Server 1 received the value %s from the Server2 with port number %d and IP address  %s\n",response,PORTNO2,inet_ntoa(portno.sin_addr));
    printf("The Server1 closed the TCP connection with Server2\n");
    sendto(sockfd, response, 12 , 0, (struct sockaddr *)&their_addr,(socklen_t)addr_len);
    close(sockfd1);
    printf("The Server 1 sends the reply %s to Client with port number %d and IP address %s \n",response,(int)ntohs(((struct sockaddr_in *)(&their_addr))->sin_port),inet_ntoa(ipadrs));
    
        
    }
}

    close(sockfd);
    
    return 0;
}  



      