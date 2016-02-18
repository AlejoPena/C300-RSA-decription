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
 

 
int main()
{
    
 int getas;
 
 getas= getaddres();
    /*  
       1. client/server are two file descriptors
       These two variables store the values returned
       by the socket system call and the accept system call.
 
       2. portNum is for storing port number on which
       the accepts connections
 
       3. isExit is bool variable which will be used to
       end the loop
 
       4. The server reads characters from the socket
       connection into a dynamic variable (buffer).
 
       5. A sockaddr_in is a structure containing an internet
       address. This structure is already defined in netinet/in.h, so
       we don't need to declare it again.
 
        DEFINITION:
 
        struct sockaddr_in
        {
          short   sin_family;
          u_short sin_port;
          struct  in_addr sin_addr;
          char    sin_zero[8];
        };
 
        6. serv_addr will contain the address of the server
 
        7. socklen_t  is an intr type of width of at least 32 bits
 
 
    */
    int client, server;
    int portNum = 5000;
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];
 
    struct sockaddr_in server_addr;
    socklen_t size;
 
    /* ---------- ESTABLISHING SOCKET CONNECTION ----------*/

    memset(&server_addr, 0 , sizeof(struct sockaddr_in)); 
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNum);

    printf("addr %d, port %d \n", server_addr.sin_addr.s_addr ,  server_addr.sin_port );

    client = socket(AF_INET, SOCK_STREAM, 0);
 
    if (client < 0)
    {
        printf( "Error establishing socket...\n");
        exit(1);
    }
 
    
 
    printf( "Socket server has been created...\n" );
 
    
 
    /* ---------- BINDING THE SOCKET ---------- */
    /* ---------------- bind() ---------------- */
 
 
    if ((bind(client, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0)
    {
        printf("=> Error binding connection, the socket has already been established...\n");
        return -1;
    }
 
    /*
        The bind() system call binds a socket to an address,
        in this case the address of the current host and port number
        on which the server will run. It takes three arguments,
        the socket file descriptor. The second argument is a pointer
        to a structure of type sockaddr, this must be cast to
        the correct type.
    */
 
    size = sizeof(server_addr);
    printf("=> Looking for clients...\n");
 
    /* ------------- LISTENING CALL ------------- */
    /* ---------------- listen() ---------------- */
 
    listen(client, 1);
 
    
 
    /* ------------- ACCEPTING CLIENTS  ------------- */
   
 
    int clientCount = 1;
    server = accept(client,(struct sockaddr *)&server_addr,&size);
 
    // first check if it is valid or not
    if (server < 0)
        printf("=> Error on accepting...\n");
 
    while (server > 0)
    {
        strcpy(buffer, "=> Server connected...\n");
        send(server, buffer, bufsize, 0);
        printf("=> Connected with the client # %d you are good to go...\n", clientCount );
        printf("=> Enter # to end the connection\n");
 
        /*
            Note that we would only get to this point after a
            client has successfully connected to our server.
            This reads from the socket. Note that the read()
            will block until there is something for it to read
            in the socket, i.e. after the client has executed a
            the send().
 
            It will read either the total number of characters
            in the socket or 1024
        */
 
        printf("Client: ");
        do {
            recv(server, buffer, bufsize, 0);
            printf("%s\n", buffer);
            if (*buffer == '#') {
                *buffer = '*';
                isExit = true;
            }
        } while (*buffer != '*');
 
        do {
            printf( "\nServer: ");
            do {
                scanf("%79s", buffer);
                send(server, buffer, bufsize, 0);
                if (*buffer == '#') {
                    send(server, buffer, bufsize, 0);
                    *buffer = '*';
                    isExit = true;
                }
            } while (*buffer != '*');
 
            printf("Client: ");
            do {
                recv(server, buffer, bufsize, 0);
                 scanf("%79s", buffer);
                if (*buffer == '#') {
                    *buffer == '*';
                    isExit = true;
                }
            } while (*buffer != '*');
        } while (!isExit);
 
        /*
            Once a connection has been established, both ends
            can both read and write to the connection. Naturally,
            everything written by the client will be read by the
            server, and everything written by the server will be
            read by the client.
        */
 
        /* ---------------- CLOSE CALL ------------- */
        /* ----------------- close() --------------- */
 
        /*
            Once the server presses # to end the connection,
            the loop will break and it will close the server
            socket connection and the client connection.
        */
 
        // inet_ntoa converts packet data to IP, which was taken from client
        printf( "\n\n=> Connection terminated with IP %s",inet_ntoa(server_addr.sin_addr));
        close(server);
        printf( "\nGoodbye...\n");
        isExit = false;
        exit(1);
    }
 
    close(client);
    return 0;
}