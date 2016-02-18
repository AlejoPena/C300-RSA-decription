  /*Name: Alejandro Pena
 Student #: 301241286
 User name: apena
 Course section: D100
 Instructor: Brian Booth
 TA: Scott Krstjanson

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h> /* obligatory includes */ 
#include <signal.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/wait.h> 
#include <netinet/in.h> 
#include <netdb.h>
#include <limits.h>


 #define PORTNUM 34835 /* random port number, we need something */ 

/*Establishing the socket*/
 int establish(unsigned short portnum) 
 { char myname[HOST_NAME_MAX+1];
  int s;
  printf("myname[MAXHOSTNAME]: %s\n", myname );
   struct sockaddr_in sa;
    struct hostent *hp;
     memset(&sa, 0, sizeof(struct sockaddr_in));  /* clear our address */ 
     gethostname(myname, sizeof(myname)-1); /* who are we? */ 
     hp= gethostbyname(myname); /* get our address info */ 
     if (hp == NULL){ /* we don't exist !? */ 
    	 return(-1); 
 	 }
     sa.sin_family= hp->h_addrtype; /* this is our host address */ 
     
     sa.sin_port= htons(portnum); /* this is our port number */ 

     if ((s= socket(AF_INET, SOCK_STREAM, 0)) < 0) {/* create socket */ 
    	 return(-1); 
 	}
     if (bind(s,(struct sockaddr *)&sa,sizeof(struct sockaddr_in)) < 0)     { 
      close(s); 
      return(-1); /* bind address to socket */ 
      } 
      listen(s, 3); /* max # of queued connects */ 
      printf("s:%d\n", s );
      return(s); 
  } 

/* wait for a connection to occur on a socket created with establish() */ 

int get_connection(int s) 
{ 

int t; /* socket of connection */ 
if ((t = accept(s,NULL,NULL)) < 0) /* accept connection if there is one */ 
  return(-1);
   return(t); 
}




void fireman(void);
void do_something(int); 



main()
{
 int s, t, i; 
 if ((s= establish(34835)) < 0) { /* plug in the phone */ 
 	perror("establish error");
 	exit(-1); 
 } 
  //signal(SIGCHLD, fireman); /* this eliminates zombies */ 

   for (i=0;i=1;i++)   { /* loop for phone calls */ 
	  if ((t= get_connection(s)) < 0) { /* get a connection */ 
	  	if (errno == EINTR) /* EINTR might happen on accept(), */ 
	  	continue; /* try again */ 
	  	perror("accept"); /* bad */ 
	  	exit(1);
	  } 
	   switch(fork()) { /* try to handle connection */ 
	  	  case -1 : /* bad news. scream and die */ 
	   		perror("fork");
	   		close(s);
	  		close(t); 
	   		exit(1); 
	 	  case 0 : /* we're the child, do something */ 
		   close(s); 
		   do_something(t); 
		   exit(0); 
	  	  default : /* we're the parent so look for */ 
		   close(t); /* another connection */ 
		  continue; 
        } 
   } 
} 
   /* as children die we should get catch their returns or else we get 
   * zombies, A Bad Thing. fireman() catches falling children. 
   */ 
   void fireman(void)
    { 
   while (waitpid(-1, NULL, WNOHANG) > 0) ;
    } 
    /* this is the function that plays with the socket. it will be called 
    * after getting a connection. 
    */
    void do_something(int s)
    {
    	send(s,"Hola cliente soy el serv jajaj",24,0);	
    	printf("msm enviado al cli\n");
       /* do your thing with the socket here 
       : :
       */ 
    }