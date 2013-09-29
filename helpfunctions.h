extern int cmdexist(char *filename, char **givenprogpath);
extern int pipecmd(char *filename1, char *argv1[], char *filename2, char *argv2[]);
extern void createfile(char *filepath, int rm);
extern void createArgsArray(char **cmd, char ***args);

