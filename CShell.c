/* 

   CShell.c : She sells CShells at the C shore 

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
#define HOSTNAMEMAX 100
#define MAXBUF 512

#define EXIT		("exit")

/* --- symbolic macros --- */
#define isexit(c) ((c) == EXIT)
#define isspec(c) (isexit(c))

/* --- use the /proc filesystem to obtain the hostname --- */
char *gethostname(char *hostname)
{
  char hostname2[MAXBUF];   
  FILE *hostfile;
  printf("Getting host file \n");
  hostfile = fopen ("/proc/sys/kernel/hostname","r");
  if(hostfile == NULL){
      printf("Unable to open hostname file!\n");
      
  }else{
      fgets(hostname, MAXBUF, hostfile);
      hostname[strlen(hostname)-1] = 0;
  }
  //printf(hostname);
  
  return hostname;

}

/* --- execute a shell command --- */
int executeshellcmd (Shellcmd *shellcmd)
{
  // Check if the command is one of the reserved.
  Cmd *firstCmd = shellcmd->the_cmds;
  char *tok = (char*)(firstCmd->cmd);
  if (isspec(tok)) {
    if (strcmp(tok, "exit")) {
      // Return 1 if exit (exits CShell).
      return 1;
    }
  }
  
  printshellcmd(shellcmd);

  return 0;
}

/* --- main loop of the simple shell --- */
int main(int argc, char* argv[]) {

  /* initialize the shell */
  char *cmdline;
  char hostname[HOSTNAMEMAX];
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
    printf("Exiting CShell. Sea you soon!\n");
  }    
    
  return EXIT_SUCCESS;
}

