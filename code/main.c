/**
 * \file main.c
 * \brief main of the project
 * \author Théo Hipault
 * \version 0.5
 * \date 12/10/2018
 *
 * Module that contain the main of the project
 *
 */

//Header file
#include "pathfinding.h"
#include "eventhandler.h"
#include "entity.h"

//Main of the programme
int main(void)
{
	SDL_Color openSetColor = {80, 160, 80, 255};
	SDL_Color closedSetColor = {160, 80, 80, 255};
	SDL_Color pathColor = {80, 80, 160, 255};
	SDL_Color entityColor = {80, 160, 160, 255};

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
	//Declaration of the field
    Field theField;
	//Declaration of the nodes for the pathfinding
	node* startNode = NULL;
	node* endNode = NULL;
	node* openSet = NULL;
	node* closedSet = NULL;
	node* path = NULL;
	//Declaration of the entity wich will be used by the neural network
	Entity* entity = NULL;
	//Used to make the entity move along the path found
	int positionInPath = 0;
	node* nodePosition = NULL;

	while(data->endEvent == false)
	{
		//Initialisation and generation of the field
		theField = initialiseField(fieldHeight, fieldWidth);
		generateEnv(theField, fieldHeight, fieldWidth);
		//Initialise the entity
		entity = initialiseEntity(0, 0, RADIUS_VIEWPOINT);
		//Initialisation of the nodes
		startNode = nearestNode(theField, fieldHeight, fieldWidth, entity->x, entity->y);
		//Updates the position of the entity for the nearest starting node
		entity->x = startNode->x;
		entity->y = startNode->y;
		endNode = nearestNode(theField, fieldHeight, fieldWidth, fieldWidth, fieldHeight);
		
		//--- Pathfinding algorithm and visualisation

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

			//If we want to interrupt the algorithme
			if(event.type == SDL_QUIT)
			{
			    // We set the ending flag to true
				data->endEvent = true;
				quit = SDL_TRUE;
				statut = EXIT_SUCCESS;
			}
		}
		//Draw the final path once the algorithm is finished
		viewNodes(&path, renderer, pathColor, tileSize);
		
		//We wait two seconds before moving the entity along the way
        SDL_Delay(200);
        
        //--- Entity movement along the line

		//Move the entity along the path
		do
		{
			//get the next position
			positionInPath++;
			nodePosition = getNode(&path, positionInPath);
			//If we find the next nodePosition
			if (nodePosition != NULL)
			{
			    //Updates the position of the entity for the nearest starting node
			    entity->x = nodePosition->x;
			    entity->y = nodePosition->y;
			    
			    //Clear the screen
			    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			    SDL_RenderClear(renderer);
			    //Draw the field
			    drawField(renderer, theField, fieldHeight, fieldWidth, tileSize);
			    //Draw the entity
			    showEntity(entity, renderer, entityColor, tileSize);
			    //Refresh the window
		    	SDL_RenderPresent(renderer);
		    	//We wait 30ms at each step to see the entity moving
			    SDL_Delay(30);
			}
		}while(nodePosition != NULL);
		
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
		nodePosition = NULL;
		positionInPath = 0;
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
