all: main.c setting.h scheduling.c
	gcc main.c scheduling.c -o PJ1

clean:	
	rm -f PJ1
