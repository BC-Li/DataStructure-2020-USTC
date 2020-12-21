#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>

#define MAXLEN 1024

char *strlower(char *s)
{
  char *t;
  for (t = s; *s != '\0'; s++)
    *s = (char)tolower(*s);
  return (t);
}

int main()

{

  FILE *outfile, *infile;

  outfile = fopen("out.gr", "wb");

  infile = fopen("in.gr", "r");

  char buf[MAXLEN];
  int buf_int[MAXLEN];
  int rc;
  char u[10], v[10], w[10];
  int u_i, v_i, w_i;

  while (fgets(buf, MAXLEN, infile) != NULL)
  {
    char *line = strlower(buf);
    sscanf(line, "%s %s %s", u, v, w);
    u_i = atoi(u);
    v_i = atoi(v);
    w_i = atoi(w);
    fwrite(&u_i, sizeof(int), 1, outfile);
    fwrite(&v_i, sizeof(int), 1, outfile);
    fwrite(&w_i, sizeof(int), 1, outfile);
  }

  fclose(infile);

  fclose(outfile);

  system("PAUSE");

  return 0;
}