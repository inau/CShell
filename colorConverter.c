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

void cprint(char* src, char* color)
{
    printf("%s%s%s", color ,src , KNRM);
}

void printfred(char* input) {
    cprint(input, KRED);
}

void printfgreen(char* input) {
    cprint(input, KGRN);    
}

void printfyellow(char* input) {
    cprint(input, KYEL);
}

void printfblue(char* input) {
    cprint(input, KBLU);
}

void printfmagenta(char* input) {
    cprint(input, KMAG);
}

void printfcyan(char* input) {
    cprint(input, KCYN);
}
