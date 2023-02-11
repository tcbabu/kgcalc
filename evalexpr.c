#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
//#include "dlink.h"
#include <kulina.h>
static double Rfac=0.01745329251994;
static double Fac = 0.01745329251994;
typedef struct _op {
   unsigned int id;  // N
   unsigned int code;
} OP;
typedef struct _num {
   unsigned int id;  // O
   double v;
} NUM;
typedef union {OP op;NUM num;} ITEM;
int SetConvFac(int state) {
  switch(state) {
    case 0:
      Fac = atan(1.0)/45.0;
    break;
    case 1:
       Fac = 1.0;
    break;
  }
  return state;
}
int dosub(double v1,double v2,double *ans) { *ans=(v1-v2);return 1;}
int doadd(double v1,double v2,double *ans) { *ans=(v1+v2);return 1;}
int domul(double v1,double v2,double *ans) { *ans=(v1*v2);return 1;}
int dodiv(double v1,double v2,double *ans) {
   if(v2!=0.0) *ans=(v1/v2);
   else return 0;
   return 1;
}
int dopow(double v1,double v2,double *ans) {
   int iv2,ret=1;
   if(v1>0.0) *ans=(pow(v1,v2));
   else {
    iv2 = (int)(v2+1.e-12);
    if(fabs(iv2-v2) < 1.e-8) *ans=(pow(v1,iv2));
    else ret=0;
   }
   return ret;
}
int doperc(double v1,double v2,double *ans) {
   int iv2;
   *ans= (v1*v2)/100.0;
   return 1;
}
int domod(double v1,double v2,double *ans) {
   int iv2,ret=1;
   if(v2==0.0) ret=0;
   else *ans= (double)((int)v1-((int)v1/(int)v2)*(int)v2);
   return 1;
}
typedef struct _ops {
  int code;
  int prio;
  int asso;
  int (*execop) (double,double,double *);
} OPS;
OPS ops[] ={
  {'-',0,0,dosub},{'+',0,0,doadd},{'*',5,0,domul},{'/',10,0,dodiv},
  {'^',12,1,dopow},{'%',14,0,doperc},{'!',5,0,domod},{'E',0,0,NULL}
};
typedef struct _funs {
 char *name;
 int (*evalfun)(double v,double *ans);
} FUNS;

