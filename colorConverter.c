#include "colorConverter.h"
#include <string.h>
#include <strings.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define BUF 1024

void mcprint(char* src, char* color)
{
    printf("%s%s%s", color ,src , KNRM);
}

void printfred(char* input) {
    mcprint(input, KRED);
}

void printfgreen(char* input) {
    mcprint(input, KGRN);    
}

void printfyellow(char* input) {
    mcprint(input, KYEL);
}

void printfblue(char* input) {
    mcprint(input, KBLU);
}

void printfmagenta(char* input) {
    mcprint(input, KMAG);
}

void printfcyan(char* input) {
    mcprint(input, KCYN);
}
