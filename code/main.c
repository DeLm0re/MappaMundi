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

//Main of the programme
int main(int argc, char** argv)
{
	if (argc < 2) 
	{
		printf("Please, enter an argument : \n");
		printf(" 1 [pathMap] : Create a new neural network and train it on a random map or on an existing map\n");
		printf(" 2 [pathMap] : Load an existing neural network and test it on a random map or on an existing map\n");
		printf(" 3 [pathGeneticNetwork] : Start a new genetic network and train it or use an existing one as reference\n");
		printf(" 4 pathGeneticNetwork [pathMap] : Load an existing genetic network and test it on a random map or on an existing map\n");
	}
	else
	{
		//Initialize the random seed value
		srand(12345);
		
		// Create a color for the entity that will move
		SDL_Color entityColor = {80, 160, 160, 255};

		
		const int tileSize = 5;
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
		pthread_create(&thread1, NULL, eventHandlerFunction, (void*) data);
	
		char savingPathNN[256] = "../NN/Reseau1.nn";
		char savingPathGN[256] = "../GN";

		NeuralNetwork* neuralNetwork = NULL;
		LabelingWeights* labelingWeights = NULL;
		
		Field *theField = NULL;
		int fieldHeight = 50;
		int fieldWidth = 50;
		
		char pathImageField[256] = "";
		
		int menuChoice = atoi(argv[1]);

        // We extract the path pf the image if it exist
        switch (menuChoice)
		{
			//New neural network
			case TRAIN_NN:
			    if (argc == 3)
	                strcpy(pathImageField, argv[2]);
				break;
			//Load neural network
			case LOAD_NN:
			    if (argc == 3)
	                strcpy(pathImageField, argv[2]);
				break;
			//Load genetic network
			case LOAD_GN:
			    if (argc == 4)
	                strcpy(pathImageField, argv[3]);
			    break;
		}
		
	    theField = createCustomField(pathImageField);
	    bool fieldIsLoad = true;
	    if (theField == NULL)
	    {
	        theField = initialiseField(fieldWidth, fieldHeight, EMPTY);
	        generateEnv(theField);
	        fieldIsLoad = false;
	    }
        fieldHeight = theField->height;
        fieldWidth = theField->width;
		        
		switch (menuChoice)
		{
			//New neural network
			case TRAIN_NN:
				neuralNetwork = trainingNN1(RADIUS_VIEWPOINT, data, fieldHeight, fieldWidth, savingPathNN);
				break;
			//Load neural network
			case LOAD_NN:
				neuralNetwork = loadNeuralNetwork(savingPathNN);
				break;
			//New genetic network
			case TRAIN_GN:
			    if (argc >= 3)
			        labelingWeights = trainingGN1(data, fieldHeight, fieldWidth, savingPathGN, argv[2], 20, 50);
			    else
			        labelingWeights = trainingGN1(data, fieldHeight, fieldWidth, savingPathGN, NULL, 20, 50);
			    break;
			//Load genetic network
			case LOAD_GN:
		        if (argc >= 3)
                    labelingWeights = loadGeneticNetwork(argv[2]);
                else
                    printf("Error : Invalid arguments\n");
			    break;
			default:
				printf("Error : Invalid arguments\n");
				break;
		}
		
		windowWidth = (fieldWidth*tileSize) + /*offset*/ 2*tileSize + (RADIUS_VIEWPOINT*2*tileSize) + 2*tileSize;
		windowHeight = fieldHeight*tileSize;
		SDL_SetWindowSize(window, windowWidth, windowHeight);
		
		//--- Main loop
		
		while(data->endEvent == false)
		{   
			//Initiate the entity, the start and end of the route according to the field
			Entity* entity = initialiseEntity(0, 0, RADIUS_VIEWPOINT, fieldWidth, fieldHeight);
			node* startNode = nearestNode(theField, entity->x, entity->y);
			entity->x = startNode->x;
			entity->y = startNode->y;
			destructNodes(&startNode);
			node* endNode = nearestNode(theField, fieldWidth, fieldHeight);
		    
			//While the entity hasn't arrived
			while ((entity->x != endNode->x || entity->y != endNode->y) && !data->endEvent)
			{
				updateFieldOfViewEntity(theField, entity);
				updateMentalMapEntity(entity);

				node *path = NULL;
                if (menuChoice == LOAD_NN)
				    path = findNextPathNN(entity, endNode, data, neuralNetwork);
				else if (menuChoice == LOAD_GN)
				    path = findNextPathGN(entity, endNode, data, labelingWeights);
				
				int positionInPath = 0;
				node *nodePosition = NULL;
				//Move the entity along the path
				do
				{
					//get the next position
					positionInPath++;
					nodePosition = getNode(&path, positionInPath);
					if (nodePosition != NULL)
					{
						entity->x = nodePosition->x;
						entity->y = nodePosition->y;

						updateFieldOfViewEntity(theField, entity);
						updateMentalMapEntity(entity);
						
						//Clear the screen
						SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
						SDL_RenderClear(renderer);
						//Draw
						drawField(renderer, entity->mentalMap, tileSize);
						drawFieldOfViewEntity(renderer, entity, theField,tileSize);
						showEntity(entity, renderer, entityColor, tileSize);
						//Refresh the window
						SDL_RenderPresent(renderer);
						SDL_Delay(30);
					}
				}while(nodePosition != NULL && !data->endEvent);
				destructNodes(&path);
			}
			destructNodes(&endNode);
			
			// We load a new field if we use a random map
			if(!fieldIsLoad)
		    {
		        destructField(&theField);
		        theField = initialiseField(fieldWidth, fieldHeight, EMPTY);
	            generateEnv(theField);
		    }
			
			while(data->waitForInstruction)
			{
				SDL_Delay(50);
				if(event.type == SDL_TEXTINPUT && 
					(*event.text.text == 'r' || 
					*event.text.text == 'R'))
				{
					data->waitForInstruction = false;
				}
			}
			data->waitForInstruction = true;
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
