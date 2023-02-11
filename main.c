#include <stdio.h>
extern int Btype;
extern float Rfac;
void *Runkgcalc(void *);
int main(int argc,char **argv) { 
   if(argc>1) {
    if((argv[1][0]=='-')&&(argv[1][1]=='t')) sscanf(argv[1]+2,"%d",&Btype);
    else if((argv[1][0]=='-')&&(argv[1][1]=='r')) sscanf(argv[1]+2,"%f",&Rfac);
    if(argc > 2) {
      if((argv[2][0]=='-')&&(argv[2][1]=='t')) sscanf(argv[2]+2,"%d",&Btype);
      else if((argv[2][0]=='-')&&(argv[2][1]=='r')) sscanf(argv[2]+2,"%f",&Rfac);
    }
   }
   Runkgcalc(NULL);
   return 1;
}
