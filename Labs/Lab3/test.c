#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include<assert.h>
#include<math.h>

 char* strlower( char *s)
{
   char* t;
   for(t = s; *s != '\0'; s++)
      *s = (char)tolower(*s);

   return(t);
}

int main(){
    FILE *fp;
     fp = fopen("USAbin.gr", "rb");
     char in_line[1024];
    int u, v, w;
    int s[3];
    while(fread(s,sizeof(int),3,fp)){
        u = s[0];
        v = s[1];
        w = s[2];
        printf("%d %d %d\n",u,v,w);
    }
}