#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /* for fork() */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* fpr wait() */
#include <fcntl.h>

int main()
{
  char *ls[] = {"ls", NULL};
  char *grep[] = {"grep", "pipe", NULL};
  char *wc[] = {"wc", NULL};
  char **cmd[] = {ls, grep, wc, NULL};
  int i;

  char *raray[] = {"echo", "hello", ">", "ROAR", NULL};

  // printf("%s\n",*cmd[0] );
  // printf("%s\n",*cmd[1] );
  // printf("%s\n",*cmd[2] );

  printf("%s\n",raray[0] );
  printf("%s\n",raray[1] );
  printf("%s\n",raray[2] );
  printf("%s\n",raray[3] );


  printf("\n" );

  raray[1] = NULL;

  printf("%s\n",raray[0] );
  // printf("%s\n",raray[1] );
  printf("%s\n",raray[2] );
  printf("%s\n",raray[3] );

  return (0);
}
