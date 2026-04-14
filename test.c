#include <time.h>
#include <string.h>
#include <stdio.h>

int main(){
  struct tm t;
  memset(&t, 0, sizeof t);
  t.tm_year = 126;
  t.tm_mon = 3;
  t.tm_mday = 13;
  mktime(&t);

  printf("%04d/%02d/%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
  printf("\n");

  t.tm_mday += 20;
  mktime(&t);
  printf("%04d/%02d/%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
  printf("\n");

  return 0;
}
