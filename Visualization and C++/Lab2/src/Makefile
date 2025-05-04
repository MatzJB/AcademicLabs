CC = cc
STD = -std=c99 -Wall -pedantic 
#LINKS = -lGL -lglut -lm #linux
LINKS = -lglut32 -lglu32 -lopengl32 #windows (cygwin)

village:
	$(CC) village.c GUtil.c $(LINKS)


orrery:
	$(CC) orrery.c GUtil.c $(LINKS)



clean:
	rm a.exe #if windows
	rm a.out #linux distributions


