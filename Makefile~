all: CShell

OBJS = parser.o print.o
LIBS= -lreadline -ltermcap
CC = gcc

CShell: CShell.o ${OBJS}
	${CC} -o $@  ${LIBS} CShell.o ${OBJS}

clean:
	rm -rf *o CShell
