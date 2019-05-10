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

#include "wrapper.h"
#include <time.h>

#define FIELD_WIDTH 20
#define FIELD_HEIGHT 20
#define SAVING_PATH_NN "../NN/Reseau1.nn"
#define SAVING_PATH_GN "../GN"

//Main of the programme
int main(int argc, char** argv)
{
	if (argc < 2) 
	{
		printf("Please, enter an argument : \n");
		printf(" 1 [pathMap] : \n\tCreate a new neural network and train it on a random map or on an existing map\n");
		printf(" 2 [pathMap] : \n\tLoad an existing neural network and test it on a random map or on an existing map\n");
		printf(" 3 [pathGeneticNetwork] : \n\tStart a new genetic network and train it or use an existing one as reference\n");
		printf(" 4 pathGeneticNetwork [pathMap] : \n\tLoad an existing genetic network and test it on a random map or on an existing map\n");
	}
	else
	{
		//Initialize the random seed value
		srand(12345);
		
		const int tileSize = 10;
		int windowWidth = 640;
		int windowHeight = 480;
		
		// Set some basic variables for the SDL to work
		SDL_Event event;
		SDL_Window *window = NULL;
		SDL_Renderer *renderer = NULL;
		dataType* data = initData(&event);
		
		// Initialisation de la SDL
		if(SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
			goto Quit;
		}
		// Initialisation de la fenêtre
		window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
								windowWidth, windowHeight, SDL_WINDOW_HIDDEN);
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
		pthread_create(&thread1, NULL, eventHandlerFunction, (void*) data);

		NeuralNetwork* neuralNetwork = NULL;
		LabelingWeights* labelingWeights = NULL;
		
		Field *theField = NULL;
		int fieldHeight = FIELD_WIDTH;
		int fieldWidth = FIELD_HEIGHT;
		
		char* pathImageField = NULL;
		
		int menuChoice = atoi(argv[1]);

        // We extract the path pf the image if it exist
        switch (menuChoice)
		{
			//New neural network
			case TRAIN_NN:
			    if (argc == 3)
	                pathImageField = argv[2];
				break;
			//Load neural network
			case LOAD_NN:
			    if (argc == 3)
			    pathImageField = argv[2];
				break;
			//Load genetic network
			case LOAD_GN:
			    if (argc == 4)
	                pathImageField = argv[3];
			    break;
		}
		
	    theField = createCustomField(pathImageField);
	    bool fieldIsFromImage = true;
	    if (theField == NULL)
	    {
	        theField = initialiseField(fieldWidth, fieldHeight, EMPTY);
	        generateEnv(theField);
	        fieldIsFromImage = false;
	    }
        fieldHeight = theField->height;
        fieldWidth = theField->width;
		        
		windowWidth = (fieldWidth*tileSize) + /*offset*/ 2*tileSize + (RADIUS_VIEWPOINT*2*tileSize) + 2*tileSize;
		windowHeight = fieldHeight*tileSize;
		SDL_SetWindowSize(window, windowWidth, windowHeight);
		
		switch (menuChoice)
		{
			//New neural network
			case TRAIN_NN:
				SDL_ShowWindow(window);
				neuralNetwork = trainingNN2(fieldWidth, fieldHeight, data, SAVING_PATH_NN, renderer, tileSize);
				break;
			//Load neural network
			case LOAD_NN:
				SDL_ShowWindow(window);
				neuralNetwork = loadNeuralNetwork(SAVING_PATH_NN);
				break;
			//New genetic network
			case TRAIN_GN:
			    // 5 generation seems to be enought and 100 member is maybe a bit too much
			    if (argc >= 3)
			        labelingWeights = trainingGN1(data, 30, 30, SAVING_PATH_GN, argv[2], 10, 100);
			    else
			        labelingWeights = trainingGN1(data, 30, 30, SAVING_PATH_GN, NULL, 10, 100);
			    break;
			//Load genetic network
			case LOAD_GN:
				SDL_ShowWindow(window);
		        if (argc >= 3)
                    labelingWeights = loadGeneticNetwork(argv[2]);
                else
                    printf("Error : Invalid arguments\n");
			    break;
			default:
				printf("Error : Invalid arguments\n");
				break;
		}
		
		//--- Main loop
		
		while(!data->endEvent)
		{   
			//Initiate the entity, the start and end of the route according to the field
			Entity* entity = initialiseEntity(0, 0, RADIUS_VIEWPOINT, fieldWidth, fieldHeight);
			node* startNode = nearestNode(theField, entity->x, entity->y);
			entity->x = startNode->x;
			entity->y = startNode->y;
			destructNodes(&startNode);
			node* endNode = nearestNode(theField, fieldWidth, fieldHeight);
			
			updateFieldOfViewEntity(theField, entity);
			updateMentalMapEntity(entity);
		    
			//While the entity hasn't arrived at destination
			while ((entity->x != endNode->x || entity->y != endNode->y) && !data->endEvent)
			{
				node *path = NULL;
                if (menuChoice == LOAD_NN)
				    path = findNextPathNN(entity, endNode, data, neuralNetwork);
				else if (menuChoice == LOAD_GN)
				    path = findNextPathGN(entity, endNode, data, labelingWeights);
				
		        moveEntityAlongPath(data, entity, path, theField, renderer, tileSize, 30);
			}
			destructNodes(&endNode);
			
			// We load a new field if we use a random map
			if(!fieldIsFromImage)
		    {
		        destructField(&theField);
		        theField = initialiseField(fieldWidth, fieldHeight, EMPTY);
	            generateEnv(theField);
		    }
			
			waitForInstruction(data);
		}

		destructNeuralNetwork(&neuralNetwork);
		destructLabelingWeights(&labelingWeights);
		
		data->endEvent = true;
		pthread_join(thread1, NULL);

		//Process that end the window and the renderer
	Quit:
		if(NULL != renderer)
			SDL_DestroyRenderer(renderer);
		if(NULL != window)
			SDL_DestroyWindow(window);
		SDL_Quit();
		int statut = data->statut;
		free(data);
		return statut;
	}
}
