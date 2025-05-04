
CC = cc
STD = -std=c99#wall pedantic
LINKS = -L. -lcompgraph -L/usr/X11R6/lib -lX11 -lpthread -lm
#L. dvs. titta här eftersom jag kopierat biblioteket

#FNAME = lab3\ HPC.c

it:#the lab
	$(CC) -std=c99 -O3 lab3HPC.c -o fractal -Wall -pedantic $(LINKS)


cleanMS:	
	rm a.exe #if windows
clean:
	rm a.out
	rm fractal

