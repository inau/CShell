/* 
   CShell.c : She sells CShells at the C shore 
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

#include "changeDir.h"
#include "helpfunctions.h"

/* --- symbolic constants --- */
#define HOSTNAMEMAX 100
#define MAXBUF 512
#define PATHBUF 1024

#define EXIT	("exit")
#define CD      ("cd")

/* --- symbolic macros --- */
#define isexit(c) (strcmp(c,EXIT) == 0)
#define iscd(c) (strcmp(c,CD) == 0)
#define isspec(c) (isexit(c) || iscd(c))


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

/* --- Kills the currently stored process --- */
void killProcessId(){
    if(processID != 0){
        kill(processID, SIGTERM);
        processID = 0;
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

  // If file exists, execute it.
  char *prog;
  if (fileexist(tok, &prog)) {
    char **args = (char**)malloc(150 * sizeof(char**));
    createArgsArray(firstCmd, &args);
		
		if (shellcmd->rd_stdout != NULL) {
			printf("Stdout: %s\n", shellcmd->rd_stdout);
			createfile(shellcmd->rd_stdout, 1);
		}

    int status;
    pid_t pid = fork();
    if (pid == 0) {
      printshellcmd(shellcmd);
			if (shellcmd->rd_stdout != NULL) {
				close(1);
				dup(open(shellcmd->rd_stdout,O_RDWR));
			}
			if (shellcmd->rd_stdout != NULL) {
			printf("Stdout: %s\n", shellcmd->rd_stdout);
			createfile(shellcmd->rd_stdout, 1);
		}
      execv(prog,args);
    } else if(pid > 0) {
      processID = pid;
      waitpid(-1, &status, 0);
      processID = 0; // Make sure we cannot kill any processes.
      return 0;
		} else {
  		printf("She stopped selling CShells... Bad child!\n");
		}
		free(args);
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
