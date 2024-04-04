#include <stdio.h>
char d[2000]={' '};int s[1<<9],x,y,dx=1,dy,S,p,a,b,c;
#define C d[y*80+x]
#define P (p?s[--p]:0)
#define Q break;case
#define A(x,v)case v:a=P;s[p++]=P x a;break;
int main(int,char**A){for(FILE*f=fopen(A[1],"r");c=fgetc(f),~c;)if(c-10)C=c,x++;else++y,x=0;c=x=y=0;while(C!='@'||S){if(S&&C!='"')s[p++]=C;else switch(C){case'0'...'9':s[p++]=C-'0';Q'>':dx=1;dy=0;Q'<':dx=-1;dy=0;Q'^':dx=0;dy=-1;Q'v':dx=0;dy=1;Q'|':dx=0;a=P;dy=a?-1:1;Q'_':dx=P?-1:1;dy=0;break;A(+,'+')A(-,'-')A(*,'*')A(/,'/')A(%,'%')case 92:a=P;b=P;s[p++]=a;s[p++]=b;Q'.':a=P;printf("%d ",a);Q',':putchar(P);Q'"':S=!S;Q':':a=P;s[p++]=a;s[p++]=a;Q'!':s[p++]=!P;Q'`':a=P;s[p++]=P>a;Q'#':x+=dx;y+=dy;Q'$':a=P;Q'?':switch(rand()&3){Q 0:dx=1;dy=0;Q 1:dx=-1;dy=0;Q 2:dx=0;dy=-1;Q 3:dx=0;dy=1;}Q'&':scanf("%d",&a);s[p++]=a;Q'~':s[p++]=getchar();Q'g':case'p':a=P;c=P*80+a;if(C=='g')s[p++]=d[c];else d[c]=P;break;}x=(x+dx+80)%80;y=(y+dy+25)%25;}}