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

#define SERVERPORT "21150"  // the port users will be connecting to 
#define size 15

int main(int argc, char *argv[])
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes,recbytes;
    char recval[size];
    socklen_t addrlen;

    if (argc != 3) {
        fprintf(stderr,"usage: talker hostname message\n");
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
    /*
    i=0;
    while(i<12)
    {
        printf("%s %s\n", key[i], value[i]);
        i++;
    }
    */

    fclose(fp);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0) 
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) 
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) 
        {
            perror("talker: socket");
            continue;
        }

        break;
    }

    if (p == NULL) 
    {
        fprintf(stderr, "talker: failed to bind socket\n");
        return 2;
    }
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

    if(!flag)
    {
        printf("\nNo such Key exists.");
        exit(1);
    }


    if ((numbytes = sendto(sockfd, msg, strlen(msg), 0, p->ai_addr, p->ai_addrlen)) == -1) 
    {
        perror("talker: sendto");
        exit(1);
    }
    freeaddrinfo(servinfo);

    if ((recbytes = recvfrom(sockfd, recval, size-1, 0, p->ai_addr, &addrlen)) == -1) 
    {
        perror("recvfrom");
        exit(1);
    }
    recval[recbytes] = '\0';
    printf("\n%s\n",recval );

    close(sockfd);

    return 0;
}