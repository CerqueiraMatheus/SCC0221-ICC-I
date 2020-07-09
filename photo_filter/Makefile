CC=gcc
INCLUDES=./include
SOURCES=./source/bitmap_reader.c ./source/bitmap_writer.c ./source/file_helper.c 
BINARY=./filter
PROG=./filter.c
LIBS=-lm

all:
	@ $(CC) -g -o $(BINARY) $(PROG) $(SOURCES) -I$(INCLUDES) $(LIBS)

preproc:
	$(CC) -E $(PROG) $(SOURCES) -I$(INCLUDES) $(LIBS)

run:
	$(BINARY)