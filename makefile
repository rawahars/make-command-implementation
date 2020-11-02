PROGNAME = make537
CC      = gcc
CFLAGS = -Wall -pedantic -Wextra
LDFLAGS =
OBJECTS = main.o error_handle.o linked_list.o graph.o text_parser.o makefile_parser.o execution_engine.o

all: clean $(PROGNAME)

$(PROGNAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(PROGNAME) $(OBJECTS)

error_handle.o: error_handle.h error_handle.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c error_handle.c

linked_list.o: linked_list.h linked_list.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c linked_list.c

graph.o: linked_list.h graph.h graph.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c graph.c

text_parser.o: error_handle.h linked_list.h text_parser.h text_parser.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c text_parser.c

makefile_parser.o: error_handle.h linked_list.h graph.h text_parser.h makefile_parser.h makefile_parser.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c makefile_parser.c

execution_engine.o: error_handle.h makefile_parser.h execution_engine.h execution_engine.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c execution_engine.c

main.o: error_handle.h makefile_parser.h execution_engine.h main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c main.c

clean:
	rm -f $(OBJECTS) $(PROGNAME)