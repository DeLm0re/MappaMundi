/**
 * \file main.c
 * \brief main of the project
 * \author Théo Hipault
 * \version 0.4
 * \date 12/10/2018
 *
 * Module that contain the main of the project
 *
 */

//General header files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//More specific headers files
#include <SDL2/SDL.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

//Header file
#include "pathfinding.h"
#include "eventhandler.h"

int main(void)
{
	SDL_Color openSetColor = {80, 160, 80, 255};
	SDL_Color closedSetColor = {160, 80, 80, 255};
	SDL_Color pathColor = {80, 80, 160, 255};

	int windowWidth = 1000;
	int windowHeight = 1000;
	SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int statut = EXIT_FAILURE;

	// Initialisation de la SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }
    // Initialisation de la fenêtre
    window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if(window == NULL)
    {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        goto Quit;
    }
    // Initialisation du renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        goto Quit;
    }

	//Declaration of the thread and the data for the events
    pthread_t thread1;
    dataType* data = initData(&event);
	pthread_create(&thread1, NULL, eventHandlerFunction, (void*) data);

	//Declaration of basic constants
	const int fieldHeight = 50;
	const int fieldWidth = 50;
	const int tileSize = 10;
	//Declaration of the field and the nodes
    Field theField;
	node* startNode = NULL;
	node* endNode = NULL;
	node* openSet = NULL;
	node* closedSet = NULL;
	node* path = NULL;

	while(data->endEvent == false)
	{
		//Initialisation and generation of the field
		theField = initialiseField(fieldHeight, fieldWidth);
		generateEnv(theField, fieldHeight, fieldWidth);
		//Initialisation of the nodes
		startNode = nearestNode(theField, fieldHeight, fieldWidth, 0, 0);
		endNode = nearestNode(theField, fieldHeight, fieldWidth, fieldWidth, fieldHeight);
		

		insertFrontNode(&openSet, cpyNode(startNode));
		while (path == NULL && data->endEvent == false)
		{
			//Do one step of A* algorithme
			path = AStar(&openSet, &closedSet, startNode, endNode, theField, fieldHeight, fieldWidth);

			//Clear the screen
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderClear(renderer);
			//Draw the field
			drawField(renderer, theField, fieldHeight, fieldWidth, tileSize);
			//Draw a box around the field
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderDrawRect(renderer, &((SDL_Rect){0, 0, fieldWidth*tileSize, fieldHeight*tileSize}));
			//Draw the openSet and the closedSet of the A* algorithme
			viewNodes(&openSet, renderer, openSetColor, tileSize);
			viewNodes(&closedSet, renderer, closedSetColor, tileSize);

			//Refresh the window
			SDL_RenderPresent(renderer);

			//If we quit
			if(event.type == SDL_QUIT)
			{
				data->endEvent = true;
				quit = SDL_TRUE;
				statut = EXIT_SUCCESS;
			}
		}
		//Draw the final path
		viewNodes(&path, renderer, pathColor, tileSize);	
		//Refresh the window
		SDL_RenderPresent(renderer);

		// Tant que le flag pour quitter est faux
		while(!quit)
		{
			SDL_Delay(50);
			// On attend l'arriver de l'event de fermeture (la croix)
			if(event.type == SDL_QUIT) 
			{
				data->endEvent = true;
				quit = SDL_TRUE;
				statut = EXIT_SUCCESS;
			}
			else if(event.type == SDL_TEXTINPUT && 
				(*event.text.text == 'r' || 
				*event.text.text == 'R'))
			{
				quit = SDL_TRUE;
			}
		}
		quit = SDL_FALSE;

		//Free the memory of all the nodes
		destructNodes(&path);
		destructNodes(&openSet);
		destructNodes(&closedSet);
		destructNodes(&startNode);
		destructNodes(&endNode);
		startNode = NULL;
		endNode = NULL;
		openSet = NULL;
		closedSet = NULL;
		path = NULL;
		//Free the memory of the field
		destructField(theField, fieldHeight);
		theField = NULL;
	}

	//Ending the thread
	data->endEvent = true;
	pthread_join(thread1, NULL);
	free(data);

	//Process that end the window and the renderer
Quit:
    if(NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if(NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return statut;
}
