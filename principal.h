 /*Name: Alejandro Pena
 Student #: 301241286
 User name: apena
 Course section: D100
 Instructor: Brian Booth
 TA: Scott Krstjanson

*/
#ifndef _PRINCIPAL_H_
#define _PRINCIPAL_H_

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
int sche_FCFS(FILE * ofile,int* fd_w_parent, int* fd_w_child, fd_set *rfds, int fds );

void *timeis(char* timestamp);

 int sche_round_robin(FILE * ofile, int* fd_w_parent, int* fd_w_child, int fds);

int child_decripting(fd_set rfds, int* fd_w_parent, int* fd_w_child, int fds);


char *piping (char lineconf);

 #endif
