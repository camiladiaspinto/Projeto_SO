CFLAGS=-Wall -g

all: merge2 merge4

merge2: merge2.c
	gcc $(CFLAGS) merge2.c -o merge2

merge4: merge4.c merge2.h
	gcc $(CFLAGS) merge4.c merge2.c -o merge4

clean:
	rm -f merge2 merge4
