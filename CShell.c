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

/* --- command history entry struct --- */
typedef struct _hisEntry {
  char *entry;
  struct _hisEntry *prev;
  struct _hisEntry *next;
} HisEntry;

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

/* --- save the commands to history --- */
int historifyCmd(char *cmdline, HisEntry *iterator) {
  if (iterator != 0) {
    iterator->next = malloc(sizeof(struct HisEntry));
    HisEntry *prev = iterator; // Save the current entry for prev.
    iterator = iterator->next;
    // Only continue if not out of memory.
    if (iterator != 0) {
      iterator->prev = prev;
      iterator->next = 0;
      iterator->entry = cmdline;
    } else {
      // Return 2 if out of memory.
      return 2;
    }
    return 0;
  }
  // The iterator pointer didn't point to anything.
  return 1;
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

  /* initialize the shell history */
  HisEntry *root; // Root history entry.
  root = malloc(sizeof(struct HisEntry));
  root->prev = 0;
  root->next = 0;
  root->entry = "";
  HisEntry *iterator = root; // The current history entry (start at root).
  
  if (gethostname(hostname)) {

    /* parse commands until exit or ctrl-c */
    while (!terminate) {
      printf("%s", hostname);
      if (cmdline = readline(":# ")) {
	if(*cmdline) {
	  historifyCmd(cmdline, iterator); // Save the cmd to history.
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

