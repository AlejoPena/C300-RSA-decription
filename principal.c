 /*Name: Alejandro Pena
 Student #: 301241286
 User name: apena
 Course section: D100
 Instructor: Brian Booth
 TA: Scott Krstjanson

*/
 #include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "decrypt.h"
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/wait.h>
//#include "memwatch.h"



 //// decript(line, fd_w_parent[fds*2+1]); //SHULD BE A FUNCTION THAT HANDLE READ LINE IF TWO FILES
            ///ARE IN, AND HANDLE ERROR, and sends back error type or success message
 //this funtion is used in childdecripting function
 

 /////int handle_chdlerr(char line, int Wfdchild, ){
 /*
 
} 
*/
int sche_FCFS(FILE * ofile,int* fd_w_parent, int* fd_w_child, fd_set *rfds, int fds ){
  fd_set writefd;
  FD_ZERO(&writefd;)
  writefd=&rfds;
  int maxfd = fd_w_parent[(fds*2)+1];
  int selectres;
  int compare;
  int response;
  char buf[10];

    printf("scheRR %d\n",(int)getpid() );
         char line[2082];
         memset(line,0, 2082);
        int resread=0;
         int j;
         for(j=0; j<fds; j++){

             close(fd_w_child[(j*2) +1]);//close unused write end  1
             close(fd_w_parent[j*2]);//close unused read end   0
         }
         j=0;

         while (!feof(ofile)){
            writefd=&rfds;
            if (selectres=(select(maxfd+1, &writefd, NULL,NULL,NULL))  ==-1){
              continue;
            };
            if(selectres == 0){
              continue;
            };

            fgets(line, 2050, ofile);
             if(feof(ofile)){
                 break;
               }

               for(j=0;j<fds;j++){

                if(FD_ISSET(fd_w_child[2*j], &writefd )){
                  resread=read(fd_w_child[2*j],buf, sizeof(buf));
                  resread= strncmp(buf,"FREE", 4);
                  if(resread == 0){
                    write(fd_w_child,line,2050);
                  }



                }
               }
             if(j==fds){j=0;};


               printf("parent writes to fd %d, line: %s\n", (j*2)+1, line );
               write(fd_w_parent[(j*2)+1],line, 2050);
               memset(line,0, 2082);
               j++;
          };

          
          for(j=0; j<fds; j++){
              write(fd_w_parent[(j*2) +1],"FINISHED", 14);
              close(fd_w_parent[(j*2) +1]);
          }


          return 1;


}

void *timeis(char* timestamp){
   time_t timenow;
         time (&timenow);
         struct tm * timeinfo;
               time(&timenow);
         timeinfo= localtime(&timenow);
         strftime(timestamp,80,"%a %b %d %T %G ", timeinfo);
       
}

 int sche_round_robin(FILE * ofile, int* fd_w_parent, int* fd_w_child, int fds){
  printf("scheRR %d\n",(int)getpid() );
         char line[2082];
         memset(line,0, 2082);
        int pipe_index=0;
         int j;
         for(j=0; j<fds; j++){

             close(fd_w_child[(j*2) +1]);//close unused write end  1
             close(fd_w_parent[j*2]);//close unused read end   0
         }
         j=0;
         while (!feof(ofile)){ 

             fgets(line, 2050, ofile);
             if(j==fds){j=0;};
             if(feof(ofile)){
                 break;
               }
             printf("parent writes to fd %d, line: %s\n", (j*2)+1, line );
             write(fd_w_parent[(j*2)+1],line, 2050);
             memset(line,0, 2082);
             j++;
          };

          
          for(j=0; j<fds; j++){
              write(fd_w_parent[(j*2) +1],"FINISHED", 14);
              close(fd_w_parent[(j*2) +1]);
          }


          return 1;


 }
int child_decripting(fd_set rfds, int* fd_w_parent, int* fd_w_child, int fds){
  char line[2082];
  memset(line,0, 2082);
   close(fd_w_parent[fds*2+1]); //close unused write end
   close(fd_w_child[fds*2]); //close unused read end
  printf("ch_descripting %d\n",(int)getpid() );
   int strcomp;
   int readpipe;
        // printf("child OUTLOOP1:%d,\n",(int)getpid());
           //write(myfd[1], "free", 4);

        // sleep(2);
        while(1){ 
         
          readpipe= read(fd_w_parent[fds*2], &line, 2050);
          //printf("child repipe%d\n",readpipe );
          if(readpipe==0){
            printf("redpipe :0 in child %d\n", (int)getpid());
            continue;
          }
          

          strcomp=strncmp(line, "-1",2);
          strcomp=strncmp(line, "FINISHED",7);
          printf("child %d strcomp: %d\n",(int)getpid(), strcomp );
          if(strcomp==0 ){
            write(fd_w_child[fds*2+1],"FINISHED_CHILD", 14);
            close(fd_w_child[fds*2+1]);
            printf("exiting child: %d\n",(int)getpid());
            //exit(0);
            break;
          };

          if(readpipe!=0){
            //// decript(line, fd_w_parent[fds*2+1]); //SHULD BE A FUNCTION THAT HANDLE READ LINE IF TWO FILES
            ///ARE IN, AND HANDLE ERROR, and sends back error type or success message


            printf("%d Child %d line =  %s\n",readpipe,(int)getpid(), line );
            memset(line,0, 2000);
            //printf("CHILD writing free\n");
            write(fd_w_child[fds*2+1],"FREE", 4);

          };
          if(readpipe == -1){break;};         
        };    
          
        printf("out of child loop\n");
        return 0;



}

