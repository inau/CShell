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
#include <sys/wait.h>
#include "parser.h"
#include "print.h"
#include "changeDir.h"
#include <signal.h>
#include <unistd.h>

/* --- symbolic constants --- */
#define HOSTNAMEMAX 100
#define MAXBUF 512

#define EXIT	("exit")
#define CD      ("cd")

/* --- symbolic macros --- */
#define isexit(c) (strcmp(c,EXIT) == 0)
#define iscd(c) (strcmp(c,CD) == 0)
#define isspec(c) (isexit(c) || iscd(c))

/* --- buffer sizes --- */
#define PATHBUF 1024

pid_t processID = 0;

/* --- use the /proc filesystem to obtain the hostname --- */
char *gethostname2(char *hostname)
{
  char hostname2[MAXBUF];   
  FILE *hostfile;
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
}

int pipecmd(char *filename1, char *argv1[], char *filename2, char *argv2[])
{
	int fd[2];
        int status = 0;
	pipe(fd);
	pid_t pid = fork();
        if(pid == 0){
            pid_t pid2 = fork();
            if(pid2 == 0){
                //grand-child
                //input of pipe
                /* Close stdin, duplicate the input side of pipe to stdin */
                dup2(fd[0], 0);
                execvp(filename2, argv2);
                close(fd[0]);

                exit(0);
            }
            else{
    		//child
                //output of pipe
    		/* Close stdin, duplicate the input side of pipe to stdin */
                dup2(fd[1], 1);

    		execvp(filename1, argv1);

    		close(fd[1]);

    		exit(0);

    		waitpid(0, &status, 0);
            }
    }
    else{
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
    if (iscd(tok)) {
        char **args = (char**)malloc(150 * sizeof(char**));
        createArgsArray(firstCmd, &args);
        if(args != NULL) {
                printf("Entered args != null \n");
                if(changeDir(args[1])) printf("Dir Changed to: %s \n", getCurrentDir());
                else printf("Unknown path: %s \n",args[1]);
        }
        else {
            printf("Entered args == null \n");
            changeDir("");
            printf("Dir changed to: %s \n",getCurrentDir());
        }
        free(args);
        return 0;
    }
  }

  // Find all the programs in the path environment.
  char *path = getenv("PATH");
  if (path != NULL) {
    char pathlist[PATHBUF];
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
          printshellcmd(shellcmd);
          execv(prog,args);
        } else if(pid > 0) {
          processID = pid;
          waitpid(-1, &status, 0);
          processID = 0; // Make sure we cannot kill any processes
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
