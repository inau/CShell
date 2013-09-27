/* 
   Helpfunctions.c - all the smaller help functions.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "parser.h"
#include "print.h"
#include <signal.h>
#include <unistd.h>

#include "helpfunctions.h"
#include "changeDir.h"

/* --- symbolic constants --- */
#define MAXBUF 512
#define PATHBUF 1024

// Find all the programs in the path environment.
int cmdexist(char *filename, char** givenprogpath) {
  char *path = getenv("PATH");
  if (path != NULL) {
    char pathlist[PATHBUF];
    strcpy(pathlist, path);
    char *progPath, *s = ":";
    progPath = strtok(pathlist, s);

    // Look through the progPaths from PATH.
    while(progPath != NULL) {
      // If the file exists, execute.
      char prog[PATHBUF];
      strcpy(prog, progPath);
      strcat(prog, "/");
      strcat(prog, filename);
      // If file exists, execute it.
      if (access(prog, F_OK) != -1) {
        *givenprogpath = prog;     
				return 1;
			}
      progPath = strtok(NULL, s);
    }
  }
  // If PATH is 0.
  return 0;
}


/* --- creates a list of arguments from a list of commands --- 
void createArgsArray(Cmd *cmdList, char ***args) {
  Cmd *curCmd = cmdList;
  char **arglist = *args;
  while(curCmd != NULL) {
    char **cmd = curCmd->cmd;
    curCmd = curCmd->next;
    
    char **curArg = cmd;
    while(*curArg != NULL) {
      *arglist = *curArg;
      *arglist++;
      *curArg++;
    }
    // End the list with null.
    *arglist = NULL;
  }

}*/

void createArgsArray(char **cmd, char ***args) {
  char **arglist = *args;
	char **curArg = cmd;
  while(*curArg != NULL) {
    *arglist = *curArg;
    *arglist++;
    *curArg++;
  }
	// End the list with null.
  *arglist = NULL;
}

/* --- creates a file --- */
void createfile(char *filepath, int rm) {
	// Check if file exists.
  if (access(filepath, F_OK) != -1) {
		// Remove file, if rm is 1.
		if (rm == 1) {
			remove(filepath);
		}
  }
	printf("Going to open a file!\n");
	FILE *fp = fopen(filepath, "w+");
	fclose(fp);
	printf("File created!\n");
}