int Recep(double v,double *ans) {
   double ang;
   if(v==0.0) return 0;
   *ans = 1./v;
   return 1;
}
int Csign(double v,double *ans) {
   double ang;
   *ans = -v;
   return 1;
}
int Sin(double v,double *ans) {
   double ang;
   ang = (v*Fac);
   *ans = sin(ang);
   return 1;
}
int Cos(double v,double *ans) {
   double ang;
   ang = (v*Fac);
   *ans = cos(ang);
   return 1;
}
int Tan(double v,double *ans) {
   double ang;
   ang = (v*Fac);
   *ans = tan(ang);
   return 1;
}
int Asin(double v,double *ans) {
   double ang;
   if( fabs(v) > 1.0) return 0;
   v = asin(v);
   ang = (v/Fac);
   *ans = (ang);
   return 1;
}
int Acos(double v,double *ans) {
   double ang;
   if( fabs(v) > 1.0) return 0;
   v = acos(v);
   ang = (v/Fac);
   *ans = (ang);
   return 1;
}
int Atan(double v,double *ans) {
   double ang;
   v = atan(v);
   ang = (v/Fac);
   *ans = (ang);
   return 1;
}
int Sqr(double v,double *ans) {
   double ang;
   *ans = (v*v);
   return 1;
}
int Sqrt(double v,double *ans) {
   double ang;
   if(v< 0.0) return 0;
   if(v==0.0) *ans =0.0;
   else *ans = sqrt(v);
   return 1;
}
int Exp(double v,double *ans) {
   double ang;
   *ans = exp(v);
   return 1;
}
int Ln(double v,double *ans) {
   double ang;
   *ans = log(v);
   return 1;
}
int Log(double v,double *ans) {
   double ang;
   *ans = log10(v);
   return 1;
}
FUNS * getfun(char *name);
FUNS funs[]= {
  {(char *)"sin",Sin},{(char *)"cos",Cos},{(char *)"tan",Tan},{(char *)"asin",Asin},
  {(char *)"acos",Acos},{(char *)"atan",Atan},{(char *)"sqr",Sqr},{(char *)"sqrt",Sqrt},
  {(char *)"exp",Exp},{(char *)"ln",Ln},{(char *)"log",Log},{(char *)"csign",Csign},{(char *)"recep",Recep},
  {(char *)"E",NULL}
};
OPS * getop(int op);
int compops(int op1,int op2) {
  int i=0,j=0,ret=1;
  while (ops[i].code!= op1) i++;
  while (ops[j].code!= op2) j++;
  if(ops[i].prio==ops[j].prio) ret=0;
  else if(ops[i].prio<ops[j].prio) ret=-1;
  return ret;
   
}
int isrightasso(int op) {
  int i=0,ret=0;
  while (ops[i].code!= op){
       if(ops[i].code=='E') return -1;
       i++;
  }
  return ops[i].asso;
}
OPS * getop(int op) {
  int i=0,ret=0;
  while (ops[i].code!= op) {
    if(ops[i].code=='E') return NULL;
    i++;
  }
  return ops+i;
}
FUNS * getfun(char *name) {
  int i=0,ret=0;
  while (strcmp(funs[i].name,name)!=0) {
    if(funs[i].name[0]=='E') return NULL;
    i++;
  }
  return funs+i;
}
int  evalexpr(char *expr,double *ans);
int getnumber(char *buf,double *ans) {
  double val,val2,val3;
  int i=0,ch,op,j,left;
  while(buf[i]==' ')i++;
  ch = buf[i];
  if(ch=='(') { 
     i++;
     j=i-1;
     left=1;
     while(left) {
            j++;
            if(buf[j]<' ') return 0;
            if(buf[j]==')')left--;
            if(buf[j]=='(')left++;
      }
      buf[j]='\0';
      if(!evalexpr(buf+i,&val)) return 0;
      buf[j]=')';
      i= j+1;
  }
  else {
   if( !isdigit(ch)&&(ch!='.')) return 0;
   while(isdigit(ch)) ch=buf[++i];
    if(ch=='.')ch=buf[++i];
    if(ch=='e'){
      ch=buf[++i];
      if(ch=='-')ch=buf[++i];
      if(ch=='+')ch=buf[++i];
      if(!isdigit(ch)) return 0;
    }
    while(isdigit(ch)) ch=buf[++i];
//TCB
    if(ch=='e'){
      ch=buf[++i];
      if(ch=='-')ch=buf[++i];
      if(ch=='+')ch=buf[++i];
      if(!isdigit(ch)) return 0;
      while(isdigit(ch)) ch=buf[++i];
    }
    buf[i]='\0';
    if(sscanf(buf,"%lg",&val)<=0) return 0;
    buf[i]=ch;
 }
 *ans=val;
 return i;
}
#define RETURN0 {Dempty(L);Dempty(Ops);return 0;}
int  evalexpr(char *expr,double *ans) {
  double val=0,val1,val2,val3;
  char buf[5000],fname[10];
  Dlink *L=NULL,*Ops=NULL;
  FUNS *fn;
  ITEM *it;
  int l,i,ret=1,ch,j,left=0,right=0,op,opcount=0,valcount=0,count;
  l = strlen(expr)+1;
  strcpy(buf,expr);
  Ops=Dopen();
  L =Dopen();
  i=0;
  while(buf[i]==' ')i++;
  if(buf[i] < ' ') ret=0;
  else {
   while(i<l) {
    ch = buf[i];
    if((ch=='(')||(ch=='-')||(ch=='+')||isdigit(ch)||(ch=='.')) { // get number
       switch(ch) {
         case '(':
           i++;
           j=i-1;
           left=1;
           while(left) {
            j++;
            if(buf[j]<' ') RETURN0;
            if(buf[j]==')')left--;
            if(buf[j]=='(')left++;
           }
           buf[j]='\0';
           if(!evalexpr(buf+i,&val1)) {
                   RETURN0;
           }
           i= j+1;
           break;
        case '+':
           i++;
           if((count=getnumber(buf+i,&val1))<=0 ) RETURN0;
           i+=count;
           break;
        case '-':
           i++;
           if((count=getnumber(buf+i,&val1))<=0 ) RETURN0;
           i+=count;
           val1 =-val1;
           break;
        default:
           if((count=getnumber(buf+i,&val1))<=0 ) RETURN0;
           i+=count;
           break;
        
       }
    }
    else {  // evaluate fun to get number
      // To be implemented to handle functions
      while(buf[i]==' ')i++;
      j=0;
      while(buf[i]>' '){
       if(buf[i]=='(') break;
       fname[j++]=buf[i++];
      }
      fname[j]='\0';
      while(buf[i]==' ')i++;
      if(buf[i]!='(') RETURN0;
      if( (fn=getfun(fname))!= NULL) {
           i++;
           j=i-1;
           left=1;
           while(left) {
            j++;
            if(buf[j]<' ') RETURN0;
            if(buf[j]==')')left--;
            if(buf[j]=='(')left++;
           }
           buf[j]='\0';
           if(!evalexpr(buf+i,&val2)) {
                   RETURN0;
           }
           i= j+1;
           if( fn->evalfun(val2,&val1) <= 0) {
                   RETURN0;
           }      
      }
      else RETURN0;
    }
    it = (ITEM *) malloc(sizeof(ITEM));
    it->num.id = 'N';
    it->num.v  = val1;
    Dadd(L,it);
    while(buf[i]==' ')i++;
    if(buf[i] < ' ') { break;}
    op = buf[i];
    if(getop(op)==NULL) RETURN0;
    i++;
    while(buf[i]==' ')i++;
    if(getop(buf[i])!=NULL) RETURN0;
    Dend(Ops);
    while ( (it=(ITEM *)Getrecordrev(Ops)) != NULL) {
      if(!isrightasso(op)) {
        if(compops(op,it->op.code)<= 0) { Dend(Ops);it=(ITEM *)Dpick(Ops);Dappend(L,it);Dend(Ops);continue;}
      }
      else {
        if(compops(op,it->op.code)< 0) { Dend(Ops);it=(ITEM *)Dpick(Ops);Dappend(L,it);Dend(Ops);continue;}
      }
      break;
    }
    it = (ITEM *) malloc(sizeof(ITEM));
    it->op.id='O';
    it->op.code=op;
    Dappend(Ops,it);
   }   
   Dend(Ops);
   while ( (it=(ITEM *)Getrecordrev(Ops)) != NULL) {
     Dappend(L,it);
   }
   Dfree(Ops);
  }  // else buf[i] < ' '
  Resetlink(L);
  while ((it=(ITEM *)Getrecord(L))!= NULL){
    switch(it->num.id) {
      case 'N':
//        printf("Num: %lf\n",it->num.v);
        break;
      case 'O':
        op = it->op.code;
//        printf("Op: %c\n",op);
        Dmove_back(L,3);
        it = (ITEM *)Dpick(L);
        val1 = it->num.v;
        free(it);
        it = (ITEM *)Dpick(L);
        val2 = it->num.v;
        free(it);
        it = (ITEM *)Dpick(L);
//        if (isrightasso(op)) val2=getop(op)->execop(val2,val1);
//        else val2=getop(op)->execop(val1,val2);
        if(getop(op)->execop(val1,val2,&val3)==1) val2=val3;
        else {Dempty(L);return 0;}
        it->num.id='N';
        it->num.v= val2;
        Dinsert(L,it);
        Resetlink(L);
      break;
        
    }
  }
  Resetlink(L);
  if ((it=(ITEM *)Getrecord(L))== NULL){ret=0;}
  else *ans=it->num.v;
  Dempty(L);
  return ret;
}
