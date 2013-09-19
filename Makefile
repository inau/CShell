all: bosh

OBJS = parser.o print.o
LIBS= -lreadline -ltermcap
CC = gcc

boash: boash.o ${OBJS}
	${CC} -o $@  ${LIBS} boash.o ${OBJS}

clean:
	rm -rf *o boash
