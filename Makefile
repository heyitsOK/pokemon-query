a3:								a3.o listFunctions.o defs.h
									gcc -o a3 a3.o listFunctions.o defs.h -lpthread
a3.o:							a3.c defs.h
									gcc -c a3.c
listFunctions.o:	listFunctions.c defs.h
									gcc -c listFunctions.c
clean:
									rm a3.o listFunctions.o
