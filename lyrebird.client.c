#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <ifaddrs.h>
#include <linux/if_link.h>
#include <netdb.h>
 
   int getaddres()
       {
            struct ifaddrs *ifaddr, *ifa;
           int family, s;
           char host[NI_MAXHOST];

           if (getifaddrs(&ifaddr) == -1) {
               perror("getifaddrs");
               exit(EXIT_FAILURE);
           }

          

           for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
               if (ifa->ifa_addr == NULL)
                   continue;

               family = ifa->ifa_addr->sa_family;

               
               printf("%s  address family: %d%s\n",
                       ifa->ifa_name, family,
                       (family == AF_PACKET) ? " (AF_PACKET)" :
                       (family == AF_INET) ?   " (AF_INET)" :
                       (family == AF_INET6) ?  " (AF_INET6)" : "");

               

               if (family == AF_INET || family == AF_INET6) {
                   s = getnameinfo(ifa->ifa_addr,
                           (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                                 sizeof(struct sockaddr_in6),
                           host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
                   if (s != 0) {
                       printf("getnameinfo() failed: %s\n", gai_strerror(s));
                       exit(EXIT_FAILURE);
                    }
                   printf("\taddress: <%s>\n", host);
               }
           }

           freeifaddrs(ifaddr);
           return(1);
       }
 
int main(int argc, char **argv)
{
     int getas;
 
 getas= getaddres();
    /* ---------- INITIALIZING VARIABLES ---------- */
    char * addr;
    addr=argv[1];

 
    int client;
    int portNum = 5000; // NOTE that the port number is same for both client and server
    portNum=atoi(addr);
    printf("portnum from io %d\n", portNum );
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];
    char* ip = "127.0.0.1";
 
    struct sockaddr_in server_addr;
 
    client = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    //server_addr.sin_port = htons(portNum);
    server_addr.sin_port = 34835;

     printf("addr %d, port %d \n", server_addr.sin_addr.s_addr ,  server_addr.sin_port );

 
    /* ---------- ESTABLISHING SOCKET CONNECTION ----------*/
   
 
    if (client < 0)
    {
        printf("\nError establishing socket...\n");
        exit(1);
    }
 
   
 
    printf("=> Socket client has been created...\n");
   
    /*
        The variable serv_addr is a structure of sockaddr_in.
        sin_family contains a code for the address family.
        It should always be set to AF_INET.
 
        htons() converts the port number from host byte order
        to a port number in network byte order.
 
    */
 
 
    // this function returns returns 1 if the IP is valid
    // and 0 if invalid
    // inet_pton converts IP to packets
    // inet_ntoa converts back packets to IP
    //inet_pton(AF_INET, ip, &server_addr.sin_addr);
 
   // if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
 
 
    /* ---------- CONNECTING THE SOCKET ---------- */
    /* ---------------- connect() ---------------- */
 
    if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        printf( "=> Connection to the server %s with port number: %d ", inet_ntoa(server_addr.sin_addr) ,portNum );
        printf("=> Connection to the server port number: %d", portNum);
 
 
 
    printf("=> Awaiting confirmation from the server...\n"); //line 40
    recv(client, buffer, bufsize, 0);
    printf("=> Connection confirmed, you are good to go...\n");
 
    printf("\n\n=> Enter # to end the connection\n");
 
    // Once it reaches here, the client can send a message first.
 
    do {
        printf("Client: ");
        do {
            scanf( "%80s",buffer);
            send(client, buffer, bufsize, 0);
            if (*buffer == '#') {
                send(client, buffer, bufsize, 0);
                *buffer = '*';
                isExit = true;
            }
        } while (*buffer != 42);
 
        printf( "Server: ");
        do {
            recv(client, buffer, bufsize, 0);
            scanf( "%80s",buffer);
            if (*buffer == '#') {
                *buffer = '*';
                isExit = true;
            }
 
        } while (*buffer != 42);
        printf("\n");
 
    } while (!isExit);
 
    /* ---------------- CLOSE CALL ------------- */
    /* ----------------- close() --------------- */
 
    /*
        Once the server presses # to end the connection,
        the loop will break and it will close the server
        socket connection and the client connection.
    */
 
    printf("\n=> Connection terminated.\nGoodbye...\n");
 
    close(client);
    return 0;
}