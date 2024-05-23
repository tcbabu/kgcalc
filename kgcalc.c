#include <kulina.h>
#include "kgcalcCallbacks.h"
#if 0
#define RED 200
#define GREEN 215
#define BLUE 200
#else
#if 0
#define RED 200
#define GREEN 220
#define BLUE 200
#else
#define RED 220
#define GREEN 235
#define BLUE 220
#endif
#endif
#if 1
#if 0
#define TRED 220
#define TGREEN 235
#define TBLUE 220
#else
#define TRED 174
#define TGREEN 189
#define TBLUE 174
#endif
#else
#if 0
#define TRED 120
#define TGREEN 130
#define TBLUE 120
#else
#define TRED 60
#define TGREEN 75
#define TBLUE 60
#endif
#endif
int Btype=4;
float Rfac= 0.25;
void *Onimg,*Offimg;
void *pdimg;
void *upimg,*downimg;

void ModifykgcalcGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
   kgDefineColor(79,150,180,180);
   kgDefineColor(71,200,255,250);
   kgDefineColor(72,20,65,50);
   kgDefineColor(73,200,205,200);
   kgDefineColor(74,20,25,20);
   gc->cur_clr= 46;
   gc->GuiFontSize=12;
   gc->InputFontSize=9;
   gc->SplashCharColor=71;
   gc->SplashFillColor=72;
   gc->SplashFont=18;
   gc->txt_fill=73;
   gc->but_char=52;
   gc->txt_fill= 51;
   gc->txt_char=40;
}
int MakeImages(DIALOG *D){
  DIG *fid;
  float size=3.5;
  int fill,r,g,b;
  fill = D->gc.fill_clr;
  kgGetDefaultRGB(fill,&r,&g,&b);
  fid = (DIG *)kgInitImage(2.*size,2.*size,8);
  kgUserFrame(fid,-size,-size,size,size);
  kgChangeColor(fid,3,240,255,0);
  kgChangeColor(fid,fill,r,g,b);
  kgRoundedRectangleFill(fid,0.,0.0,2*size,2*size,0,fill,0.0);
  kgRoundedRectangleFill(fid,0.,0.0,2*size,2*size,0,0,0.5);
  kgRoundedRectangleFill(fid,0.,0.0,2*size,2*size,0,3,0.5);
  kgChangeColor(fid,3,255,255,0);
  kgRoundedRectangleFill(fid,0.,0.0,2*size-2.0,2*size-2.0,0,3,0.5);
  Onimg=kgGetResizedImage(fid);
  kgCloseImage(fid);
  fid = (DIG *)kgInitImage(2.*size,2.*size,8);
  kgUserFrame(fid,-size,-size,size,size);
  kgChangeColor(fid,fill,r,g,b);
  kgChangeColor(fid,3,0,0,0);
  kgRoundedRectangleFill(fid,0.,0.0,2*size,2*size,0,fill,0.0);
  kgRoundedRectangleFill(fid,0.,0.0,2*size-1,2*size-1,0,3,0.5);
  Offimg=kgGetResizedImage(fid);
  kgCloseImage(fid);
  return 1;
}
int kgcalcGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  BUT_STR  *butn0=NULL; 
  kgDefineColor(59,RED,GREEN,BLUE);
  kgDefineColor(60,RED,GREEN,BLUE);
  kgDefineColor(61,RED,GREEN,BLUE);
  kgDefineColor(62,RED,GREEN,BLUE);
  kgDefineColor(63,RED,GREEN,BLUE);
  kgDefineColor(64,RED,GREEN,BLUE);
  kgDefineColor(65,RED,GREEN,BLUE);
  kgDefineColor(66,RED,GREEN,BLUE);
  kgDefineColor(67,RED,GREEN,BLUE);
  MakeImages(D);
#if 0
  kgDefineColor(59,225,220,195);
  kgDefineColor(60,200,180,80);
  kgDefineColor(61,230,180,60);
  kgDefineColor(62,160,210,160);
  kgDefineColor(63,190,160,240);
