WARNING_FLAGS = -Wall -Wextra -Werror -pedantic -Wformat\
				-Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes\
				-Wmissing-prototypes -Wcast-align -pedantic-errors

CFLAGS = -std=c99 -m64 -O3 $(WARNING_FLAGS)
CLIBS = -lpthread `sdl2-config --cflags --libs`
CC = gcc
DEP = pathfinding.o core.o display.o eventhandler.o
EXE = main

all: $(EXE)

$(EXE): $(EXE).o $(DEP)
	@echo "Links edition :"
	$(CC) -o $@ $^ $(CLIBS)
	rm -f *~ *.o
	@echo ""

%.o: %.c
	@echo "Compilation $@"
	$(CC) $(CFLAGS) -c $^ $(CLIBS)
	@echo ""

clean:
	rm -f *.o
	rm -f $(EXE)
	rm -f *~
