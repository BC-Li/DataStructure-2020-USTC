#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include<assert.h>
#include<math.h>
#include<ctype.h>
#include<omp.h>
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
    // index_t n = 0;
    // index_t m = 0;
    int u, v, w;

    // g->n = (int)23947347;
    // g->m = (int)58333344;
    // while(fp!=NULL) {
        // fread(&u, sizeof(int), 1, fp);
        // // char *line = strlower(in_line);
        //     fread(&v, sizeof(int), 1, fp);
        //     fread(&w, sizeof(int), 1, fp);
        // // sscanf(line, "%d %d %d", &u, &v, &w);
        // printf("%d %d %d\n",u,v,w);
    //     break;
    int s[3];
    while(fread(s,sizeof(int),3,fp)){
        u = s[0];
        v = s[1];
        w = s[2];
        printf("%d %d %d\n",u,v,w);
    }
    // }
}