#endif
  pdimg=kgPowerdownImage(20,255,0,0);
  upimg=kgUpImage(14,0,0,0);
  downimg=kgDownImage(14,0,0,0);
  butn0= (BUT_STR *)malloc(sizeof(BUT_STR)*16);
  butn0[0].sw=1;
  strcpy(butn0[0].title,(char *)"1");
  butn0[0].xpmn=NULL;
  butn0[0].xpmp=NULL;
  butn0[0].xpmh=NULL;
  butn0[0].bkgr=59;
  butn0[0].butncode='1';
  butn0[1].sw=1;
  strcpy(butn0[1].title,(char *)"2");
  butn0[1].xpmn=NULL;
  butn0[1].xpmp=NULL;
  butn0[1].xpmh=NULL;
  butn0[1].bkgr=59;
  butn0[1].butncode='2';
  butn0[2].sw=1;
  strcpy(butn0[2].title,(char *)"3");
  butn0[2].xpmn=NULL;
  butn0[2].xpmp=NULL;
  butn0[2].xpmh=NULL;
  butn0[2].bkgr=59;
  butn0[2].butncode='3';
  butn0[3].sw=1;
  strcpy(butn0[3].title,(char *)"!h43!w42+");
  butn0[3].xpmn=NULL;
  butn0[3].xpmp=NULL;
  butn0[3].xpmh=NULL;
  butn0[3].bkgr=63;
  butn0[3].butncode='+';
  butn0[4].sw=1;
  strcpy(butn0[4].title,(char *)"4");
  butn0[4].xpmn=NULL;
  butn0[4].xpmp=NULL;
  butn0[4].xpmh=NULL;
  butn0[4].bkgr=59;
  butn0[4].butncode='4';
  butn0[5].sw=1;
  strcpy(butn0[5].title,(char *)"5");
  butn0[5].xpmn=NULL;
  butn0[5].xpmp=NULL;
  butn0[5].xpmh=NULL;
  butn0[5].bkgr=59;
  butn0[5].butncode='5';
  butn0[6].sw=1;
  strcpy(butn0[6].title,(char *)"6");
  butn0[6].xpmn=NULL;
  butn0[6].xpmp=NULL;
  butn0[6].xpmh=NULL;
  butn0[6].bkgr=59;
  butn0[6].butncode='6';
  butn0[7].sw=1;
  strcpy(butn0[7].title,(char *)"!w42-");
  butn0[7].xpmn=NULL;
  butn0[7].xpmp=NULL;
  butn0[7].xpmh=NULL;
  butn0[7].bkgr=63;
  butn0[7].butncode='-';
  butn0[8].sw=1;
  strcpy(butn0[8].title,(char *)"7");
  butn0[8].xpmn=NULL;
  butn0[8].xpmp=NULL;
  butn0[8].xpmh=NULL;
  butn0[8].bkgr=59;
  butn0[8].butncode='7';
  butn0[9].sw=1;
  strcpy(butn0[9].title,(char *)"8");
  butn0[9].xpmn=NULL;
  butn0[9].xpmp=NULL;
  butn0[9].xpmh=NULL;
  butn0[9].bkgr=59;
  butn0[9].butncode='8';
  butn0[10].sw=1;
  strcpy(butn0[10].title,(char *)"9");
  butn0[10].xpmn=NULL;
  butn0[10].xpmp=NULL;
  butn0[10].xpmh=NULL;
  butn0[10].bkgr=59;
  butn0[10].butncode='9';
  butn0[11].sw=1;
  strcpy(butn0[11].title,(char *)"!x !z34!f355");
  butn0[11].xpmn=NULL;
  butn0[11].xpmp=NULL;
  butn0[11].xpmh=NULL;
  butn0[11].bkgr=63;
  butn0[11].butncode='*';
  butn0[12].sw=1;
  strcpy(butn0[12].title,(char *)"0");
  butn0[12].xpmn=NULL;
  butn0[12].xpmp=NULL;
  butn0[12].xpmh=NULL;
  butn0[12].bkgr=59;
  butn0[12].butncode='0';
  butn0[13].sw=1;
  strcpy(butn0[13].title,(char *)"!x!x.");
  butn0[13].xpmn=NULL;
  butn0[13].xpmp=NULL;
  butn0[13].xpmh=NULL;
  butn0[13].bkgr=59;
  butn0[13].butncode='.';
  butn0[14].sw=1;
  strcpy(butn0[14].title,(char *)"!x=");
  butn0[14].xpmn=NULL;
  butn0[14].xpmp=NULL;
  butn0[14].xpmh=NULL;
  butn0[14].bkgr=63;
  butn0[14].butncode='=';
  butn0[15].sw=1;
  strcpy(butn0[15].title,(char *)"/");
  butn0[15].xpmn=NULL;
  butn0[15].xpmp=NULL;
  butn0[15].xpmh=NULL;
  butn0[15].bkgr=63;
  butn0[15].butncode='/';
  DIN b0 = { 
    'n',
    184,115,  
    349,249,
    0,2,  
    40, 
    30, 
    4,4, 
    1,0.200000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn0, 
    kgcalcbutnbox1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b0.Wid,(char *)"");
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*12);
  butn1[0].sw=1;
  strcpy(butn1[0].title,(char *)"C");
  butn1[0].xpmn=NULL;
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].bkgr=60;
  butn1[0].butncode='C';
  butn1[1].sw=1;
  strcpy(butn1[1].title,(char *)"AC");
  butn1[1].xpmn=NULL;
  butn1[1].xpmp=NULL;
  butn1[1].xpmh=NULL;
  butn1[1].bkgr=60;
  butn1[1].butncode='A';
  butn1[2].sw=1;
  strcpy(butn1[2].title, (char *)"");
  butn1[2].xpmn=pdimg;
  butn1[2].xpmp=NULL;
  butn1[2].xpmh=NULL;
  butn1[2].bkgr=61;
  butn1[2].butncode='P';
  butn1[3].sw=1;
  strcpy(butn1[3].title,(char *)"!z54!f35\"");
  butn1[3].xpmn=NULL;
  butn1[3].xpmp=NULL;
  butn1[3].xpmh=NULL;
  butn1[3].bkgr=60;
  butn1[3].butncode='M';
  butn1[4].sw=1;
  strcpy(butn1[4].title,(char *)"!x(");
  butn1[4].xpmn=NULL;
  butn1[4].xpmp=NULL;
  butn1[4].xpmh=NULL;
  butn1[4].bkgr=63;
  butn1[4].butncode='(';
  butn1[5].sw=1;
  strcpy(butn1[5].title,(char *)"!x)");
  butn1[5].xpmn=NULL;
  butn1[5].xpmp=NULL;
  butn1[5].xpmh=NULL;
  butn1[5].bkgr=63;
  butn1[5].butncode=')';
  butn1[6].sw=1;
  strcpy(butn1[6].title,(char *)"M+");
  butn1[6].xpmn=NULL;
  butn1[6].xpmp=NULL;
  butn1[6].xpmh=NULL;
  butn1[6].bkgr=64;
  butn1[6].butncode='M';
  butn1[7].sw=1;
  strcpy(butn1[7].title,(char *)"MR");
  butn1[7].xpmn=NULL;
  butn1[7].xpmp=NULL;
  butn1[7].xpmh=NULL;
  butn1[7].bkgr=64;
  butn1[7].butncode='M';
  butn1[8].sw=1;
  strcpy(butn1[8].title,(char *)"MC");
  butn1[8].xpmn=NULL;
  butn1[8].xpmp=NULL;
  butn1[8].xpmh=NULL;
  butn1[8].bkgr=64;
  butn1[8].butncode='M';
  butn1[9].sw=1;
  strcpy(butn1[9].title,(char *)"M+");
  butn1[9].xpmn=NULL;
  butn1[9].xpmp=NULL;
  butn1[9].xpmh=NULL;
  butn1[9].bkgr=64;
  butn1[9].butncode='M';
  butn1[10].sw=1;
  strcpy(butn1[10].title,(char *)"MR");
  butn1[10].xpmn=NULL;
  butn1[10].xpmp=NULL;
  butn1[10].xpmh=NULL;
  butn1[10].bkgr=64;
  butn1[10].butncode='M';
  butn1[11].sw=1;
  strcpy(butn1[11].title,(char *)"MC");
  butn1[11].xpmn=NULL;
  butn1[11].xpmp=NULL;
  butn1[11].xpmh=NULL;
  butn1[11].bkgr=64;
  butn1[11].butncode='M';
  DIN b1 = { 
    'n',
    354,115,  
    483,249,
    0,2,  
    40, 
    30, 
    3,4, 
    1,0.200000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn1, 
    kgcalcbutnbox2callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b1.Wid,(char *)"");
  BUT_STR  *butn2=NULL; 
  butn2= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn2[0].sw=1;
  strcpy(butn2[0].title,(char *)"deg!f352!f23rad");
  butn2[0].xpmn=NULL;
  butn2[0].xpmp=NULL;
  butn2[0].xpmh=NULL;
  butn2[0].bkgr=67;
  butn2[0].butncode='~';
  DIN b2 = { 
    'n',
    14,85,  
    89,112,
    0,0,  
    70, 
    22, 
    1,1, 
    1,0.200000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn2, 
    kgcalcbutnbox3callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b2.Wid,(char *)"");
  BUT_STR  *butn3=NULL; 
  butn3= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn3[0].sw=1;
  strcpy(butn3[0].title,(char *)"rad!f352!f23deg");
  butn3[0].xpmn=NULL;
  butn3[0].xpmp=NULL;
  butn3[0].xpmh=NULL;
  butn3[0].bkgr=67;
  butn3[0].butncode='~';
  DIN b3 = { 
    'n',
    89,85,  
    165,112,
    0,0,  
    70, 
    22, 
    1,1, 
    1,0.200000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn3, 
    kgcalcbutnbox4callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b3.Wid,(char *)"");
  BUT_STR  *butn4=NULL; 
  butn4= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn4[0].sw=1;
  strcpy(butn4[0].title,(char *)"!f36p");
  butn4[0].xpmn=NULL;
  butn4[0].xpmp=NULL;
  butn4[0].xpmh=NULL;
  butn4[0].bkgr=67;
  butn4[0].butncode='~';
  DIN b4 = { 
    'n',
    425,85,  
    452,111,
    0,0,  
    22, 
    22, 
    1,1, 
    1,0.200000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn4, 
    kgcalcbutnbox5callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b4.Wid,(char *)"");
  BUT_STR  *butn5=NULL; 
  butn5= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn5[0].sw=1;
  strcpy(butn5[0].title,(char *)"e");
  butn5[0].xpmn=NULL;
  butn5[0].xpmp=NULL;
  butn5[0].xpmh=NULL;
  butn5[0].bkgr=67;
  butn5[0].butncode='~';
  DIN b5 = { 
    'n',
    452,85,  
    481,111,
    0,0,  
    22, 
    22, 
    1,1, 
    1,0.200000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn5, 
    kgcalcbutnbox6callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b5.Wid,(char *)"");
  T_ELMT *e6  ; 
  e6 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e6[0].fmt = (char *)malloc(5);
  strcpy(e6[0].fmt,(char *)"%45s");
  e6[0].v=(void *)v[0];
  e6[0].sw=1;
  DIT t6 = { 
    't',
    22,50,  
    469,74,
    22, 
    1,1, 
    e6,
    1,1,
    NULL,kgcalctextbox1callback,9,0,0,0 /* args,Call back */
  };
  t6.type=1;
  strcpy(t6.Wid,(char *)"");
  t6.pt=NULL;
  BUT_STR  *butn7=NULL; 
  butn7= (BUT_STR *)malloc(sizeof(BUT_STR)*16);
  butn7[0].sw=1;
  strcpy(butn7[0].title,(char *)"Sin");
  butn7[0].xpmn=NULL;
  butn7[0].xpmp=NULL;
  butn7[0].xpmh=NULL;
  butn7[0].bkgr=65;
  butn7[0].butncode='~';
  butn7[1].sw=1;
  strcpy(butn7[1].title,(char *)"Cos");
  butn7[1].xpmn=NULL;
  butn7[1].xpmp=NULL;
  butn7[1].xpmh=NULL;
  butn7[1].bkgr=65;
  butn7[1].butncode='~';
  butn7[2].sw=1;
  strcpy(butn7[2].title,(char *)"Tan");
  butn7[2].xpmn=NULL;
  butn7[2].xpmp=NULL;
  butn7[2].xpmh=NULL;
  butn7[2].bkgr=65;
  butn7[2].butncode='~';
  butn7[3].sw=1;
  strcpy(butn7[3].title,(char *)"!x+!b!w32_");
  butn7[3].xpmn=NULL;
  butn7[3].xpmp=NULL;
  butn7[3].xpmh=NULL;
  butn7[3].bkgr=66;
  butn7[3].butncode='~';
  butn7[4].sw=1;
  strcpy(butn7[4].title,(char *)"Asin");
  butn7[4].xpmn=NULL;
  butn7[4].xpmp=NULL;
  butn7[4].xpmh=NULL;
  butn7[4].bkgr=65;
  butn7[4].butncode='~';
  butn7[5].sw=1;
  strcpy(butn7[5].title,(char *)"Acos");
  butn7[5].xpmn=NULL;
  butn7[5].xpmp=NULL;
  butn7[5].xpmh=NULL;
  butn7[5].bkgr=65;
  butn7[5].butncode='~';
  butn7[6].sw=1;
  strcpy(butn7[6].title,(char *)"Atan");
  butn7[6].xpmn=NULL;
  butn7[6].xpmp=NULL;
  butn7[6].xpmh=NULL;
  butn7[6].bkgr=65;
  butn7[6].butncode='~';
  butn7[7].sw=1;
  strcpy(butn7[7].title,(char *)"%");
  butn7[7].xpmn=NULL;
  butn7[7].xpmp=NULL;
  butn7[7].xpmh=NULL;
  butn7[7].bkgr=63;
  butn7[7].butncode='~';
  butn7[8].sw=1;
  strcpy(butn7[8].title,(char *)"!f353!f23 !b!S__!e");
  butn7[8].xpmn=NULL;
  butn7[8].xpmp=NULL;
  butn7[8].xpmh=NULL;
  butn7[8].bkgr=66;
  butn7[8].butncode='~';
  butn7[9].sw=1;
  strcpy(butn7[9].title,(char *)"x!S2!e");
  butn7[9].xpmn=NULL;
  butn7[9].xpmp=NULL;
  butn7[9].xpmh=NULL;
  butn7[9].bkgr=66;
  butn7[9].butncode='~';
  butn7[10].sw=1;
  strcpy(butn7[10].title,(char *)"x!Sy!e");
  butn7[10].xpmn=NULL;
  butn7[10].xpmp=NULL;
  butn7[10].xpmh=NULL;
  butn7[10].bkgr=63;
  butn7[10].butncode='~';
  butn7[11].sw=1;
  strcpy(butn7[11].title,(char *)"!x!z67!k!x!x!x!z231!r!x!x!x_!b!y!y!y!d!z13 !z31!z32x");
  butn7[11].xpmn=NULL;
  butn7[11].xpmp=NULL;
  butn7[11].xpmh=NULL;
  butn7[11].bkgr=66;
  butn7[11].butncode='~';
  butn7[12].sw=1;
  strcpy(butn7[12].title,(char *)"e!Sx!e");
  butn7[12].xpmn=NULL;
  butn7[12].xpmp=NULL;
  butn7[12].xpmh=NULL;
  butn7[12].bkgr=66;
  butn7[12].butncode='~';
  butn7[13].sw=1;
  strcpy(butn7[13].title,(char *)"ln x");
  butn7[13].xpmn=NULL;
  butn7[13].xpmp=NULL;
  butn7[13].xpmh=NULL;
  butn7[13].bkgr=66;
  butn7[13].butncode='~';
  butn7[14].sw=1;
  strcpy(butn7[14].title,(char *)"log!s10!ex");
  butn7[14].xpmn=NULL;
  butn7[14].xpmp=NULL;
  butn7[14].xpmh=NULL;
  butn7[14].bkgr=66;
  butn7[14].butncode='~';
  butn7[15].sw=1;
  strcpy(butn7[15].title,(char *)"Mod");
  butn7[15].xpmn=NULL;
  butn7[15].xpmp=NULL;
  butn7[15].xpmh=NULL;
  butn7[15].bkgr=63;
  butn7[15].butncode='~';
  DIN b7 = { 
    'n',
    14,115,  
    179,249,
    0,2,  
    40, 
    30, 
    4,4, 
    1,0.200000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn7, 
    kgcalcbutnbox7callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b7.Wid,(char *)"");
  DIM m8 = { 
    'M',
    43,11,  
    454,44,  
    1,0  
  };
  strncpy(m8.msg,(char *)"0",499);
  strcpy(m8.Wid,(char *)"");
  BUT_STR  *butn9=NULL; 
  butn9= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn9[0].sw=1;
  strcpy(butn9[0].title,(char *)"");
  butn9[0].xpmn=upimg;
  butn9[0].xpmp=NULL;
  butn9[0].xpmh=NULL;
  butn9[0].bkgr=-1;
  butn9[0].butncode='~';
  butn9[1].sw=1;
  strcpy(butn9[1].title,(char *)"");
  butn9[1].xpmn=downimg;
  butn9[1].xpmp=NULL;
  butn9[1].xpmh=NULL;
  butn9[1].bkgr=-1;
  butn9[1].butncode='~';
  DIN b9 = { 
    'n',
    3,50,  
    27,84,
    0,0,  
    18, 
    14, 
    1,2, 
    9,0.100000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn9, 
    kgcalcbutnbox8callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b9.Wid,(char *)"");
  BUT_STR  *butn10=NULL; 
  butn10= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn10[0].sw=1;
  strcpy(butn10[0].title,(char *)"!f35 !b!!");
  butn10[0].xpmn=NULL;
  butn10[0].xpmp=NULL;
  butn10[0].xpmh=NULL;
  butn10[0].bkgr=-1;
  butn10[0].butncode='~';
  DIN b10 = { 
    'n',
    467,52,  
    489,82,
    0,0,  
    19, 
    22, 
    1,1, 
    9,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn10, 
    kgcalcbutnbox9callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b10.Wid,(char *)"");
  BUT_STR  *butn11=NULL; 
  butn11= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn11[0].sw=1;
  strcpy(butn11[0].title,(char *)"Help");
  butn11[0].xpmn=NULL;
  butn11[0].xpmp=NULL;
  butn11[0].xpmh=NULL;
  butn11[0].bkgr=62;
  butn11[0].butncode='~';
  DIN b11 = { 
    'n',
    347,85,  
    405,111,
    0,0,  
    54, 
    22, 
    1,1, 
    1,0.200000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn11, 
    kgcalcbutnbox10callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b11.Wid,(char *)"");
  DIHB d12 = { 
    'P',
    241,92,  
    273,111,   
    0,15,  
    26,  
    (int *)v[1],
    NULL,
    NULL,kgcalcdslide1callback,0,0,2,-250240220 /* args, callback */
  };
  strcpy(d12.Wid,(char *)"kgcalcWidget15");
  DIM m13 = { 
    'm',
    210,94,  
    239,108,  
    1,0  
  };
  strncpy(m13.msg,(char *)"!f07!c09Deg.",499);
  strcpy(m13.Wid,(char *)"kgcalcWidget16");
  DIM m14 = { 
    'm',
    276,94,  
    304,108,  
    -1,0  
  };
  strncpy(m14.msg,(char *)"!f05Rad.",499);
  strcpy(m14.Wid,(char *)"kgcalcWidget17");
  char *xpm15;// Pixmap info
  xpm15 = (char *)Onimg;
  DIP p15 = { 
    'p',
    201,98,  
    210,107,  
    (void *)xpm15,
    -1, /* bkgr colour */ 
      0,0 /* border hide */ 
  };
  strcpy(p15.Wid,(char *)"kgcalcWidget18");
  char *xpm16;// Pixmap info
  xpm16 = (char *)Offimg;
  DIP p16 = { 
    'p',
    306,98,  
    315,107,  
    (void *)xpm16,
    -1, /* bkgr colour */ 
      0,0 /* border hide */ 
  };
  strcpy(p16.Wid,(char *)"kgcalcWidget19");
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+18));
  d =dtmp+i; 
  d[17].t=NULL;
  b0.type=b1.type=b2.type=b3.type=b4.type=b5.type=b7.type=b11.type=Btype;
  b0.fac=b1.fac=b2.fac=b3.fac=b4.fac=b5.fac=b7.fac=b11.fac=Rfac;

  d[0].t = (DIT *)malloc(sizeof(DIN));
  *d[0].N = b0;
  d[0].N->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIN));
  *d[1].N = b1;
  d[1].N->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIN));
  *d[2].N = b2;
  d[2].N->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIN));
  *d[3].N = b3;
  d[3].N->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIN));
  *d[4].N = b4;
  d[4].N->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DIN));
  *d[5].N = b5;
  d[5].N->item = -1;
  d[6].t = (DIT *)malloc(sizeof(DIT));
  *d[6].t = t6;
  d[6].t->item = -1;
  d[7].t = (DIT *)malloc(sizeof(DIN));
  *d[7].N = b7;
  d[7].N->item = -1;
  d[8].t = (DIT *)malloc(sizeof(DIM));
  *d[8].m = m8;
  d[8].m->item = -1;
  d[9].t = (DIT *)malloc(sizeof(DIN));
  *d[9].N = b9;
  d[9].N->item = -1;
  d[10].t = (DIT *)malloc(sizeof(DIN));
  *d[10].N = b10;
  d[10].N->item = -1;
  d[11].t = (DIT *)malloc(sizeof(DIN));
  *d[11].N = b11;
  d[11].N->item = -1;
  d[12].t = (DIT *)malloc(sizeof(DIHB));
  *d[12].B = d12;
  d[12].B->item = -1;
  d[13].t = (DIT *)malloc(sizeof(DIM));
  *d[13].m = m13;
  d[13].m->item = -1;
  d[14].t = (DIT *)malloc(sizeof(DIM));
  *d[14].m = m14;
  d[14].m->item = -1;
  d[15].t = (DIT *)malloc(sizeof(DIP));
  *d[15].p = p15;
  d[15].p->item = -1;
  d[16].t = (DIT *)malloc(sizeof(DIP));
  *d[16].p = p16;
  d[16].p->item = -1;
  d[17].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakekgcalcGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Text_Box1  1 data values
    Integerslidebar1  1 data value

