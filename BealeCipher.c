/*Program to decipher Beale's cipher*/
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  char letters[1322];
  char s[32];
  char buf[1024];
  int i = 0;
  int num;
printf("hello\n" );
  FILE* declaration = fopen("declarationOfIndependence.txt", "r");
  FILE* cipher = fopen("cipher.txt", "r");

  while(fscanf(declaration , "%s" , s) != EOF ){
    letters[i++] = s[0];
  }

  i = 0;

  while(fscanf(cipher, "%d", &num) != EOF){
    buf[i++] = letters[(num - 1) % 1322];
  }

  buf[i] = '\0';


  printf("%s\n", buf );

  fclose(declaration);
  fclose(cipher);

  return 0;
}
