/**
 * \file main.c
 * \brief main of the project
 * \author Théo Hipault
 * \version 0.3
 * \date 01/10/2018
 *
 * Module that contain the main of the project
 *
 */

//General header files
#include <stdio.h>
#include <stdlib.h>

//Header file
#include "display.h"

//Constants
#define HEIGHT (1000)
#define WIDTH (1000)

//Arguments of the file
#define HEIGHT_FIELD (52)
#define WIDTH_FIELD (52)

//Arguments size of the obstacle
#define SIZE_OBSTACLE (20)

int main(void)
{
    SDL_Renderer *renderer = NULL;
    SDL_Event event;
	SDL_bool quit = SDL_FALSE;
    SDL_Window *window = NULL;
    int statut = EXIT_FAILURE;

    //Declaration of the field
    field theField;

    //Initialisation and creation of the window and renderer
    if(0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }
    window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if(NULL == window)
    {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        goto Quit;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(NULL == renderer)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        goto Quit;
    }

    //Initialisation of the field
    initialiseField(theField, HEIGHT_FIELD, WIDTH_FIELD);

    //Generate the environment
    generateEnv(theField);

    //Draw the field
    drawField(renderer, theField, HEIGHT_FIELD, WIDTH_FIELD, SIZE_OBSTACLE);

    SDL_Delay(500);
    SDL_RenderPresent(renderer);
    SDL_Delay(500);

    while(!quit)
	{
		SDL_WaitEvent(&event);
		if(event.type == SDL_QUIT)
			quit = SDL_TRUE;
		    statut = EXIT_SUCCESS;
	}

    Quit:
        if(NULL != renderer)
        {
            SDL_DestroyRenderer(renderer);
        }
        if(NULL != window)
        {
            SDL_DestroyWindow(window);
        }
        SDL_Quit();

        return statut;
}