*************************************************/
   char  *v0 ;
   v0 = (char *)malloc(sizeof(100));
   v0[0] = '\0';
   int  *v1 ;
   v1 = (int *)malloc(sizeof(int));
   *v1 = 1;
   void** v=(void **)malloc(sizeof(void*)*2);
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = kgcalcGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v;
   return GrpId;
}

int kgcalc( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  kgColorTheme(&D,TRED,TGREEN,TBLUE);
  ModifykgcalcGc(&(D.gc));    /*  set colors for gui*/
  D.d=NULL;
  GrpId = kgcalcGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 12;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 140;   /* Position of Dialog */ 
  D.yo = 55;
  D.xl = 496;    /*  Length of Dialog */
  D.yl = 265;    /*  Width  of Dialog */
  D.Initfun = kgcalcinit;    /*   init fuction for Dialog */
  D.Cleanupfun = kgcalccleanup;    /*   init fuction for Dialog */
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 1;    /*  1 for Window Decorration */
  D.transparency = 0.2000;    /*  float 1.0 for full transparency */
  D.Newwin = 0;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 1;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 0;    /*  1 for stickyness */
  D.Resize = 0;    /*  1 for Resize option */
  D.MinWidth = 100;    /*   for Resize option */
  D.MinHeight = 100;    /*   for Resize option */
#if 1 
  D.Callback = kgcalcCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = kgcalcResizeCallBack;  /*  Resize callback */
  D.Fixpos = 0;    /*  1 for Fixing Position */
  D.NoTaskBar = 0;    /*  1 for not showing in task bar*/
  D.StackPos = 0;    /* -1,0,1 for for Stack Position -1:below 0:normal 1:above*/
  D.Shapexpm = NULL;    /*  PNG/jpeg file for window shape;Black color will not be drawn */
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = pt;    /*  any data to be passed by user*/
  strcpy(D.name,"Kulina Calculator");    /*  Dialog name you may change */
  if(D.fullscreen!=1) {    /*  if not fullscreen mode */
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
  }
  else {    // for fullscreen
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
     D.xo=D.yo=0; D.xl = xres; D.yl=yres;
  }    /*  end of fullscreen mode */
  ret= kgUi(&D);
  kgCleanUi(&D);
  kgFreeImage(pdimg);
  kgFreeImage(upimg);
  kgFreeImage(downimg);
  kgFreeImage(Onimg);
  kgFreeImage(Offimg);
  return ret;
}
void *Runkgcalc(void *arg) {
/*************************************************

    Text_Box1  1 data values
    Integerslidebar1  1 data value

*************************************************/
   char  v0[2050]=" " ;
   int   v1 = 1;
   void* v[2];
   v[0]=(void *)(v0);
   v[1]=(void *)(&v1);
   void *pt=NULL; /* pointer to send any extra information */
   kgcalc(NULL,v,pt );
   return NULL;
}
