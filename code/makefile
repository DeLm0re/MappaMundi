WARNING_FLAGS = -Wall -Wextra -Werror -pedantic -Wformat\
				-Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes\
				-Wmissing-prototypes -Wcast-align -pedantic-errors

CFLAGS = -std=c99 -m64 -O3 $(WARNING_FLAGS)

CLIBS = -lpthread `sdl2-config --cflags --libs`

main: main.o pathfinding.o core.o display.o eventhandler.o
	@echo "Links edition :"
	gcc -o main main.o pathfinding.o core.o display.o eventhandler.o $(CLIBS)
	rm -f *~ *.o
	@echo ""

main.o:	main.c pathfinding.h core.h display.h eventhandler.h
	@echo "Compilation main :"
	gcc $(CFLAGS) -c main.c
	@echo ""

pathfinding.o : pathfinding.c pathfinding.h
	@echo "Compilation core :"
	gcc $(CFLAGS) -c pathfinding.c $(CLIBS)
	@echo ""

eventhandler.o : eventhandler.c eventhandler.h
	@echo "Compilation core :"
	gcc $(CFLAGS) -c eventhandler.c $(CLIBS)
	@echo ""

core.o : core.c core.h
	@echo "Compilation core :"
	gcc $(CFLAGS) -c core.c $(CLIBS)
	@echo ""

display.o: display.c display.h
	@echo "Compilation display :"
	gcc $(CFLAGS) -c display.c $(CLIBS)
	@echo ""

clean:
	rm -f *.o
	rm -f main
	rm -f *~
