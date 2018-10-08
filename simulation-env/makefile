all: main.o

main.o: main.c
	gcc main.c -o main `sdl2-config --cflags --libs`
	
clean: 
	rm -rf *.o main.*
