CFLAGS=-Wall -g

all: merge2 merge4

merge2: merge2.c
	gcc $(CFLAGS) merge2.c -o merge2

mergeN: mergeN.c merge2.h
	gcc $(CFLAGS) mergeN.c merge2.c -o mergeN

merge4: merge4.c merge2.h
	gcc $(CFLAGS) merge4.c merge2.c -o merge4

msort: msort.c merge4.h 
	gcc $(CFLAGS) msort.c merge4.c merge2.c -o msort

clean:
	rm -f merge2 merge4
