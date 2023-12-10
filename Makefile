CFLAGS=-Wall -g


merge2: merge2.c
	gcc merge2.c -o merge2

merge4: merge4.c merge2.h
	gcc merge4.c merge2.c -o merge4 

mergeN: mergeN.c merge2.h
	gcc mergeN.c merge2.c -o mergeN

msort: msort.c merge4.h 
	gcc msort.c merge4.c merge2.c -o msort

clean:
	rm -f merge2 merge4 mergeN msort
