CC=gcc
INCLUDES=./include
SOURCES=./source/file_helper.c ./source/metadata.c
BINARY=./starter
PROG=./starter.c
LIBS=-lm

all:
	$(CC) -o $(BINARY) $(PROG) $(SOURCES) -I$(INCLUDES) $(LIBS)

preproc:
	$(CC) -E $(PROG) $(SOURCES) -I$(INCLUDES) $(LIBS)

run:
	$(BINARY)