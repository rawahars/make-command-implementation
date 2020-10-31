PROGNAME = make537
CC      = gcc
CFLAGS = -Wall -pedantic -Wextra
LDFLAGS =
OBJECTS = linked_list.o graph.o

all: clean $(PROGNAME)

$(PROGNAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(PROGNAME) $(OBJECTS)

graph.o: graph.h linked_list.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c graph.c

linked_list.o: linked_list.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c linked_list.c

clean:
	rm -f $(OBJECTS) $(PROGNAME)