
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "23150"  // the port users will be connecting to

#define BACKLOG 10     // how many pending connections queue will hold

void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;
    int i;
    int bytes_rec;
    char key[12][10], value[12][10];
	const char s2[2] = " ";
	char val[10];
    struct hostent *h;
    struct in_addr ipadrs;
    h = gethostbyname("nunki.usc.edu");
    ipadrs = *(struct in_addr *)(h->h_addr);

    int port_size;
    struct sockaddr_in portno;

 

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP
    
   FILE *fp;
    char buff[100];
	
    fp = fopen("server3.txt","r");
    if (!fp)
        return 1;

    while(i<4)
    {
        fscanf(fp, "%s %s", key[i], value[i]);   // file reading
        i++;
    }

    int count = 4;
    i=0;
  
    fclose(fp);
	


// beej code
    if ((rv = getaddrinfo("nunki.usc.edu", PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        return 2;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        char val[15];

        inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),s, sizeof s);

       port_size=sizeof(portno);
           if((getpeername(new_fd,(struct sockaddr *)&portno,(socklen_t*)&port_size))==-1)
            {
                perror("getpeername");
            }

    printf("The Server3 has TCP port number %d and IP address %s",PORT,inet_ntoa(ipadrs));

        if (!fork()) { // this is the child process
            close(sockfd); // child doesn't need the listener
            bytes_rec = recv(new_fd, val,9,0);
            printf("The Server3 has received a request with key %s from Server2 with port number %d and IP address %s",val,(unsigned)ntohs(portno.sin_port),inet_ntoa(ipadrs));
            
            val[bytes_rec]='\0';
            
        char *request = (char *) malloc (10);
        strcpy(request, strtok(val, " "));
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
	            if (send(new_fd, msg,12, 0) == -1)  // reply send
                perror("send");
              printf("The Server3 sends the reply %s to the Server2 with port number %d and IP address %s\n",msg,(unsigned)ntohs(portno.sin_port),inet_ntoa(ipadrs));
            close(new_fd);
            exit(0);
          
     }   }
        
        close(new_fd);  // parent doesn't need this
    }

    return 0;
}

