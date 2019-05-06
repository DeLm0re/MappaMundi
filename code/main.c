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
		printf("Please, enter an argument :\n1 : Create a new neural network and train it\n2 : Load an existing neural network\n3 : Start a new genetic network and train it\n4 : Load an existing genetic network\n");
	}
	else
	{
		//Initialize the random seed value
		srand(12345);
		
		// Create a color for the entity that will move
		SDL_Color entityColor = {80, 160, 160, 255};

		//Initialisation and generation of a field :
			//theField = initialiseField(fieldWidth, fieldHeight, EMPTY);
			//generateEnv(theField);
		//Creation of a field by using a custom field in CUSTOM_FIELD_PATH (prototype/h)
		Field *theField = createCustomField("petite");
		
		//Declaration of basic constants
		int fieldHeight = theField->height;
		int fieldWidth = theField->width;

		//Size of a tile (ex: wall)
		const int tileSize = 15;

		// init a width and height for the windows
		int windowWidth = (fieldWidth*tileSize) + /*offset*/ 2*tileSize + (RADIUS_VIEWPOINT*2*tileSize) + 2*tileSize;
		int windowHeight = fieldHeight*tileSize;
		
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

		char trainingMapNN[256] = "petite";
		char savingPathNN[256] = "../NN/Reseau1.nn";
		char savingPathGN[256] = "../GN/genome.gn";

		NeuralNetwork* neuralNetwork = NULL;
		LabelingWeights* labelingWeights = NULL;
		
		int menuChoice = atoi(argv[1]);

		switch (menuChoice)
		{
			//New neural network
			case TRAIN_NN:
				//neuralNetwork = trainingNN1(RADIUS_VIEWPOINT, data, fieldHeight, fieldWidth, savingPathNN);
				neuralNetwork = trainingNN2(data, trainingMapNN, savingPathNN, renderer, tileSize, entityColor);
				break;
			//Load neural network
			case LOAD_NN:
				neuralNetwork = loadNeuralNetwork(savingPathNN);
				break;
			//New genetic network
			case TRAIN_GN:
			    break;
			//Load genetic network
			case LOAD_GN:
                labelingWeights = loadGeneticNetwork(savingPathGN);
			    break;
			default:
				printf("Error : Invalid arguments");
				break;
		}
	
		//--- Main loop
		
		while(data->endEvent == false)
		{
		    //Initialisation and generation of a field :
			    //theField = initialiseField(fieldWidth, fieldHeight, EMPTY);
			    //generateEnv(theField);
		    //Creation of a field by using a custom field in CUSTOM_FIELD_PATH (prototype/h)
			theField = createCustomField("petite");
			//Declaration of basic constants
		    fieldHeight = theField->height;
		    fieldWidth = theField->width;

			//Declaration of the entity wich will be used by the neural network
			Entity* entity = NULL;

			//Initialise the entity
			entity = initialiseEntity(0, 0, RADIUS_VIEWPOINT, fieldWidth, fieldHeight);
			//Initialisation of the nodes
			node* startNode = nearestNode(theField, entity->x, entity->y);
			//Updates the position of the entity for the nearest starting node
			entity->x = startNode->x;
			entity->y = startNode->y;
			destructNodes(&startNode);
			node* endNode = nearestNode(theField, fieldWidth, fieldHeight);
			
			//While the entity hasn't arrived
			while ((entity->x != endNode->x || entity->y != endNode->y) && !data->endEvent)
			{	
				//Updates the field of view of our entity
				updateFieldOfViewEntity(theField, entity);
				//Updates the mental map of our entity with its new field of view
				updateMentalMapEntity(entity);

				node *path = findNextPathNN(entity, endNode, data, neuralNetwork);
				
				//We set the path position
				int positionInPath = 0;

				node *nodePosition = NULL;
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
			//While the waiting flag is set to true (waiting for inputs)
			while(data->waitForInstruction)
			{
				SDL_Delay(50);
				if(event.type == SDL_TEXTINPUT && 
					(*event.text.text == 'r' || 
					*event.text.text == 'R'))
				{
					//We set the waiting flag to false (not waiting for inputs anymore)
					data->waitForInstruction = false;
				}
			}
			//We put the waiting flag back to true
			data->waitForInstruction = true;

			//Free the memory of the field
			destructField(&theField);
		}

		//Free the neural network from the memory
		destructNeuralNetwork(&neuralNetwork);
		
		//Free the labeling weights from the memory
		destructLabelingWeights(&labelingWeights);

		//Ending the thread
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
