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
#include "display.h"
#include "eventhandler.h"

//Main of the programme
int main(void)
{
	//Initialize the random seed value
	srand(12345);

/* 	SDL_Color openSetColor = {80, 160, 80, 255};
	SDL_Color closedSetColor = {160, 80, 80, 255}; 
	SDL_Color pathColor = {80, 80, 160, 255};*/
	SDL_Color entityColor = {80, 160, 160, 255};

	int windowWidth = 1000;
	int windowHeight = 1000;
	SDL_Event event;
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
	//Used to know if we're waiting for an instruction (keyboard input, etc)
	bool waitForInstruction = true;
	//Declaration of the field
    Field *theField = NULL;
	//Declaration of the nodes for the pathfinding
	node* startNode = NULL;
	node* endNode = NULL;
	node* path = NULL;
	//Declaration of the entity wich will be used by the neural network
	Entity* entity = NULL;
	//Used to make the entity move along the path found
	int positionInPath = 0;
	node* nodePosition = NULL;
	
	while(data->endEvent == false)
	{
		//Initialisation and generation of the field
		theField = initialiseField(fieldWidth, fieldHeight, EMPTY);
		generateEnv(theField);
		//Initialise the entity
		entity = initialiseEntity(0, 0, RADIUS_VIEWPOINT, fieldWidth, fieldHeight);
		//Initialisation of the nodes
		startNode = nearestNode(theField, entity->x, entity->y);
		//Updates the position of the entity for the nearest starting node
		entity->x = startNode->x;
		entity->y = startNode->y;
		endNode = nearestNode(theField, fieldWidth, fieldHeight);
		
		//--- Pathfinding algorithm and visualisation
        
		path = findPathFrom_To_(startNode, endNode, theField, &(data->endEvent));
        
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

				//Updates the field of view of our entity
				updateFieldOfViewEntity(theField, entity);

				//Updates the mental map of our entity with its new field of view
				updateMentalMapEntity(entity);
			    
			    //Clear the screen
			    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			    SDL_RenderClear(renderer);
			    //Draw the field
			    drawField(renderer, entity->mentalMap, tileSize);
				//Draw the field of view
				drawFieldOfViewEntity(renderer, entity, tileSize);
			    //Draw the entity
			    showEntity(entity, renderer, entityColor, tileSize);
			    //Refresh the window
		    	SDL_RenderPresent(renderer);
		    	//We wait 30ms at each step to see the entity moving
			    SDL_Delay(30);
			    
			    //If we want to quit the program (the cross in the top right corner or the "q" key on the keyboard)
			    if (event.type == SDL_QUIT ||
			    (event.type == SDL_TEXTINPUT && 
				(*event.text.text == 'q' || 
				*event.text.text == 'Q')))
			    {
			        //We put an end to the program
				    data->endEvent = true;
				    //We update the exit statut
				    statut = EXIT_SUCCESS;
				    //We set the waiting flag to false (not waiting for inputs anymore)
				    waitForInstruction = false;
			    }
			}
		}while(nodePosition != NULL && !data->endEvent);
		
		//While the waiting flag is set to true (waiting for inputs)
		while(waitForInstruction)
		{
			SDL_Delay(50);
			//If we want to quit the program (the cross in the top right corner or the "q" key on the keyboard)
			if (event.type == SDL_QUIT ||
			    (event.type == SDL_TEXTINPUT && 
				(*event.text.text == 'q' || 
				*event.text.text == 'Q')))
			{
			    //We put an end to the program
				data->endEvent = true;
				//We update the exit statut
				statut = EXIT_SUCCESS;
				//We set the waiting flag to false (not waiting for inputs anymore)
				waitForInstruction = false;
			}
			//If we want to do a new simulation ("r" key on the keyboard)
			else if(event.type == SDL_TEXTINPUT && 
				(*event.text.text == 'r' || 
				*event.text.text == 'R'))
			{
			    //We set the waiting flag to false (not waiting for inputs anymore)
				waitForInstruction = false;
			}
		}
		//We put the waiting flag back to true
		waitForInstruction = true;

		//Free the memory of all the nodes
		destructNodes(&path);
		destructNodes(&startNode);
		destructNodes(&endNode);
		nodePosition = NULL;
		positionInPath = 0;
		//Free the memory of the field
		destructField(&theField);
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
