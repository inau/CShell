/* 

   CShell.c : She sells SChell at the sea shore. 

 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "parser.h"
#include "print.h"

/* --- symbolic constants --- */
#define MAXBUF 512

/* --- use the /proc filesystem to obtain the hostname --- */
char *gethostname()
{
    
  char hostname[MAXBUF];   
  FILE *hostfile;
  printf("Getting host file \n");
  hostfile = fopen ("/proc/sys/kernel/hostname","r");
  if(hostfile == NULL){
      printf("Unable to open hostname file!\n");
      
  }else{
        scanf(hostfile, "%[^\n]", hostname);
      
  }
  printf(hostname);
  return hostname;

}

/* --- execute a shell command --- */
int executeshellcmd (Shellcmd *shellcmd)
{
  printshellcmd(shellcmd);

  return 0;
}

/* --- main loop of the simple shell --- */
int main(int argc, char* argv[]) {

  /* initialize the shell */
  char *cmdline;
  char hostname[MAXBUF];
  int terminate = 0;
  Shellcmd shellcmd;
  
  if (gethostname(hostname)) {

    /* parse commands until exit or ctrl-c */
    while (!terminate) {
      printf("%s", hostname);
      if (cmdline = readline(":# ")) {
	if(*cmdline) {
	  add_history(cmdline);
	  if (parsecommand(cmdline, &shellcmd)) {
	    terminate = executeshellcmd(&shellcmd);
	  }
	}
	free(cmdline);
      } else terminate = 1;
    }
    printf("Exiting CShell... Sea you soon!\n");
  }    
    
  return EXIT_SUCCESS;
}

