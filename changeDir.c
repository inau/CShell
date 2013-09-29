/*
 * inau@itu.dk 
 * Written to provide the functions as known from the cd command
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <dirent.h>

#include "changeDir.h"

#define ONEUP ".."
#define ROOT "/"

/*Constants*/
#define DIRBUFFER 1024

char *curDir;

int checkBuffer() { /*Called to make sure the buffer is setup*/
    if(curDir == NULL) {
        curDir[DIRBUFFER];
        curDir = ROOT;
    }
}

int isFullPath(char *input) {
    printf("is path: "); 
    DIR *dp;
     dp = opendir (input);
     if (dp != NULL)
         {
          printf("yes \n");
          (void) closedir (dp);
          return 0;
         }
       else {
         printf("no \n");
         printf("Couldn't open the directory: %s \n", curDir);
         return 1;
       }
}

int isSubDir(char *input) {
    printf("is sub \n");
    DIR *dp;
    struct dirent *ep; /*info about directory*/
    dp = opendir (curDir);
    if (dp != NULL)
    {
       while (ep = readdir (dp)) {
          if(strcmp(ep->d_name, input)) {
                 printf("SubF: %s\n",ep->d_name);
                 return 0;
          }
       }
       printf("Couldn't find %s in the current directory \n", input);
       (void) closedir (dp);
       return 1;
    }
    else {
         printf("Couldn't open the directory: %s \n", curDir);
         return 1;
    }
}

int changeDir(char *newDir) {
    checkBuffer();
    if(newDir == NULL) { /*Empty args = root folder*/
        printf("Dir = NULL\n");
        curDir = ROOT;
        return 1;
    }
    else {
        printf("Dir = %s\n",newDir);
        if(strcmp(curDir, newDir) != 0) { /*If both are the same no changes are needed*/
                if(strcmp(newDir, ONEUP) == 0) {            
                    printf("Matched 1up\n");
                    return 1;
                }
                else if(isFullPath(newDir) == 0) {
                    printf("Matched full path\n");
                    curDir = newDir;
                    return 1;
                }
                else if(isSubDir == 0) {
                    printf("Matched sub dir\n");
                    strcat(curDir, newDir);
                    strcat(curDir, "/");
                    return 1;
                }
                else {
                    printf("No match \n");
                    return 0;
                }
        }
        else return 1;
    }
    return -1;
}

char* getCurrentDir(void) {
    checkBuffer();
    return curDir;   
}
