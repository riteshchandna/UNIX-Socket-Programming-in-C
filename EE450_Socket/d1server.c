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
#define MYPORT "21150"    // the port users will be connecting to
#define MAXBUFLEN 100
#define maxsize 6

// get sockaddr , IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

#define SERVER_2_PORT "22150" // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once 


int main(void)
{
      int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes,nbytes;
    struct sockaddr_storage their_addr;
    char buf[MAXBUFLEN];
    socklen_t addr_len,sin_size,udpport_size;
    char s[INET6_ADDRSTRLEN];
    
    socklen_t udpport_size;
    struct sockaddr_in udpport; 
    struct sockaddr_in adr_inet;
    adr_inet.sin_family = AF_INET;
    adr_inet.sin_addr.s_addr = htonl(INADDR_ANY);
    adr_inet.sin_port = 0;


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP
    
    
    int sockfd1, numbytes1;  
    char buf1[MAXDATASIZE];
    struct addrinfo hints1, *servinfo1, *p1;
    int rv1;
    char s1[INET6_ADDRSTRLEN];
    char key[12][10], value[12][10];
    int i=0;
    char cachekey[15], cacheval[15];
   



    memset(&hints1, 0, sizeof hints1);
    hints1.ai_family = AF_UNSPEC;
    hints1.ai_socktype = SOCK_STREAM;
    char* token1 =(char *) malloc (40);
    char* token2=(char *) malloc (40);
 
	/*-----------------FILE READING-------------*/
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
    while(i<4)
    {
        printf("%s %s\n", key[i], value[i]);
        i++;
    }
    fclose(fp);
	/*--------------END OF FILE READING--------------*/

  



    if ((rv = getaddrinfo("nunki.usc.edu", MYPORT, &hints, &servinfo)) != 0) {
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
        fprintf(stderr, "server1: failed to bind socket\n");
        return 2;
    }

    freeaddrinfo(servinfo);
   // printf("server1: waiting to recvfrom...\n");
    addr_len = sizeof their_addr;

      udpport_size=sizeof(udpport);
       if((getsockname(sockfd,(struct sockaddr *)&udpport,&udpport_size))==-1)
       {
        perror("getsockname");
       } 

       //Onscreen message
    

 printf("The Server 1 has UDP port number %d and IP address %s\n",(unsigned)ntohs(udpport.sin_port),getbyhostname("nunki.usc.edu")));

  
   addr_len = sizeof their_addr;
   while(1)
   {
    if ((numbytes = recvfrom(sockfd,token1,9, 0,(struct sockaddr *)&their_addr, &addr_len)) == -1) 
    {
        perror("recvfrom");
        exit(1);
    }
    token1 [9]='\0';
  
  //Onscreen message
	printf("The Server 1 has received a request with %s from client 1 with port number %d and IP address %s\n",token1,(unsigned)ntohs(udpport.sin_port),inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),s, sizeof s));

	 char sendmsg[10];
     strcpy(sendmsg,token1);
     sendmsg[9]='\0';
    char *request = (char *) malloc (10);
        strcpy(request, strtok(token1, " "));
        char *k = (char *) malloc (10);
        strcpy(k, strtok(NULL, " "));

        int flag = 0;
        char temp[10];

        if(!strcmp(request, "GET"))
        {
            for(i = 0; i < count; i++)
            {
                if(!strcmp(k, key[i]))
                {
                    printf("\nThe Server 1 has received a request with key %s which maps to value %s\n", key[i], value[i]);
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
printf("The Server 1 sends the reply %s to Client with port number %d and IP address %s",msg,getbyhostname("nunki.usc.edu"));
  
	if ((nbytes=sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&their_addr,(socklen_t)addr_len)) == -1) {
        perror("client1: sendto");
        exit(1);
    }
    }
    else
    {
    	    if ((rv1 = getaddrinfo("nunki.usc.edu", SERVER_2_PORT, &hints1, &servinfo1)) != 0) {
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
  //  printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure
    
    if (send(sockfd1, sendmsg, 9, 0) == -1)
                perror("send");

            printf("")
    if ((numbytes1 = recv(sockfd1, buf1, 12, 0)) == -1) {
        perror("recv");
        exit(1);
    }

    buf1[numbytes1] = '\0';
   // strcpy(cachevalue,buf1);

    printf("client: received '%s'\n",buf1);
    sendto(sockfd, buf1, 12 , 0, (struct sockaddr *)&their_addr,(socklen_t)addr_len);
    close(sockfd1);
    printf("The Server sends reply %s to Client with port number %d and IP address %s \n",buf1,gethostbyname("nunki.usc.edu"));
    
    	
    }
}

    close(sockfd);
    
    return 0;
}        