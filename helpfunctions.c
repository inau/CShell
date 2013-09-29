/* 
   Helpfunctions.c - all the smaller help functions.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"

#include "helpfunctions.h"

/* --- symbolic constants --- */
#define MAXBUF 512
#define PATHBUF 1024

// Find all the programs in the path environment.
int fileexist(char *filename, char** givenprogpath) {
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

int pipecmd(char *filename1, char *argv1[], char *filename2, char *argv2[])
{
	int fd[2];
  int status = 0;
	pipe(fd);
	pid_t pid = fork();
  if(pid == 0) {
    pid_t pid2 = fork();
    if(pid2 == 0){
        //grand-child
        //input of pipe
        /* Close stdin, duplicate the input side of pipe to stdin */
        dup2(fd[0], 0);
        execvp(filename2, argv2);
        close(fd[0]);

        exit(0);
    } else {
  		//child
      //output of pipe
  		/* Close stdout, duplicate the output side of pipe to stdout */
      dup2(fd[1], 1);
  		execvp(filename1, argv1);
  		close(fd[1]);
  		exit(0);
  		waitpid(0, &status, 0);
    }
  } else {
    //parent
    waitpid(0, &status, 0); // Wait for all children?
  }
  return 0;
}

/* --- creates a list of arguments from a list of commands --- */
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

}

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

