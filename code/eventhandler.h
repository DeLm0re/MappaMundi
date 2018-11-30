
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <pthread.h>
#include <SDL2/SDL.h>

typedef struct dataType
{
	SDL_Event* event;
	bool endEvent;
} dataType;

// The function used in the thread
void* eventHandlerFunction(void* data);

dataType* initData(SDL_Event* event);