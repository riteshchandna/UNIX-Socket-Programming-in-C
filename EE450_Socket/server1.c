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

#define MYPORT "21150" 
#define MYPORT2 "22150"   // the port users will be connecting to

#define MAXBUFLEN 100
#define MAXDATASIZE 100 // max number of bytes we can get at once 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main()
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes,sentbytes;
    struct sockaddr_storage their_addr;
    char buf[MAXBUFLEN];
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];


    int sockfd_tcp;
    struct addrinfo hints_tcp, *servinfo_tcp, *p_tcp;
    int rv_tcp;
    int numbytes_tcp,sentbytes_tcp;
    struct sockaddr_storage their_addr;
    char buf_tcp[20];
    socklen_t addr_len;
    char s_tcp[INET6_ADDRSTRLEN];
    
    
    socklen_t udpport_size;
    struct sockaddr_in udpport; 
    struct sockaddr_in adr_inet;
    adr_inet.sin_family = AF_INET;
    adr_inet.sin_addr.s_addr = htonl(INADDR_ANY);
    adr_inet.sin_port = 0;
    char key[12][10], value[12][10];
    int i = 0;
    FILE *fp;


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    memset(&hints_tcp, 0, sizeof hints_tcp);
    hints_tcp.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints_tcp.ai_socktype = SOCK_STREAM;

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
    //return 0;


    
 

    if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("listener: bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        return 2;
    }

    freeaddrinfo(servinfo);
    
    printf("listener: waiting to recvfrom...\n");

    addr_len = sizeof their_addr;
    if ((numbytes = recvfrom(sockfd, buf,9 , 0,
        (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        perror("recvfrom");
        exit(1);
    }

    printf("listener: got packet from %s\n",
        inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),s, sizeof s));
    printf("listener: packet is %d bytes long\n", numbytes);
    buf[numbytes] = '\0';
    printf("listener: packet contains \"%s\"\n", buf);
  
    char *request = (char *) malloc (10);
        strcpy(request, strtok(buf, " "));
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

          /*  if(!flag)
            {
                printf("\nNo such Key exists.");
                exit(1);
            } */
        }
        temp[7]='\0';

   if ((sentbytes = sendto(sockfd,temp, strlen(temp), 0,
             (struct sockaddr *)&their_addr,(socklen_t)addr_len)) == -1) {
        perror("talker: sendto");
        exit(1);
    }

    close(sockfd);
    if(!flag)
    {

    if ((rv_tcp = getaddrinfo("nunki.usc.edu", MYPORT2, &hints_tcp, &servinfo_tcp)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv_tcp));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p_tcp = servinfo_tcp; p_tcp != NULL; p_tcp = p_tcp->ai_next) {
        if ((sockfd_tcp = socket(p_tcp->ai_family, p_tcp->ai_socktype,
                p_tcp->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd_tcp, p_tcp->ai_addr, p_tcp->ai_addrlen) == -1) {
            close(sockfd_tcp);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p_tcp == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p_tcp->ai_family, get_in_addr((struct sockaddr *)p_tcp->ai_addr),
            s_tcp, sizeof s_tcp);
    printf("client: connecting to %s\n", s_tcp);

    freeaddrinfo(servinfo_tcp); // all done with this structure

    if ((numbytes_tcp = send(sockfd_tcp, "helloworl", 9, 0)) == -1) {
        perror("recv");
        exit(1);
    }

  //  buf[numbytes_tcp] = '\0';

    //printf("client: received '%s'\n",buf_tcp);

    close(sockfd_tcp);

    return 0;
}
