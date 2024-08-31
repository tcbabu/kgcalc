#include <kulina.h>
#include <math.h>
#include <string.h>
extern void *Onimg,*Offimg;
static char dspbuf[200];
static void *Dsp=NULL;
static Dlink *Elist=NULL;
static double val=0,mem1=0.,mem2=0.;
static int mitem=8,dot=0,left=0,tbox=6,Added=0;
static char exprbuff[2000],valbuf[60],str1[100],str2[100],cpybuff[2000],subexpr[2000];
int  evalexpr(char *expr,double *ans);
void Splashmsg(char * msg);
int SetConvFac(int state);
int AddOp(int op);
int Strcat(char *d,char *s) {
  int lo,l,ret=1;
  lo = strlen(d);
  l = strlen(s);
  if((l+lo)<=499) strcat(d,s);
  else {
    ret =0;
    Splashmsg((char *)"BUFFER OVERFLOW");
  }
  return ret;
}
void BackupExpr(void) {
  char *pt;
  if(exprbuff[0]!='\0') {
    pt= (char *)malloc(strlen(exprbuff)+1);
    strcpy(pt,exprbuff);
    Dappend(Elist,pt);
  }
  Added=1;
}
char *GetFrecord(void) {
  if(Elist->cr==NULL) Dend(Elist);
  else Dmove_forward(Elist,1);
  if(Elist->cr!=NULL) {valbuf[0]='\0';return (char *)(Elist->cr->bf);}
  else return NULL;
}
char *GetBrecord(void) {
  if(Elist->cr==NULL) Drewind(Elist);
  else  Dmove_back(Elist,1);
  if(Elist->cr!=NULL) {valbuf[0]='\0';return (char *)(Elist->cr->bf);}
  else return NULL;
}
int Dspcpy(char *d,char *s){
  int i=0;
  while((s[i]==' ')&&(s[i]!='\0')) i++;
  strcpy(d,s+i);
  return 1;
}
static int calcUpdate(void *tmp,double val) {
 double rem;
 int ival,l,i,sign=1;
 if(val < 0.0) sign=-1;
 ival = (int)(fabs(val)+1.e-12);
 rem = fabs(val) -ival;
 ival *=sign;
 dspbuf[0]='\0';
// if((rem >1.e-30)) {
 {
    if(fabs(val)>1.){
         if(fabs(val)> 1.e+12 ) sprintf(dspbuf,"%20.12lg",val);
         else sprintf(dspbuf,"%26.12lf",val);
    }
    else {
      if(fabs(val)< 1.e-12 ) {
         if(val==0.0) strcpy(dspbuf,"0");
         else sprintf(dspbuf,"%20.12lg",val);
      }
      else sprintf(dspbuf,"%20.14lf",val);
    }
    l = strlen(dspbuf);
    i=l-1;
    if((dspbuf[0]!='.')) {
      while((i>0)&&(dspbuf[i]=='0'))i--;
      if(dspbuf[i]=='.')i--;
      dspbuf[i+1]='\0';
    }
 }
// else sprintf(dspbuf,"%d",ival);
 kgSplash(tmp,mitem,dspbuf);
 return 1;
}
void Splashmsg(char * msg) {
  char buff[300];
  strcpy(buff,msg);
  kgSplash(Dsp,mitem,buff);
//  kgUpdateOn(Dsp);
}
static int CheckDot(char *buf) {
  int ret=0,i=0;
  while(buf[i]!='\0') if(buf[i]=='.'){ret=1;break;}else i++;
  return ret;
}
static int CheckOp(int ch) {
  if( (ch=='-')||(ch=='+')||(ch=='*')||(ch=='/')||(ch=='^')||(ch=='%')||(ch=='!') )return 1;
  else return 0;
}
static int CheckLeftBracket(int ch) {
  if( (ch=='(') )return 1;
  else return 0;
}
static int CheckRightBracket(int ch) {
  if( (ch==')') )return 1;
  else return 0;
}
static int CheckExpr(char *buf){
  int left=0,i=0,ch;
  while(buf[i]!='\0') {
      if(buf[i]=='(') left++;
      if(buf[i]==')') left--;
      if(left < 0) break;
      i++;
  }
  if(i==0) {Splashmsg((char *)"!!!!"); return 0;}
  if(left != 0) {Splashmsg((char *)"Brackets do not match !!!!"); return 0;}
  ch = buf[--i];
  while((i>0)&&(ch==')')) ch =buf[--i];
  if( CheckOp(ch)) {Splashmsg((char *)"Incomplete Expression");return 0;}
  return 1;
}
int GetBracketCount(char *buf) {
  int left=0,i=0;
  while(buf[i]!='\0') {
    if(buf[i]=='(')left++;
    if(buf[i]==')') left--;
    i++;
  }
  return left;
}
int CleanRightBlank(char *buf) {
  int l,i;
  l = strlen(buf);
  i = l-1;
  while(i>=0) {if(buf[i]==' ')buf[i]='\0';else break;i--;}
  return i+1;
}
int AddRightBracket(int op) {
   char opbuf[5];
   CleanRightBlank(exprbuff);
   sprintf(opbuf,"%c",op);
      if(valbuf[0]!='\0') {
         if(CheckOp(exprbuff[strlen(exprbuff)-1])||CheckLeftBracket(exprbuff[strlen(exprbuff)-1])) {
           strcpy(cpybuff,exprbuff);
           if(!Strcat(cpybuff,valbuf)) return 0;
           if(!Strcat(cpybuff,opbuf)) return 0;
           strcpy(exprbuff,cpybuff);
           valbuf[0]='\0';
           dot=0;
           kgSetstring(Dsp,tbox,0,exprbuff);
           kgUpdateWidget(kgGetWidget(Dsp,tbox));
           Splashmsg((char *)"0");
         }
      }
      else if(exprbuff[0]!='\0') {
         if(!CheckOp(exprbuff[strlen(exprbuff)-1])&&(!CheckLeftBracket(exprbuff[strlen(exprbuff)-1]))) {
           strcpy(cpybuff,exprbuff);
           if(!Strcat(cpybuff,opbuf)) return 0;
           strcpy(exprbuff,cpybuff);
           valbuf[0]='\0';
           dot=0;
           kgSetstring(Dsp,tbox,0,exprbuff);
           kgUpdateWidget(kgGetWidget(Dsp,tbox));
           Splashmsg((char *)"0");
         }
      }
      return 1;
}
char *getSubExpr(char *expr,char *subexpr) {
  int ln,i,Rb=0,Lb=0;
  char *pt=NULL;
  ln = strlen(expr);
  i = ln-1;
  if (i>= 0) {
    while(expr[i]==' ') { expr[i]='\0';i--; if(i< 0) break; }
    while(i>=0) {
      switch(expr[i]) {
        case ')': Rb++; break;
        case '(': Rb--; break;
        default:
          if(Rb==0) {
            if(CheckOp(expr[i])) {
              if(expr[i+1]!='\0') {
                strcpy(subexpr,expr+i+1);
                pt=subexpr;
                expr[i+1]='\0';
              }
              return pt;

            }
          }
          break;
      }
      i--;
    }
  }
  return pt;
}
int AddBracket(int op) {
   char opbuf[5];
   CleanRightBlank(exprbuff);
   if(op==')') {
      if(GetBracketCount(exprbuff)> 0) {
        AddRightBracket(')');
      }
   }
   else {
      sprintf(opbuf,"%c",op);
      if(valbuf[0]=='\0') {
       if(exprbuff[0]!='\0') {
         if(CheckOp(exprbuff[strlen(exprbuff)-1])||CheckLeftBracket(exprbuff[strlen(exprbuff)-1])) {
          if(op=='(') {
           strcpy(cpybuff,exprbuff);
           if(!Strcat(cpybuff,opbuf)) return 0;
           strcpy(exprbuff,cpybuff);
           valbuf[0]='\0';
           dot=0;
           kgSetstring(Dsp,tbox,0,exprbuff);
           kgUpdateWidget(kgGetWidget(Dsp,tbox));
           Splashmsg((char *)"0");
          }
         }
       }
       else {
          if(op=='(') {
           BackupExpr();
           Added=0;
           strcpy(exprbuff,opbuf);
           valbuf[0]='\0';
           dot=0;
           kgSetstring(Dsp,tbox,0,exprbuff);
           kgUpdateWidget(kgGetWidget(Dsp,tbox));
           Splashmsg((char *)"0");
          }
       }
      }
   }
   Added=0;
   return 1;
}
int AddFun(char *fun) {
   char opbuf[5];
   CleanRightBlank(exprbuff);
      if(valbuf[0]!='\0') {
         if(CheckOp(exprbuff[strlen(exprbuff)-1])||CheckLeftBracket(exprbuff[strlen(exprbuff)-1])) {
             strcpy(cpybuff,exprbuff);
             if(!Strcat(cpybuff,fun)) return 0;
             strcpy(exprbuff,cpybuff);
         }
         else {
          BackupExpr();
          Added=0;
          strcpy(exprbuff,fun);
         }
         strcpy(cpybuff,exprbuff);
         if(!Strcat(cpybuff,(char *)"(")) return 0;
         if(!Strcat(cpybuff,valbuf)) return 0;
         if(!Strcat(cpybuff,(char *)")")) return 0;
         strcpy(exprbuff,cpybuff);
         valbuf[0]='\0';
         dot=0;
         kgSetstring(Dsp,tbox,0,exprbuff);
         kgUpdateWidget(kgGetWidget(Dsp,tbox));
         if(CheckExpr(exprbuff)) {
            if(!evalexpr(exprbuff,&val)) {
             Splashmsg((char *)"Error !! ");
            }
            else calcUpdate(Dsp,val);
            valbuf[0]='\0';
            dot=0;
         }
      }
      else if(exprbuff[0]!='\0') {
         if(getSubExpr(exprbuff,subexpr)!= NULL) {
           strcpy(cpybuff,exprbuff);
           if(!Strcat(cpybuff,fun)) return 0;
           if(!Strcat(cpybuff,(char *)"(")) return 0;
           if(!Strcat(cpybuff,subexpr)) return 0;
           if(!Strcat(cpybuff,(char *)")")) return 0;
           strcpy(exprbuff,cpybuff);
           valbuf[0]='\0';
           dot=0;
           kgSetstring(Dsp,tbox,0,exprbuff);
           kgUpdateWidget(kgGetWidget(Dsp,tbox));
           if(CheckExpr(exprbuff)) {
            if(!evalexpr(exprbuff,&val)) {
               Splashmsg((char *)"Error !! ");
            }
            else calcUpdate(Dsp,val);
            valbuf[0]='\0';
            dot=0;
           }
         }
         else {
           if((exprbuff[0]!='\0')&&(!CheckOp(exprbuff[strlen(exprbuff)-1]))&&(!GetBracketCount(exprbuff))) {
             
             strcpy(cpybuff,fun);
             Strcat(cpybuff,(char *)"(");
             if(!Strcat(cpybuff,exprbuff)) return 0;
             if(!Strcat(cpybuff,(char *)")")) return 0;
             strcpy(subexpr,exprbuff);
             strcpy(exprbuff,cpybuff);
             valbuf[0]='\0';
             dot=0;
             kgSetstring(Dsp,tbox,0,exprbuff);
             kgUpdateWidget(kgGetWidget(Dsp,tbox));
             if(CheckExpr(exprbuff)) {
              if(!evalexpr(exprbuff,&val)) {
               Splashmsg((char *)"Error !! ");
              }
              else calcUpdate(Dsp,val);
              valbuf[0]='\0';
              dot=0;
             }
           }
           else {
              Splashmsg((char *)"Incomplete Expression");
              return 0;
           }
         }
     }
     Added=0;
      return 1;
}
int AddOp(int op) {
   char opbuf[5];
   CleanRightBlank(exprbuff);
   sprintf(opbuf,"%c",op);
      if(valbuf[0]!='\0') {
         if(CheckOp(exprbuff[strlen(exprbuff)-1])||CheckLeftBracket(exprbuff[strlen(exprbuff)-1])) {
           strcpy(cpybuff,exprbuff);
           if(!Strcat(cpybuff,valbuf)) return 0;
           strcpy(exprbuff,cpybuff);
         }
         else {
           BackupExpr();
           Added=0;
           strcpy(exprbuff,valbuf);
         }
         strcpy(cpybuff,exprbuff);
         if(!Strcat(cpybuff,opbuf)) return 0;
         strcpy(exprbuff,cpybuff);
         valbuf[0]='\0';
         dot=0;
         kgSetstring(Dsp,tbox,0,exprbuff);
         kgUpdateWidget(kgGetWidget(Dsp,tbox));
         Splashmsg((char *)"0");
      }
      else if(exprbuff[0]!='\0') {
            if(CheckOp(exprbuff[strlen(exprbuff)-1])) {
               exprbuff[strlen(exprbuff)-1]='\0';
            }
            else {
#if 0
              if((op!='+')&&(op!='-')) {
                strcpy(cpybuff,"(");
                Strcat(cpybuff,exprbuff);
                Strcat(cpybuff,")");
                strcpy(exprbuff,cpybuff);
              }
#endif
            }
            strcpy(cpybuff,exprbuff);
            if(!Strcat(cpybuff,opbuf)) return 0;
            strcpy(exprbuff,cpybuff);
            valbuf[0]='\0';
            dot=0;
            kgSetstring(Dsp,tbox,0,exprbuff);
            kgUpdateWidget(kgGetWidget(Dsp,tbox));
            Splashmsg((char *)"0");
         }
         else {
           if(op=='-'){
             BackupExpr();
             Added=0;
             strcpy(exprbuff,(char *)"-");
             kgSetstring(Dsp,tbox,0,exprbuff);
             kgUpdateWidget(kgGetWidget(Dsp,tbox));
             Splashmsg((char *)"0");
           }
         }
         Added=0;
      return 1;
}
int  kgcalcbutnbox1callback(int key,int i,void *Tmp) {
  DIA *D;DIB *B; 
  int n,ret =0; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].b;
  n = B->nx*B->ny;
  switch(key) {
    case 1: 
      if(CheckDot(valbuf)==0) {
        if(val==0.0) strcpy(valbuf,(char *)"1");
        else strcat(valbuf,(char *)"1");
      }
      else strcat(valbuf,(char *)"1");
      sscanf(valbuf,"%lf",&val);
      kgSplash(Tmp,mitem,valbuf);
      break;
    case 2: 
      if(CheckDot(valbuf)==0) {
        if(val==0.0) strcpy(valbuf,(char *)"2");
        else strcat(valbuf,(char *)"2");
      }
      else strcat(valbuf,(char *)"2");
      sscanf(valbuf,"%lf",&val);
      kgSplash(Tmp,mitem,valbuf);
      break;
    case 3: 
      if(CheckDot(valbuf)==0) {
        if(val==0.0) strcpy(valbuf,(char *)"3");
        else strcat(valbuf,(char *)"3");
      }
      else strcat(valbuf,(char *)"3");
      sscanf(valbuf,"%lf",&val);
      kgSplash(Tmp,mitem,valbuf);
      break;
    case 4: 
      AddOp('+');
      break;
    case 5: 
      if(CheckDot(valbuf)==0) {
        if(val==0.0) strcpy(valbuf,(char *)"4");
        else strcat(valbuf,(char *)"4");
      }
      else strcat(valbuf,(char *)"4");
      sscanf(valbuf,"%lf",&val);
      kgSplash(Tmp,mitem,valbuf);
      break;
    case 6: 
      if(CheckDot(valbuf)==0) {
        if(val==0.0) strcpy(valbuf,(char *)"5");
        else strcat(valbuf,(char *)"5");
      }
      else strcat(valbuf,(char *)"5");
      sscanf(valbuf,"%lf",&val);
      kgSplash(Tmp,mitem,valbuf);
      break;
    case 7: 
      if(CheckDot(valbuf)==0) {
        if(val==0.0) strcpy(valbuf,(char *)"6");
        else strcat(valbuf,(char *)"6");
      }
      else strcat(valbuf,(char *)"6");
      sscanf(valbuf,"%lf",&val);
      kgSplash(Tmp,mitem,valbuf);
      break;
    case 8: 
      AddOp('-');
      break;
    case 9: 
      if(CheckDot(valbuf)==0) {
        if(val==0.0) strcpy(valbuf,(char *)"7");
        else strcat(valbuf,(char *)"7");
      }
      else strcat(valbuf,(char *)"7");
      sscanf(valbuf,"%lf",&val);
      kgSplash(Tmp,mitem,valbuf);
      break;
    case 10: 
      if(CheckDot(valbuf)==0) {
        if(val==0.0) strcpy(valbuf,(char *)"8");
        else strcat(valbuf,(char *)"8");
      }
      else strcat(valbuf,(char *)"8");
      sscanf(valbuf,"%lf",&val);
      kgSplash(Tmp,mitem,valbuf);
      break;
    case 11: 
      if(CheckDot(valbuf)==0) {
        if(val==0.0) strcpy(valbuf,(char *)"9");
        else strcat(valbuf,(char *)"9");
      }
      else strcat(valbuf,(char *)"9");
      sscanf(valbuf,"%lf",&val);
      kgSplash(Tmp,mitem,valbuf);
      break;
    case 12: 
      AddOp('*');
      break;
    case 13: 
      if(CheckDot(valbuf)==0) {
        if(val==0.0) strcpy(valbuf,(char *)"0");
        else strcat(valbuf,(char *)"0");
      }
      else {
        strcat(valbuf,(char *)"0");
      }
      sscanf(valbuf,"%lf",&val);
      kgSplash(Tmp,mitem,valbuf);
      break;
    case 14: 
      if(CheckDot(valbuf)==0) {
       dot=1;
       strcat(valbuf,(char *)".");
       sscanf(valbuf,"%lf",&val);
       kgSplash(Tmp,mitem,valbuf);
      }
      break;
    case 15: 
      if(exprbuff[0]!='\0') {
       val =0.0; 
       strcpy(exprbuff,kgGetstring(Dsp,tbox,0));
       if(valbuf[0]!='\0') {
         if(!CheckOp(exprbuff[strlen(exprbuff)-1])&&
            (!CheckLeftBracket(exprbuff[strlen(exprbuff)-1]))) {
             BackupExpr();
             Added=0;
             exprbuff[0]='\0';
         }
       }
       strcpy(cpybuff,exprbuff);
       if(!Strcat(cpybuff,valbuf)) return 0;
       strcpy(exprbuff,cpybuff);
       kgSetstring(Dsp,tbox,0,exprbuff);
       kgUpdateWidget(kgGetWidget(Dsp,tbox));
       valbuf[0]='\0';
       if(CheckExpr(exprbuff)) {
        if(!evalexpr(exprbuff,&val)) {
         Splashmsg((char *)"Error !! ");break;
        }
        else {
         calcUpdate(Tmp,val);
         if((exprbuff[strlen(exprbuff)-1]!=')')||(exprbuff[0]!='(')) {
           strcpy(cpybuff,(char *)"(");
           Strcat(cpybuff,exprbuff);
           Strcat(cpybuff,(char *)")");
           strcpy(exprbuff,cpybuff);
           kgSetstring(Dsp,tbox,0,exprbuff);
           kgUpdateWidget(kgGetWidget(Dsp,tbox));
         }
        }
       }
       kgUpdateWidget(kgGetWidget(Dsp,tbox));
      }
      break;
    case 16: 
      AddOp('/');
      break;
  }
  kgSetCurrentWidget(Tmp,6);
  return ret;
}
int  kgcalcbutnbox2callback(int key,int i,void *Tmp) {
  DIA *D;DIB *B; 
  int n,ret =0; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].b;
  n = B->nx*B->ny;
  switch(key) {
    case 1: 
      val=0;
      calcUpdate(Tmp,val);
      valbuf[0]='\0';
      dot=0;
      break;
    case 2: 
      val=0;
      Splashmsg((char *)"0");
      valbuf[0]='\0';
      dot=0;
      BackupExpr();
      Added=0;
      exprbuff[0]='\0';
      kgSetstring(Dsp,tbox,0,exprbuff);
      kgUpdateWidget(kgGetWidget(Dsp,tbox));
      break;
    case 3: 
      ret=1;
      break;
    case 4: 
      if(valbuf[0]!='\0') {
       valbuf[strlen(valbuf)-1]='\0';
       if(valbuf[0]!='\0') Splashmsg(valbuf);
       else Splashmsg((char *)"0");
      }
      break;
    case 5: 
      AddBracket('(');
      break;
    case 6: 
      AddBracket(')');
      break;
    case 7: 
      mem1+=val;
      break;
    case 8: 
      val=mem1;
      calcUpdate(Tmp,mem1);
      Dspcpy(valbuf,dspbuf);
      break;
    case 9: 
      mem1=0;
      break;
    case 10: 
      mem2+=val;
      break;
    case 11: 
      val=mem2;
      calcUpdate(Tmp,mem2);
      Dspcpy(valbuf,dspbuf);
      break;
    case 12: 
      mem2=0;
      break;
  }
  kgSetCurrentWidget(Tmp,6);
  return ret;
}
int  kgcalcbutnbox3callback(int key,int i,void *Tmp) {
  DIA *D;DIB *B; 
  int n,ret =0; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].b;
  n = B->nx*B->ny;
  val = (val/45.0)*atan(1.0);
  calcUpdate(Tmp,val);
  Dspcpy(valbuf,dspbuf);
  switch(key) {
    case 1: 
      break;
  }
  kgSetCurrentWidget(Tmp,6);
  return ret;
}
int  kgcalcbutnbox4callback(int key,int i,void *Tmp) {
  DIA *D;DIB *B; 
  int n,ret =0; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].b;
  n = B->nx*B->ny;
  val = (val*45.0)/atan(1.0);
  calcUpdate(Tmp,val);
  Dspcpy(valbuf,dspbuf);
  switch(key) {
    case 1: 
      break;
  }
  kgSetCurrentWidget(Tmp,6);
  return ret;
}
int  kgcalcbutnbox5callback(int key,int i,void *Tmp) {
  DIA *D;DIB *B; 
  int n,ret =0; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].b;
  n = B->nx*B->ny;
  val= atan(1.0)*4;
  calcUpdate(Tmp,val);
  Dspcpy(valbuf,dspbuf);
  switch(key) {
    case 1: 
      break;
  }
  kgSetCurrentWidget(Tmp,6);
  return ret;
}
int  kgcalcbutnbox6callback(int key,int i,void *Tmp) {
  DIA *D;DIB *B; 
  int n,ret =0; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].b;
  n = B->nx*B->ny;
  val= exp(1.0);
  calcUpdate(Tmp,val);
  Dspcpy(valbuf,dspbuf);
  switch(key) {
    case 1: 
      break;
  }
  kgSetCurrentWidget(Tmp,6);
  return ret;
}
int  kgcalctextbox1callback(int key,int i,void *Tmp) {
  DIA *D;DIT *T;T_ELMT *e; 
  int ret=1;
  D = ((DIALOG *)Tmp)->d;
  T = D[i].t;
  e = T->elmt;
  strcpy(exprbuff,kgGetstring(Tmp,i,0));
  if(CheckExpr(exprbuff)) {
       if(evalexpr(exprbuff,&val)){
         calcUpdate(Tmp,val);
         valbuf[0]='\0';
         dot=0;
         
       }
       else Splashmsg((char *)"Error !!!!!!");
  }
  Added=0;
  kgUpdateOn(Tmp);
  return ret;
}
double Fact(double v) {
  if(v<2) return 1;
  else return v*Fact(v-1);
}
int  kgcalcbutnbox7callback_o(int key,int i,void *Tmp) {
  DIA *D;DIB *B; 
  int n,ret =0; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].b;
  n = B->nx*B->ny;
  switch(key) {
    case 1: 
      val = sin((val/45.0)*atan(1.0));
      calcUpdate(Tmp,val);
      break;
    case 2: 
      val = cos((val/45.0)*atan(1.0));
      calcUpdate(Tmp,val);
      break;
    case 3: 
      val = tan((val/45.0)*atan(1.0));
      calcUpdate(Tmp,val);
      break;
    case 4: 
      val = -val;
      calcUpdate(Tmp,val);
      break;
    case 5: 
      if(fabs(val)<= 1.0) {
       val = asin(val)*45.0/atan(1.0);
       calcUpdate(Tmp,val);
      }
      else {
       val=0;
       strcpy(valbuf,(char *)"Error !!!");
       kgSplash(Tmp,mitem,valbuf);
       valbuf[0]='\0';
       dot=0;
      }
      break;
    case 6: 
      if(fabs(val)<= 1.0) {
       val = acos(val)*45.0/atan(1.0);
       calcUpdate(Tmp,val);
      }
      else {
       val=0;
       strcpy(valbuf,(char *)"Error !!!!!!  ");
       kgSplash(Tmp,mitem,valbuf);
       valbuf[0]='\0';
       dot=0;
      }
      break;
    case 7: 
      if(val==0.0) {
       val=0;
       strcpy(valbuf,(char *)"Error !!!!!!  ");
       kgSplash(Tmp,mitem,valbuf);
       valbuf[0]='\0';
       dot=0;
      }
      else {
       val = atan(val)*45.0/atan(1.0);
       calcUpdate(Tmp,val);
      }
      break;
    case 8: 
      break;
    case 9: 
      if(val<=0.0) {
       val=0;
       strcpy(valbuf,(char *)"Error !!!!!!");
       kgSplash(Tmp,mitem,valbuf);
       valbuf[0]='\0';
       dot=0;
      }
      else {
       val = sqrt(val);
       calcUpdate(Tmp,val);
      }
      break;
    case 10: 
      val = (val*val);
      calcUpdate(Tmp,val);
      break;
    case 11: 
      break;
    case 12: 
      if(val<=0.0) {
       val=0;
       strcpy(valbuf,(char *)"Error !!!");
       kgSplash(Tmp,mitem,valbuf);
       valbuf[0]='\0';
       dot=0;
      }
      else {
       val = 1.0/(val);
       calcUpdate(Tmp,val);
      }
      break;
    case 13: 
      val = exp(val);
      calcUpdate(Tmp,val);
      break;
    case 14: 
      if(val<=0.0) {
       val=0;
       strcpy(valbuf,(char *)"Error !!!");
       kgSplash(Tmp,mitem,valbuf);
       valbuf[0]='\0';
       dot=0;
      }
      else {
       val = log(val);
       calcUpdate(Tmp,val);
      }
      break;
    case 15: 
      if(val<=0.0) {
       val=0;
       strcpy(valbuf,(char *)"Error !!!");
       kgSplash(Tmp,mitem,valbuf);
       valbuf[0]='\0';
       dot=0;
      }
      else {
       val = log10(val);
       calcUpdate(Tmp,val);
      }
      break;
    case 16: 
      if(val<1.0) {
       val=0;
       strcpy(valbuf,(char *)"Error !!!");
       kgSplash(Tmp,mitem,valbuf);
       valbuf[0]='\0';
       dot=0;
      }
      else {
       val = Fact(val);
       calcUpdate(Tmp,val);
      }
      break;
  }
  kgSetCurrentWidget(Tmp,6);
  return ret;
}
int  kgcalcbutnbox7callback(int key,int i,void *Tmp) {
  DIA *D;DIB *B; 
  int n,ret =0; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].b;
  n = B->nx*B->ny;
  switch(key) {
    case 1: 
      AddFun((char *)"sin");
      break;
    case 2: 
      AddFun((char *)"cos");
      break;
    case 3: 
      AddFun((char *)"tan");
      break;
    case 4: 
      AddFun((char *)"csign");
      break;
    case 5: 
      AddFun((char *)"asin");
      break;
    case 6: 
      AddFun((char *)"acos");
      break;
    case 7: 
      AddFun((char *)"atan");
      break;
    case 8: 
      AddOp('%');
      break;
    case 9: 
      AddFun((char *)"sqrt");
      break;
    case 10: 
      AddFun((char *)"sqr");
      break;
    case 11: 
      AddOp('^');
      break;
    case 12: 
      AddFun((char *)"recep");
      break;
    case 13: 
      AddFun((char *)"exp");
      break;
    case 14: 
      AddFun((char *)"ln");
      break;
    case 15: 
      AddFun((char *)"log");
      break;
    case 16: 
      AddOp('!');
      break;
  }
  kgSetCurrentWidget(Tmp,6);
  return ret;
}
int  kgcalcbutnbox8callback(int key,int i,void *Tmp) {
  char *pt;
  DIA *D;DIB *B; 
  int n,ret =0; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].b;
  n = B->nx*B->ny;
