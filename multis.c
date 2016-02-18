/*
** selectserver.c -- a cheezy multiperson chat server
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <ifaddrs.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "3490"   // port we're listening on
#define DEBUG 1 //0 TO NOT INCLUDE DEBUG CODE

void timeis(char* timestamp){
        char *strgtime;
         time_t timenow;
         time (&timenow);
         struct tm * timeinfo;
               time(&timenow);
         timeinfo= localtime(&timenow);
         strftime(strgtime,80,"%a %b %d %T %G ", timeinfo);
         sprintf(timestamp,"%s",strgtime);
       
}


//Assign files to clients

int sche_FCFS(FILE * configfile, char *line , int* fd_soc, fd_set rfds, int fds, char clientadd ){
 
  fd_set writefd;
  FD_ZERO(&writefd);
  writefd=rfds;

  int selectres;
  int compare;
  int response;
  char buf[1100];
  int resread=0;
  int j, lenstr;
  char times[80];
  char logmsm[1030];
  char *path_name;
  char *path_out;
  path_name=strtok(line," ");
  path_out=strtok(NULL, "\n");
   #if(DEBUG)
    printf("RR %s\n",buf );
    printf("clienadd inrr  %d\n",clientadd );
  #endif

         
        
               
       if (selectres=(select(fds+1, &writefd, NULL,NULL,NULL))  ==-1){
          return -1;
        };
        if(selectres == 0){
          return sche_FCFS(configfile, line,fd_soc, rfds, fds, clientadd);
        };

             

           for(j=0;j<fds;j++){

            if(FD_ISSET(j, &writefd )){
              resread=read(j,buf, sizeof(buf));
               #if(DEBUG)
                      printf("RR %s\n",buf );
                #endif
              resread= strncmp(buf,"FREE", 4);
              if(resread == 0){
               timeis(times);
    sprintf(logmsm,"[%s] The lyrebird.client %d has been given the task of decrypting %s \n", times,clientadd, path_name);
                 lenstr=strlen(logmsm);
                 send(j,line,2050,0);
                 fwrite(logmsm,sizeof(char), lenstr, configfile);

              }else{
                //CHECK IF CLIENT FINISHED
                resread= strncmp(buf,"CLOSING", 7);
                 if(resread==0){
                     timeis(times);
                    sprintf(logmsm,"[%s]  lyrebird.client %d has disconnected expectedly \n", times,clientadd);
                 lenstr=strlen(logmsm);
                 send(j,line,2050,0);
                 fwrite(logmsm,sizeof(char), lenstr, configfile);
                    close(j);
                 }
                
              }

            }
           }
          return 1;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa )
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void * addr_to_print(int sa, char * ret)
{
    char ipstr[INET6_ADDRSTRLEN+1];
    
   inet_ntop(AF_INET, get_in_addr((struct sockaddr*)&sa),
                                ipstr, INET6_ADDRSTRLEN);
   sprintf(ret, "%s",ipstr);
   return ret;
 
}



char *getaddres(char *addreturn)
       {
            struct ifaddrs *ifaddr, *ifa;
           int family, s, comp;
           char host[NI_MAXHOST];
           char *ifaa;
           

           if (getifaddrs(&ifaddr) == -1) {
               perror("getifaddrs");
               exit(EXIT_FAILURE);
           }

          

           for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
               if (ifa->ifa_addr == NULL)
                   continue;

               family = ifa->ifa_addr->sa_family;

               
               ifaa=ifa->ifa_name;
               comp=strncmp(ifaa,"em", 2);

               if (family == AF_INET && comp == 0 ) {
                   s = getnameinfo(ifa->ifa_addr,
                           (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                                 sizeof(struct sockaddr_in6),
                           host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
                   if (s != 0) {
                       printf("getnameinfo() failed: %s\n", gai_strerror(s));
                       exit(EXIT_FAILURE);
                    }
                   printf("address: <%s>, %s\n", host,  ifa->ifa_name );
                   addreturn=host;
                   sprintf(addreturn, "%s",host);
                   
               }
           }

           freeifaddrs(ifaddr);
           return addreturn;
       }

int main(int argc , char *argv[])
{
    FILE * ifile;
    FILE * logfile;

    char *ifile_arr=argv[1];
    char *filelogarr=argv[2];

    ifile= fopen(ifile_arr, "r+");
    logfile= fopen(filelogarr, "a+");

    if(ifile== NULL || logfile == NULL){printf("ERROR: fthere is not sufficient parameters\n"); exit(9);};

    char times[80];   //for time stamp
    char logmsm[2050];
    char line[2050];
    char hostadd[NI_MAXHOST];
    char clientadd[NI_MAXHOST];
    fd_set master;    // master file descriptor list
    fd_set read_fds;  // temp file descriptor list for select()
    int fdmax;        // maximum file descriptor number
    int sockets, lenstr;     //////////////*******************SOCKETNUM

    int listener;     // listening socket descriptor
    int newfd;        // newly accept()ed socket descriptor
    struct sockaddr_storage remoteaddr; // client address
    socklen_t addrlen;

    char buf[2085];    // buffer for client data
    int nbytes;

    char remoteIP[INET6_ADDRSTRLEN];

    int yes=1;        // for setsockopt() SO_REUSEADDR, below
    int i, j, rv, pid;

    struct addrinfo hints, *ai, *p;

    FD_ZERO(&master);    // clear the master and temp sets
    FD_ZERO(&read_fds);

    // get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
    }
    
    for(p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
       #if(DEBUG)
        printf("sockests: %d\n", sockets);
        #endif
        sockets++;
        if (listener < 0) { 
            continue;
        }
        
        // lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }

        break;
    }

    // if we got here, it means we didn't get bound
    if (p == NULL) {
        fprintf(stderr, "selectserver: failed to bind\n");
        exit(2);
    }

    freeaddrinfo(ai); // all done with this

    // listen
    if (listen(listener, 10) == -1) {
        perror("listen");
        exit(3);
    }

    // add the listener to the master set
    FD_SET(listener, &master);

    // keep track of the biggest file descriptor
    fdmax = listener; // so far, it's this one
     pid=(int)getpid();
     #if(DEBUG)
        printf("BEFOR TIMES: \n");
        #endif
     timeis(times);
     getaddres(hostadd);
     #if(DEBUG)
        printf("WRITE SERVER IP TO LOG: \n");
        #endif
     sprintf(logmsm,"[%s] lyrebird.server: PID %d on host %s, port %s ", times, pid,  getaddres(hostadd), PORT);
     lenstr=strlen(logmsm);
     fwrite(logmsm,sizeof(char), lenstr, logfile);
     





    // main loop
    for(;;) {
        read_fds = master; // copy it
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }
        #if(DEBUG)
        printf("MAIN LOOP\n");
        #endif

        // run through the existing connections looking for data to read
        for(i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) { // we got one!!
                if (i == listener) {
                    // handle new connections
                    addrlen = sizeof remoteaddr;
                    #if(DEBUG)
                    printf("ACCEPTING CALLS: \n");
                    #endif
                    newfd = accept(listener,
                        (struct sockaddr *)&remoteaddr,
                        &addrlen);

                    if (newfd == -1) {
                        perror("accept");
                    } else {
                        FD_SET(newfd, &master); // add to master set
                        if (newfd > fdmax) {    // keep track of the max
                            fdmax = newfd;
                        }
                     
                            sprintf(logmsm,"[%s] lyrebird.server: PID %d on host %s, port %s \n", times, pid,  getaddres(hostadd), PORT);
                            lenstr=strlen(logmsm);
                            fwrite(logmsm,sizeof(char), lenstr, logfile);
                            #if(DEBUG)
                                printf("selectserver: new connection from %s on "
                                "socket %d\n",
                                inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr*)&remoteaddr),
                                    remoteIP, INET6_ADDRSTRLEN),
                                newfd);
                              
                            #endif
                              
                    }
                } else {
                    // h

                    fgets(line, 2050, ifile);
                    read_fds=master;
                   // hostadd=inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr*)&remoteaddr),
                   //                 remoteIP, INET6_ADDRSTRLEN);
                    #if(DEBUG)
                    addr_to_print(newfd, hostadd);
                    printf("clientadd: %s\n", hostadd);
                    break;
                    #endif

                    sche_FCFS(logfile, line , newfd, read_fds, fdmax, 
                        inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr*)&remoteaddr),
                                    remoteIP, INET6_ADDRSTRLEN));
                    if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
                        // got error or connection closed by client
                        if (nbytes == 0) {
                            // connection closed
                           


                           
                           
                        } else {
                            perror("recv");
                        }
                        
                    } else {
                        // we got some data from a client
                        for(j = 0; j <= fdmax; j++) {
                            // send to everyone!
                            if (FD_ISSET(j, &master)) {
                                // except the listener and ourselves
                                if (j != listener && j != i) {
                                    if (send(j, buf, nbytes, 0) == -1) {
                                        perror("send");
                                    }
                                }
                            }
                        }
                    }
                } // END handle data from client
            } // END got new incoming connection
        } // END looping through file descriptors
        if(feof(ifile)){break;}
    } // END for(;;)--and you thought it would never end!
    fclose(logfile);
    return 0;
}