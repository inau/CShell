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
#include <signal.h>
#include <unistd.h>

/* --- symbolic constants --- */
#define HOSTNAMEMAX 100
#define MAXBUF 512

#define EXIT	("exit")

/* --- symbolic macros --- */
#define isexit(c) (strcmp(c,EXIT) == 0)
#define isspec(c) (isexit(c))

/* --- buffer sizes --- */
#define PATHBUF 512

pid_t processID = 0;

/* --- use the /proc filesystem to obtain the hostname --- */
char *gethostname2(char *hostname)
{
  char hostname2[MAXBUF];   
  FILE *hostfile;
  printf("Getting host file \n");
  hostfile = fopen ("/proc/sys/kernel/hostname","r");
  if(hostfile == NULL){
      printf("Unable to open hostname file!\n");
      
  }else{
      printf("Sally sells Cshells by the Cshore. << Top nizzle sonz \n");
      fgets(hostname, MAXBUF, hostfile);
      hostname[strlen(hostname)-1] = 0;
  }
  
  return hostname;

}


void killProcessId(){
    if(processID != 0){
        kill(processID, SIGTERM);
        processID = 0;
    }
    else
        printf("No child to kill");
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

/* --- execute a shell command --- */
int executeshellcmd (Shellcmd *shellcmd)
{
  // Check if the command is one of the reserved.
  Cmd *firstCmd = shellcmd->the_cmds;
  char *tok = *(firstCmd->cmd);
  if (isspec(tok)) {
    if (isexit(tok)) {
      // Return 1 if exit (exits CShell).
      return 1;
    }
  }

  // Find all the programs in the path environment.
  char *path = getenv("PATH");
  if (path != NULL) {
    char pathlist[1024];
    strcpy(pathlist, path);
    char *progPath, *s = ":";
    progPath = strtok(pathlist, s);

    printf("Path is: %s\n", path);

    // Look through the progPaths from PATH.
    while(progPath != NULL) {
      // If the file exists, execute.
      char prog[PATHBUF];
      strcpy(prog, progPath);
      strcat(prog, "/");
      strcat(prog, tok);
      printf("Prog is: %s\n", prog);
      // If file exists, execute it.
      if (access(prog, F_OK) != -1) {
        printf("Inside access(prog, F_OK) != -1)...\n");
        char **args = (char**)malloc(150 * sizeof(char**));
        createArgsArray(firstCmd, &args);

        int status;
        pid_t pid = fork();
        if (pid == 0) {
          execv(prog,args);
        } else if(pid > 0) {
          processID = pid;
          waitpid(-1, &status, 0);
          free(args);
          return 0;
        } else {
          printf("She stopped selling CShells... Bad child!\n");
        }
        
      }
      // Makes the progPath take the next token.
      progPath = strtok(NULL, s);
    }
    printf("Command not found!\n");
  }

  printshellcmd(shellcmd);

  return 0;
}

void catch_int(int sig_num){

    //2 = ctrl - c
    if(sig_num == 2)
        killProcessId();
}

/* --- main loop of the simple shell --- */
int main(int argc, char* argv[]) {

  /* initialize the shell */
  char *cmdline;
  char hostname[HOSTNAMEMAX];
  int terminate = 0;
  Shellcmd shellcmd;
  
  if (gethostname2(hostname)) {

    /* parse commands until exit or ctrl-c */
    while (!terminate) {
        signal(SIGINT, catch_int);
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

