 /*Name: Alejandro Pena
 Student #: 301241286
 User name: apena
 Course section: D100
 Instructor: Brian Booth
 TA: Scott Krstjanson

*/
#ifndef _DECRYPT_H_
#define _DECRYPT_H_

#include <stdio.h>


#include <string.h>


unsigned long long  changebase41(char* arr, int inde);


unsigned long long  modeq(unsigned long long  baseS);

unsigned long long exponente(int base, int power);

char check(char ll);

char translate(char ll);

void longtochar(unsigned long long mod, char* arr, int index);

void everysix(char* arr, int* tweetsize);

char * readline(FILE * ofile, int* twsize, char* status);

int decryption(char* encriptedfile_path, char* decript_to_path);



#endif 

