/* 
   CShell.c : She sells CShells at the C shore 
 */

typedef struct _singlecmd { 
    char **cmd;
    char *rd_stdin;
    char *rd_stdout;
    char *rd_stderr;
    int background;
} Singlecmd;

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

/* --- executes a single command --- */
int executecmd(Singlecmd *singlecmd) {
	// Check if the command is one of the reserved ones.;
  char *tok = *(singlecmd->cmd);
  char **temp = singlecmd->cmd;
  if (isspec(tok)) {
    if (isexit(tok)) {
      // Return 1 if exit (exits CShell).
      return 1;
    }
    if (iscd(tok)) {
		  char **args = (char**)malloc(150 * sizeof(char**));
		  createArgsArray(singlecmd->cmd, &args);
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
  if (cmdexist(tok, &prog)) {
    char **args = (char**)malloc(150 * sizeof(char**));
    createArgsArray(singlecmd->cmd, &args);
    int status;
    pid_t pid = fork();
    if (pid == 0) {
			// If we have specified a standard out,
			// create the file and set it as stdout.
			if (singlecmd->rd_stdout != NULL) {
				createfile(singlecmd->rd_stdout, 1);
				close(1);
				dup(open(singlecmd->rd_stdout,O_RDWR));
			}
			// If we have specified a standard in,
			// set it as stdin.
			if (singlecmd->rd_stdin != NULL) {
				if (access(singlecmd->rd_stdin, F_OK) != -1) {
					close(0);
					dup(open(singlecmd->rd_stdin,O_RDWR));
				}
			}
			// Execute the file with the given arguments.
      execv(prog,args);
    } else if(pid > 0) {		
			// If the process is not running as a background process,
			// wait for the process to finish.
			if (singlecmd->background == 0) {
				processID = pid;
      	waitpid(pid, &status, 0);
				processID = 0; // Make sure we cannot kill any processes.
			}
      return 0;
		} else {
  		printf("She stopped selling CShells... Bad child!\n");
		}
		free(args);
	}
	return 0;
}

/* --- execute a shell command --- */
int executeshellcmd (Shellcmd *shellcmd)
{
	int cmdcount = 0;
	Cmd *curcmd;
	curcmd = shellcmd->the_cmds;
	while(curcmd != NULL) {
		// Execute one command at a time.
		Singlecmd *singlecmd = (Singlecmd*)malloc(sizeof(Singlecmd));
		singlecmd->cmd = curcmd->cmd;
		singlecmd->rd_stdin = shellcmd->rd_stdin;
		singlecmd->rd_stdout = shellcmd->rd_stdout;
		singlecmd->rd_stderr = shellcmd->rd_stderr;
		singlecmd->background = shellcmd->background;
		int result = executecmd(singlecmd); // Get the result from execution.
		free(singlecmd);
		if (result > 0) return 1; // Exit if we want to.
		curcmd = curcmd->next;
	}

  return 0;
}

void catch_int(int sig_num){
    //2 = ctrl - c
    if(sig_num == 2) {
    	killProcessId();
    	printf("\n");
    }
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
