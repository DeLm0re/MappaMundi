/**
 * \file eventhandler.c
 * \brief Functions that are used for dealing with the interactions the user might make
 * \author Louis Schuck
 * \version 0.1
 * \date 22/11/2018
 *
 * Module that contain the functions that are used for dealing with the interactions the user might make
 *
 */

//Header file
#include "eventhandler.h"

/**
 * \fn eventHandlerFunction(void* data)
 * \brief function that raise a event
 *
 * \param data : data which define the kind of event we have to raise
 * \return void*
 */
void* eventHandlerFunction(void* data)
{
	dataType* cdata = (dataType*) data;
	SDL_Event* event = cdata->event;
	while(cdata->endEvent == false)
	{
		SDL_PollEvent(event);
	}
	return 0;
}

/**
 * \fn initData(SDL_Event* event)
 * \brief function that initialise a structure dataType with a event and return it
 *
 * \param event : event which will be store in the new dataType structure we want to return
 * \return dataType* : the new structure dataType initialise with our event
 */
dataType* initData(SDL_Event* event)
{
    dataType* data = malloc(sizeof(dataType));
    data->event = event;
    data->endEvent = false;
    return data;
}
