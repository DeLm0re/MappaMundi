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

int main(void)
{
	SDL_Color openSetCol = {80, 160, 80, 255};
	SDL_Color closedSetCol = {160, 80, 80, 255};
	SDL_Color pathCol = {80, 80, 160, 255};

	int wwidth = 640;
	int wheight = 480;
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
                              wwidth, wheight, SDL_WINDOW_SHOWN);
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

	
	int mapHeight = 20;
	int mapWidth = 20;
	int tileSize = 20;
	//char** map = initMap(mapHeight, mapWidth);
	//fillMap(map, mapHeight, mapWidth);

	node* startNode = initNode(0, 0, 0, 0);
	node* endNode = initNode(mapWidth-1, mapHeight-1, 0, 0);
	node* openSet = NULL;
	node* closedSet = NULL;

	node* path = NULL;

	while (path == NULL)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		path = AStar(&openSet, &closedSet, startNode, endNode, mapHeight, mapWidth);

		//showMap(map, mapHeight, mapWidth, renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &((SDL_Rect){0, 0, mapWidth*tileSize, mapHeight*tileSize}));
		viewNodes(&openSet, renderer, openSetCol, tileSize);
		viewNodes(&closedSet, renderer, closedSetCol, tileSize);

		// Rafraichissement de la fenêtre
		SDL_RenderPresent(renderer);
	}

	viewNodes(&path, renderer, pathCol, tileSize);	
	// Rafraichissement de la fenêtre
	SDL_RenderPresent(renderer);

	// Tant que le flag pour quitter est faux
	while(!quit)
	{
		SDL_WaitEvent(&event);
        // On attend l'arriver de l'event de fermeture (la croix)
		if(event.type == SDL_QUIT)
        {
			quit = SDL_TRUE;
		    statut = EXIT_SUCCESS;
        }
	}   

	destructNodes(&path);
	destructNodes(&openSet);
	destructNodes(&closedSet);
	destructNodes(&startNode);
	destructNodes(&endNode);
	//destructMap(map, mapHeight);

	// Procédure de fermeture de la fenêtre
Quit:
    if(NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if(NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return statut;
}