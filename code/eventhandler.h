/**
 * \file eventhandler.h
 * \brief Functions prototypes that are used for dealing with the interactions the user might make
 * \author Louis Schuck
 * \version 0.1
 * \date 22/11/2018
 *
 * Module that contain the prototypes of functions that are used for dealing with the interactions the user might make
 *
 */

//Header files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <pthread.h>
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
 * \brief function that raise a event
 *
 * \param data : data which define the kind of event we have to raise
 * \return void*
 */
void* eventHandlerFunction(void* data);

/**
 * \fn dataType* initData(SDL_Event* event)
 * \brief function that initialise a structure dataType with a event and return it
 *
 * \param event : event which will be store in the new dataType structure we want to return
 * \return dataType* : the new structure dataType initialise with our event
 */
dataType* initData(SDL_Event* event);
