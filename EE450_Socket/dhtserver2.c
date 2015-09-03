
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

#define PORT "22150"  // the port users will be connecting to

#define BACKLOG 10     // how many pending connections queue will hold
#define PORTNO3 "23150" // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once 


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
    char buf[15];		
	int comp = 0;
	int flag_found = 0;
    char arr[10];
 
    struct sockaddr_in sin;
    socklen_t len = sizeof(sin);
 
 	char hname[300];
    char ip[300];
    gethostname(hname, sizeof hname);
    int sockfd1, numbytes1;  
    char buf1[MAXDATASIZE];
    struct addrinfo hints1, *servinfo1, *p1;
    int rv1;
    char s1[INET6_ADDRSTRLEN];
    struct hostent *h;
    struct in_addr ipadrs;
    h = gethostbyname("nunki.usc.edu");
    ipadrs = *(struct in_addr *)(h->h_addr);

    int port_size;
    struct sockaddr_in portno;



    memset(&hints1, 0, sizeof hints1);
    hints1.ai_family = AF_UNSPEC;
    hints1.ai_socktype = SOCK_STREAM;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP
    
     char key[12][10], value[12][10];
    int i = 0;
    FILE *fp;

    fp = fopen("server2.txt","r");
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
                   // dynamic port

    port_size=sizeof(portno);
           if((getpeername(new_fd,(struct sockaddr *)&portno,(socklen_t*)&port_size))==-1)
            {
                perror("getpeername");
            }
    printf("Server2 has TCP port number %d and IP address %s\n",PORT,inet_ntoa(ipadrs));

        inet_ntop(their_addr.ss_family,
        get_in_addr((struct sockaddr *)&their_addr),s, sizeof s);

        if (!fork()) { // this is the child process
            close(sockfd); // child doesn't need the listener
    		if ((recv(new_fd, arr, 9, 0)) == -1) {
        		perror("recv");
        		exit(1);
    		}  
    		arr[9]='\0' ;
    


            // beej code ends
    		printf("The Server2 has received a request with key %s from the Server1 with port number and IP address\n",arr,(unsigned)ntohs(portno.sin_port),inet_ntoa(ipadrs));
        
        char sendmsg[9];
        strcpy(sendmsg,arr);
        sendmsg[9]='\0';
        char *request = (char *) malloc (10);
        strcpy(request, strtok(arr, " "));
        char *k = (char *) malloc (10);
        strcpy(k, strtok(NULL, " "));

        int flag = 0;
        char temp[10];

        if(!strcmp(request, "GET"))
        {
            for(i = 0; i < count; i++)   // key-value comparison
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

    		if(flag == 1){     
            if (send(new_fd, msg, 12, 0) == -1)
                perror("send");
                printf("The Server2 sends the reply POST %s to the Server1 with port number and IP address\n",msg,(unsigned)ntohs(portno.sin_port),inet_ntoa(ipadrs));
            	close(new_fd);
            	exit(0);
        }
        else{
                 // beej code starts
     
              if ((rv1 = getaddrinfo("nunki.usc.edu", PORTNO3, &hints1, &servinfo1)) != 0) {
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

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return '2';
    }

    inet_ntop(p1->ai_family, get_in_addr((struct sockaddr *)p1->ai_addr),
            s1, sizeof s1);

    freeaddrinfo(servinfo); // all done with this structure
    
    if (send(sockfd1, sendmsg, 9, 0) == -1)
                perror("send");
            printf("The Server2 sends the request %s to Server3\n",sendmsg);

    if ((numbytes1 = recv(sockfd1, buf1, 12 , 0)) == -1) {
        perror("recv");
        exit(1);
    }

    buf1[numbytes1] = '\0';

    printf("The Server2 received the value %s from Server3 with port number %d and IP address %s\n",buf1,PORTNO3,inet_ntoa(portno.sin_addr));
    printf("The Server2 closed the TCP connection with Server3\n");
    send(new_fd, buf1, 12 , 0); 
    close(sockfd1);
    printf("The Server2 sends the reply POST %s to the Server1 with port number and IP address\n",buf1,(unsigned)ntohs(portno.sin_port),inet_ntoa(ipadrs));
    
    	
        }
        }
        close(new_fd);  // parent doesn't need this
        exit(0);
    }

    return 0;
}