//  if(!Added||(valbuf[0]!='\0'))BackupExpr();
  if(!Added)BackupExpr();
  switch(key) {
    case 1: 
#if 0
      pt = GetFrecord();
      if(pt==NULL) {
       BackupExpr();
       pt = GetBrecord();
      }
#endif
      pt = GetBrecord();
      if(pt!=NULL) strcpy(exprbuff,pt);
      break;
    case 2: 
      pt = GetFrecord();
      if(pt!=NULL) strcpy(exprbuff,pt);
      else {
        Resetlink(Elist);
        pt = GetFrecord();
        if(pt!=NULL) strcpy(exprbuff,pt);
      }
      break;
  }
  kgSetstring(Dsp,tbox,0,exprbuff);
  kgUpdateWidget(kgGetWidget(Dsp,tbox));
  kgSetCurrentWidget(Tmp,6);
  return ret;
}
int  kgcalcbutnbox9callback(int key,int i,void *Tmp) {
  DIA *D;DIB *B; 
  int n,ret =0,ln; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].b;
  n = B->nx*B->ny;
  ln = strlen(exprbuff);
  if(ln!=0) exprbuff[ln-1]='\0';
  kgSetstring(Dsp,tbox,0,exprbuff);
  kgUpdateWidget(kgGetWidget(Dsp,tbox));
  switch(key) {
    case 1: 
      break;
  }
  kgSetCurrentWidget(Tmp,6);
  return ret;
}
int  kgcalcbutnbox10callback(int key,int i,void *Tmp) {
  DIA *D;DIB *B; 
  int n,ret =0; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].b;
  n = B->nx*B->ny;
  switch(key) {
    case 1: 
      break;
  }
  kgSetCurrentWidget(Tmp,6);
  return ret;
}
int kgcalcinit(void *Tmp) {
  int ret = 1;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  str1[0]=str2[0]=valbuf[0]=exprbuff[0]='\0';
  Dsp=Tmp;
  Elist=Dopen();
  SetConvFac(0);
  D->df=6;
  return ret;
}
int kgcalcCallBack(void *Tmp,void *tmp) {
  int ret = 0;
  DIALOG *D;
  KBEVENT *kbe;
  D = (DIALOG *)Tmp;
  kbe = (KBEVENT *)tmp;
  if(kbe->event ==1) {
    if(kbe->button ==1) {
    }
  }
  return ret;
}
int  kgcalcdslide1callback(int val,int i,void *Tmp) {
  /*********************************** 
    val : current value 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIHB *SD; 
  DIP *p1,*p2;
  int ret=1; 
  D = (DIALOG *)Tmp;
  SD = (DIHB *) kgGetWidget(Tmp,i);
  p1 = (DIP *)kgGetWidget(Tmp,15);
  p2 = (DIP *)kgGetWidget(Tmp,16);
//  printf("val=%d\n",val);
  if(val >8) {
    p1->xpm = Offimg;
    p2->xpm = Onimg;
    kgWrite(kgGetWidget(D,13),(char *)"!f05Deg.");
    kgWrite(kgGetWidget(D,14),(char *)"!f07!c09Rad.");
    kgUpdateWidget(p1);
    kgUpdateWidget(p2);
    SetConvFac(1);
  }
  else  {
    p2->xpm = Offimg;
    p1->xpm = Onimg;
    kgWrite(kgGetWidget(D,13),(char *)"!f07!c09Deg.");
    kgWrite(kgGetWidget(D,14),(char *)"!f05Rad.");
    kgUpdateWidget(p1);
    kgUpdateWidget(p2);
    SetConvFac(0);
  }
  kgSetCurrentWidget(Tmp,6);
  kgUpdateOn(Tmp);
  return ret;
}
int kgcalccleanup(void *Tmp) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  return ret;
}
int kgcalcResizeCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 0;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  kgRedrawDialog(D);
  return ret;
}
