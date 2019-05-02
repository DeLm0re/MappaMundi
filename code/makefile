WARNING_FLAGS = -Wall -Wextra -pedantic -Wformat\
				-Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes\
				-Wmissing-prototypes -Wcast-align -pedantic-errors\
				-Wno-strict-overflow

CFLAGS = -std=c99 -m64 -O3 $(WARNING_FLAGS)
CLIBS = -lpthread -lm `sdl2-config --cflags --libs`
CC = gcc
DEP = BmpLib.o OutilsLib.o eventhandler.o rioFunction.o neuralNetwork.o core.o geneticAlgorithm.o pathfinding.o entity.o display.o wrapper.o
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
