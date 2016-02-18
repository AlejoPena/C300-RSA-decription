/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include "principal.h"

#define DEBUG 1 //0 TO NOT INCLUDE DEBUG CODE
#define PORT "3490" // the port client will be connecting to 

#define MAXDATASIZE 2050 // max number of bytes we can get at once 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
    int sockfd, numbytes;  
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
        #if(DEBUG)
          
        printf("client: connecting to %s\n", s);
        #endif

    freeaddrinfo(servinfo); // all done with this structure

    fd_set sock;
    fd_set temp;

    FD_ZERO(&sock);
    FD_SET(sockfd, &sock);

    int selctrev;
    int sent;
    int loops=5;
    int i=0;
    while(1){ 
                        #if(DEBUG)
                          printf("main loop\n");
                        #endif

        temp=sock;
        selctrev= select(sockfd+1, &temp, NULL, NULL,NULL);
                        #if(DEBUG)
                          printf("after select\n");
                        #endif
        memset(buf,0, 1099);
                        #if(DEBUG)
                          printf("SENDING FREE\n");
                        #endif
        sent= send(sockfd, "FREE", MAXDATASIZE-1,MSG_DONTWAIT);

        if(FD_ISSET(sockfd,&sock)){ 
                            #if(DEBUG)
                          printf("RECIVING FROM SERV\n");
                        #endif

            if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
                perror("recv");
            }
        }else{
                            #if(DEBUG)
                          printf("SOCK NOT IN IFSSET CONTINUE \n");
                        #endif

            continue;
        };
        //printf("recv: %s\n", buf );

        
                        #if(DEBUG)
                          printf("COMPARING CLOSE\n");
                        #endif
        sent= strncmp(buf, "CLOSE", 5);
        if(sent == 0){
            send(sockfd, "CLOSING",7,0);
                        #if(DEBUG)
                         printf("close sig from server\n");
                        #endif
             break;
            i=1;
        }

        printf("client: received '%s'\n",buf);
        continue;
    }

    close(sockfd);

    return 0;
    exit(1);
}