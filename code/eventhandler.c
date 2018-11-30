
#include "eventhandler.h"

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

dataType* initData(SDL_Event* event)
{
    dataType* data = malloc(sizeof(dataType));
    data->event = event;
    data->endEvent = false;
    return data;
}
