/**
 * \file eventhandler.h
 * \brief Prototypes of functions un eventhandler.c
 * \author Louis Schuck
 * \version 0.1
 * \date 22/11/2018
 *
 * Prototypes of functions un eventhandler.c
 *
 */

#ifndef H_EVENTHANDLER
	#define H_EVENTHANDLER

#include <stdbool.h>
#include <SDL2/SDL.h>

//The structure dataType which define what is the kind of event which is happening
typedef struct dataType
{
	SDL_Event* event;
	bool endEvent;
	int statut;
	bool waitForInstruction;
} dataType;

/**
 * \fn void* eventHandlerFunction(void* data)
 * \brief Function that raises an event
 *
 * \param data : data which define the kind of event we have to raise
 * 
 * \return void*
 */
void* eventHandlerFunction(void* data);

/**
 * \fn dataType* initData(SDL_Event* event)
 * \brief Function that initializes a structure dataType with a event and returns it
 *
 * \param event : event which will be stored in the new dataType structure we want to return
 * 
 * \return dataType* : The new structure dataType initialized with our event
 */
dataType* initData(SDL_Event* event);

#endif