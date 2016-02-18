#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>



int main (int argc, char **argv) {

FILE * ifile;

char *ifile_arr=argv[1];

ifile= fopen(ifile_arr, "r+");

if(ifile== NULL){printf("ERROR: files argv[1] does not exist\n"); exit(9);};

char line[2082];
char free_child[20];
int exitval=0;


int stat=0;

int readpipe;


pid_t childPID ;

 long cpu_num=0;
 cpu_num=(sysconf(_SC_NPROCESSORS_ONLN));	

 printf("numero de cpus= %lu\n", cpu_num);

 int fd[2];
// pipe(fd);
int closechl=0;

int strcomp=0;

  if (pipe (fd))
    {
      fprintf (stderr, "fdPipe failed.\n");
      return EXIT_FAILURE;
    }
 
 int myfd[2];
 pipe(myfd);

/*for() { 
  if (pipe (myfd))//two pipes?
    {
      fprintf (stderr, "myPipe failed.\n");
      return EXIT_FAILURE;
    }
}  */  
printf("antes del fork loop\n");
int forksNum=0;

for(forksNum; forksNum <=3; forksNum++){

	     pid_t pid = fork();
		printf("in fork:%d, %s\n",(int)getpid(), line );
	     if(pid== -1){return 0;}
	    // printf("bef child-parent %d\n", (int)read(myfd[0], &line, 2000) );

    /*/CHILDREN*/ 
		if(pid== 0){ //child
	    	 close(myfd[0]);
	    	 printf("child OUTLOOP1:%d,\n",(int)getpid());
	         //write(myfd[1], "free", 4);

	     	while(1){ 
	    	 
	     		write(myfd[1],"BUSY", 4);
	     		sleep(3);
	     		readpipe= read(fd[0], &line, 2000);
	     		printf("child repipe%d\n",readpipe );
	     		if(readpipe==0){
	     			stat++;
	     			printf("child reads pipe =0, %d\n",stat);
	     		}
	     		

	     		if(readpipe!=0){
	     			printf("%d Child %dline =  %s\n",readpipe,(int)getpid(), line );
	     			write(myfd[1],"free", 4);
	     			memset(line,0, 2000);
	     			printf("CHILD writing free\n");
	     			write(myfd[1],"FREE", 4);

	     		};

	     		readpipe= strncmp(line, "-1",2);
	     		printf("chilpipe strncmp=%d\n",readpipe );

	     		strcomp=strncmp(line, "-1",2);
	     		if(strcomp==0){
	     			close(fd[1]);
	     			printf("exiting child %d\n",(int)getpid());
	     			exit(0);
	     			break;
	     		};
	     	};		
	     		
	     	
	     	return 0;

			/*/PARENT*/ 
	    }else{    ///parent
	     		close(fd[0]);
	     		fgets(line, 2082, ifile);
	     		write(fd[1],line, 200);
	     		
	     		while(!feof(ifile) ){
	     			printf("parent loop\n");
	     			//sleep(1);

	     			fgets(line, 2082, ifile);
	     			
	     			if(feof(ifile)){  
	     						printf("EOF fd=-1\n");                
            		  			write(fd[1], "-1", 2);
            		  			break;
            		}

	     			while(1){
	     				readpipe= read(myfd[0], &free_child, 4);
	     				strcomp=strncmp(free_child, "BUSY",4);
	     				if(strcomp == 0){
	     					printf("continue parent\n");
	     					continue;}
	     				printf("repipe papa=%d\n",readpipe );
	     				if(readpipe ==0){printf("papa readpipe 0\n");};

	     				if(readpipe != 0){  
	     					//printf("PARENT writing pipe: %s, readpipe %d\n", line, readpipe );
	     					//printf("Papa writing line on pipe \n");	     					          			     
	     			      	write(fd[1],line, 2000);
	     			      	memset(line,0, 2000);
	     			      	break;
	     		        	
	     					
	     				}
	     			};	     			
	     		};
	     		stat=0;

	     		if(feof(ifile)){  
                   close(fd[1]);
                   close(myfd[0]);
            		
	     			for(closechl=0; closechl<=cpu_num; closechl++){
              		  childPID= wait(&stat);
               		  exitval = WEXITSTATUS(stat);
              	  		//printf("   child PID: %d status %d, returval %d\n",(int)childPID, stat, exitval );
              	 	 if(stat != 0){
                 
                 		 printf(" Child process ID #%d did not terminate successfully.\n",(int)childPID );
                		};
              		}; return 1;
            	}
	     	}
}
	
	return 1;
}