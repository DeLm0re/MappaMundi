/**
 * \file eventhandler.c
 * \brief Functions that are used to deal with user interactions at any moment
 * \author Louis Schuck
 * \version 0.1
 * \date 22/11/2018
 *
 * Module that contain the functions that are used to deal with user interactions at any moment
 *
 */

//Header file
#include "eventhandler.h"

/**
 * \fn void* eventHandlerFunction(void* data)
 * \brief Function that raises an event
 *
 * \param data : data which define the kind of event we have to raise
 * 
 * \return void*
 */
void* eventHandlerFunction(void* data)
{
	dataType* cdata = (dataType*) data;
	SDL_Event* event = cdata->event;
	while(cdata->endEvent == false)
	{
		SDL_PollEvent(event);
		if (cdata->event->type == SDL_QUIT ||
		(cdata->event->type == SDL_TEXTINPUT && 
		(*cdata->event->text.text == 'q' || 
		*cdata->event->text.text == 'Q')))
		{
			//We put an end to the program
			cdata->endEvent = true;
			//We update the exit statut
			cdata->statut = EXIT_SUCCESS;
			//We set the waiting flag to false (not waiting for inputs anymore)
			cdata->waitForInstruction = false;
		}
		if(cdata->event->type == SDL_TEXTINPUT &&
		cdata->waitForInstruction &&
		(*cdata->event->text.text == 'r' || 
		*cdata->event->text.text == 'R'))
		{
			cdata->waitForInstruction = false;
		}
	}
	return 0;
}

/**
 * \fn dataType* initData(SDL_Event* event)
 * \brief Function that initializes a structure dataType with a event and returns it
 *
 * \param event : event which will be stored in the new dataType structure we want to return
 * 
 * \return dataType* : The new structure dataType initialized with our event
 */
dataType* initData(SDL_Event* event)
{
    dataType* data = malloc(sizeof(dataType));
    data->event = event;
    data->endEvent = false;
    data->statut = EXIT_FAILURE;
    data->waitForInstruction = false;
    return data;
}
