WARNING_FLAGS = -Wall -Wextra -Werror -pedantic -Wformat\
				-fstrict-overflow -Wstrict-overflow=5\
				-Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes\
				-Wmissing-prototypes -Wcast-align -pedantic-errors

CFLAGS = -std=c99 -m64 -O3 $(WARNING_FLAGS)

CLIBS = `sdl2-config --cflags --libs`

main: main.o  core.o  display.o
	@echo "Links edition :"
	gcc -o main main.o  core.o  display.o $(CLIBS)
	rm -f *~ *.o
	@echo ""

main.o:	main.c core.h display.h
	@echo "Compilation main :"
	gcc $(CFLAGS) -c main.c
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