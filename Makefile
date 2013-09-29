all: CShell

OBJS = parser.o print.o changeDir.o helpfunctions.o
LIBS= -lreadline -ltermcap
CC = gcc

CShell: CShell.o ${OBJS}
	${CC} -o $@ CShell.o ${OBJS} ${LIBS}

clean:
	rm -rf *o CShell