char *piping (char lineconf) {

//Reviewing config file
FILE * ifile;

char *ifile_arr=lineconf;

ifile= fopen(ifile_arr, "r+");

if(ifile== NULL){printf("ERROR: files argv[1] does not exist\n"); exit(9);};

char line[2082];

fgets(line, 2050, ifile);






 long cpu_num=0;

 cpu_num=(sysconf(_SC_NPROCESSORS_ONLN))-1;
 printf("pipe # to create: %lu\n", cpu_num);


//Variables needed in main fork loop for parent and children
char *path_name;

char *path_out;

int ppp[14];


char times[80];

int stat=0;
int child_decryp_res;

pid_t parent= getpid();

pid_t childPID ;

int exitval=0;

int children_num=1;

int car=0;
int pipe_index=0;
int i=cpu_num*2;

pid_t pid=-1;

int papa[i];
int fd_w_parent[cpu_num*2];

int baby[i];
int fd_w_child[cpu_num*2];
i=0;
int * pipeptn;
fd_set parent_writefd;
fd_set child_writefd;

int retval;
FD_ZERO(&parent_writefd);
FD_ZERO(&child_writefd);

printf("parent %d\n",(int)getpid() );


////MAIN FORK LOOP this will created children processes 
for(i=0; i< cpu_num;i++){

 //  printf("pipe %d, %d \n",fd_w_parent[pipe_index], fd_w_parent[pipe_index+1] );
  printf("forloop: %d\n",i);
    pipeptn=&fd_w_parent[i*2];
  //  printf("pipeptn: %p\n",&pipeptn );
  
  if (pipe (pipeptn) )
   {
        fprintf (stderr, "fdPipe failed.\n");
        return EXIT_FAILURE;
   };

    FD_SET(fd_w_parent[i*2], &parent_writefd);
    //FD_SET(fd_w_parent[i*2+1], &parent_writefd);

    
    printf("pipep %d : %d, pipep %d: %d\n", i*2,fd_w_parent[i*2], i*2+1, fd_w_parent[i*2+1]);
    //  printf("pipe %d, %d \n",fd_w_parent[pipe_index], fd_w_parent[pipe_index+1] );//***************
    // fd_w_child[pipe_index]=&baby[pipe_index];
    pipeptn=&fd_w_child[i*2];
    if (pipe (pipeptn) )
    {
      fprintf (stderr, "fdPipe failed.\n");
      return EXIT_FAILURE;
    };
      
      FD_SET(fd_w_child[i*2],   &child_writefd);
      //FD_SET(fd_w_child[i*2+1], &child_writefd);

      printf("pipeC %d : %d, pipeC %d: %d\n", i*2, fd_w_child[i*2], i*2+1, fd_w_child[i*2+1]);
     // printf("pipe child %d , %d\n",fd_w_child[pipe_index],fd_w_child[pipe_index+1] );//*****************
      pipe_index+=2;


      ///**************FORK*****FORK*****************************FORK******
      pid=fork();

      if(pid == 0){
        printf("fork %d\n",(int)getpid() );
        break;
      };
};

////*****************FORKS PARENT AND CHILD*******************************************************


    if(pid < 0){
      perror("fork failed."); exit(-1);
     };
      
     if(pid == 0){                              ///***************CHILD

    stat=child_decripting(&rfds, fd_w_parent,fd_w_child, i); 
     
     if(stat == 0){

      exit(44);

     }
        

        
   
      }else{ //***************************************************PARENT

       

         if(schedule_FCFS==0){

              //sche_FCFS(ifile,fd_w_parent,fd_w_child, &rfds, fds);
          }else if(schedule_round_rob == 0){

            child_decryp_res=sche_round_robin(ifile,fd_w_parent,fd_w_child, cpu_num);

            }else{
             exit(-1);
            };
        
                      
        stat=0;
        if(feof(ifile)){  
                
         for(car=0;car<=cpu_num; car++) {
                  childPID= wait(&stat);
                  exitval = WEXITSTATUS(stat);
                 //printf("   child PID: %d status %d, returval %d\n",(int)childPID, stat, exitval );
                 if(exitval!=44 && stat != 0){
                   timeis(times);
                   printf("[%s] Child process ID #%d did not terminate successfully.\n",times,(int)childPID );
                  };
          }
         }
      };       






 ////closing cofig file
fclose(ifile);


//printf("exiting papa\n");
return 1;


      }; 
            


     







