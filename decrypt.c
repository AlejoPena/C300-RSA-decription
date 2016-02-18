  /*Name: Alejandro Pena
 Student #: 301241286
 User name: apena
 Course section: D100
 Instructor: Brian Booth
 TA: Scott Krstjanson

*/
#include "decrypt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
//#include "memwatch.h"    	/////RECUERDA ARRELGARLO




//Change the base of a long int composed of 6 chars
unsigned long long  changebase41(char* arr, int inde)
{
	unsigned long long result=0;
	int index=inde;
	int powr=5;
    // printf("firts char in chanbase41: %c\n", arr[index] );
	while(index < inde+6)
	{
		if(arr[index]!='@'){result= result+ ((unsigned long long)arr[index]*exponente(41,(powr)));};
		//printf("result: %llu, ascii val: %d #: #d  %d\n",result,arr[index], index);
		index++;
		powr--;
	}
	//printf("converted: %lld\n", result);
	return result;
}


//Does modular exponientiation using the baseS and constants exponente and modula
unsigned long long  modeq(unsigned long long  baseS)
{
	
	unsigned long long base= baseS;
	unsigned long long exponente=1921821779;
	unsigned long long modula=4294434817;
	//printf("antes de assert\n");
	assert((modula - 1)*(modula - 1));
	unsigned long long result=1;
	base= base%modula;
	//printf("antes del loop\n");
	while(exponente>0)	{
		//printf("dentro del loop\n");
			if((exponente % 2) ==1){
				//printf("dentro del if\n");
				result=(result * base) % modula;
			};
			exponente=exponente >> 1;
			base= (base*base) % modula;
			//printf("fuera del if\n");
	};
	//printf("mod%llu\n", result);
	return result;
}

//Calculates the power by exponientiation by squearing
unsigned long long exponente(int base, int power)
{
	unsigned long long bs=base;
	if (base == 0 ){return 0;}
	if (power== 1){return base;}
	if (power==0){return 1;}

	unsigned long long  pw =power; 
	unsigned long long  carry =1; 
	
	while(pw>1)
	{
		if(pw % 2==0){
			bs=bs*bs;
			pw=pw/2;
		}else{
			carry=bs*carry;
			bs=bs*bs;
			pw=(pw -1)/2;

		}
	}
		//printf("bs*carry: %lld\n", bs*carry);
	return bs*carry;
}

//this transforms the value of the character and does not allow other
char check(char ll){
	int l =ll;
	if(l>='a' && l<='z'){
		return l-'`';
	};
	if(l== 32){ return 64;};
    if(l== 35){ return l-8;};
    if(l== 46){ return l-18;};
    if(l== 44){ return l-15;};
    if(l== 33){ return l-2;};
    if(l== 39){ return l-9;};
    if(l== 63){ return l-31;};
    if(l== 40){ return l-7;};
    if(l== 41){ return l-7;};
    if(l== 45){ return l-10;};
    if(l== 58){ return l-22;};
    if(l== 36){ return l+1;};
    if(l== 47){ return l-9;};
    if(l== 38){ return l+1;};
    if(l== 92){ return l-52;};
   

}
//transforms the value of the array cahr into a valid character for 
char translate(char ll){
	int l=ll;

	if(l>=1 && l<=26){
		return l+96;
	};
	if(l==0){return 32;};
	if(l>40){return 126;};
    if(l== 27){ return 35;};
    if(l== 28){ return 46;};
    if(l== 29){ return 44;};
    if(l== 30){ return 39;};
    if(l== 31){ return 33;};
    if(l== 32){ return 63;};
    if(l== 33){ return 40;};
    if(l== 34){ return 41;};
    if(l== 35){ return 45;};
    if(l== 36){ return 58;}
    else{return 126;};



}

//translates the long integer into character values using funct translate
void longtochar(unsigned long long mod, char* arr, int index){
	int in=0;
	int carreta=index;
	int car=0;
	int letra;
	unsigned long long divide=mod;
	unsigned long long res=0;
	char ar[6];

	for(in; in<6;in++){
		letra=divide%41;
		divide/=41;
		ar[in]=translate(letra);
	};
	
    car=5;
    for(car; car>-1; car--){
    	arr[carreta]=ar[car];    	
    	carreta++;
    }


}

// takes an array and decyfer its values into the apropiate ascii ones
void everysix(char* arr, int* tweetsize){

	
    int indes=0;
    unsigned long long mod=0;

	while( indes<tweetsize[0]){

		mod= changebase41(arr, indes);
		
		mod= modeq(mod);
		
		longtochar(mod, arr, indes);

		indes+=6;
	};

}

///Reads one line from FILE into an array
char * readline(FILE * ofile, int* twsize, char* status)
{
	
	
	char *bufffer;
	char letra=0;
	int index=0;

	// Open file and check it exist
	//ofile= fopen(ar, "r");

	
	if (ofile == NULL){
		printf("There is no tweet to read and decifer. \n");
		exit(9);
	};
     
    
	// allocate memory to store file contents
	bufffer= malloc(10000*sizeof(char));
	memset(bufffer, 0, 10000*sizeof(char));
	//printf("bufffer was malloc\n");
	if(bufffer== NULL){
		printf ("memory error allocating for bufffer \n");
		exit(9);
	};

	int the8thchat=1;
	
		letra= fgetc(ofile);
	while(index != 10000 && letra !='\n' && letra != EOF){

		status[0]= letra;

		if(the8thchat==8){

			letra= fgetc(ofile);
			the8thchat=1;
		};

		bufffer[index]=(char)letra;
		letra= fgetc(ofile);
		index++;
		the8thchat++;
   	 };

    int i=0;
       
    
    index= strlen(bufffer);    
        

     i=0;

     while(i!=index){

    	bufffer[i]= check(bufffer[i]);
		
		i++;

    };

    twsize[0]=index;
   
	
	return bufffer;
    
}

//// decription running funtion

char decryption(char* encriptedfile_path, char* decript_to_path)
{
	 char res[1024];
	 if(encriptedfile_path == NULL || decript_to_path == NULL){
		//printf("ERROR: there are not enough paramenters\n");
		return(-1);
	}else{   

	FILE * ofile;
	char * myfile=encriptedfile_path;


	ofile= fopen(myfile, "r+");

	if(ofile== NULL){printf("ERROR: files argv[1] does not exist\n"); return(-1);};	


	FILE * outfile;

	char* printfile=decript_to_path;


	outfile= fopen(printfile, "a+");
	
	
	char *tweet;

	int sizeoftweet[1]={0};

	char reading[1];

	unsigned long long result=1;    

	int est=0;
 	
	while(!feof(ofile) )
	{

		tweet=readline(ofile, sizeoftweet,reading);

		sizeoftweet[0]= strlen(tweet);

		everysix(tweet,sizeoftweet);

		sizeoftweet[0]= strlen(tweet);

		fwrite(tweet,sizeof(char), sizeoftweet[0], outfile);

		if (!feof(ofile)){fputc('\n',outfile);
		}else{
			fclose(ofile);
			fclose(outfile);};

			int in=0;
		
	est++;
	};

	
		
  
	free (tweet);
	est=(int)getpid();
    sprintf(res,"has successfully decrypted %s in process %d", encriptedfile_path, est);
	
	return est;

}	
 

 
}
