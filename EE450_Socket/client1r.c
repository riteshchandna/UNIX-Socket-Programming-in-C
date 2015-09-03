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

#define SERVERPORT "21159"    // the port users will be connecting to
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
    if (argc != 3) {
        fprintf(stderr,"Please Enter Your Search (USC, UCLA etc.):\n");
        exit(1);
    }

    FILE *fp;
    char key[12][10], value[12][10];
    int i = 0;
    fp=NULL;
    fp = fopen("client1.txt","r");
    if (!fp)
        return 1;

    while(i<12)
    {
        fscanf(fp, "%s %s", key[i], value[i]);
        i++;
    }
  
    fclose(fp);


 char *token1;
 char*token;
 char token2[maxsize];
const char s2[2] = " ";

 char msg[15];
    strcpy(msg,"GET ");
    int flag= 0;
    for(i = 0; i < 12; i++)
    {
        if(!strcmp(argv[2], key[i]))
        {
            printf("\nThe Client 1 has received a request with search word  %s, which maps to %s", key[i], value[i]);
            strcat(msg, value[i]);
            flag = 1;
            break;
        }
    }
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client1: socket");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client1: failed to bind socket\n");
        return 2;
    }

    if ((numbytes = sendto(sockfd, msg,9, 0,
             p->ai_addr, p->ai_addrlen)) == -1) {
        perror("client1: sendto");
        exit(1);
    }

    freeaddrinfo(servinfo);

    printf("The Client 1 sends the request %s to the Server 1 with port number _____ and IP address ____.",msg);
    printf("\nThe Client1’s port number is _______ and the IP address is _______.");
    //addr_len = sizeof their_addr;
    if ((numbytes = recvfrom(sockfd,token2,maxsize-1, 0,p->ai_addr,&addrlen)) == -1) 
    socket        perror("recvfrom");
        exit(1);
    }

    printf("\nThe Client1 received the value %s from the Server 1 with port number _____and IP address____.",token2);
     printf("\nThe Client1 has TCP port number %d and the IP address is __.");
    close(sockfd);

    return 0;
}