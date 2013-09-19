#include <stdio.h>
#include "parser.h"
#include "print.h"

/* --- print a shell command --- */
void printshellcmd(Shellcmd *shellcmd)
{
  printf("Shellcommand:\n");
  printf("   stdin : %s\n", shellcmd->rd_stdin  ? shellcmd->rd_stdin  : "<none>" );
  printf("   stdout: %s\n", shellcmd->rd_stdout ? shellcmd->rd_stdout : "<none>" );
  printf("   bg    : %s\n", shellcmd->background ? "yes" : "no");
  Cmd *cmdlist = shellcmd->the_cmds;
  while (cmdlist != NULL) {
    char **cmd = cmdlist->cmd;
    cmdlist = cmdlist->next;

    char **printcmd = cmd;
    printf("    [");
    while (*printcmd != NULL) {
      printf("%s ", *printcmd++); // print the cmd and arguments
    }
    printf("]\n");

  }
}

