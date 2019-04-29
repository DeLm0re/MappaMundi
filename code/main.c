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
int main(int argc, char** argv)
{
	if (argc < 2) 
	{
		printf("Please, enter an argument : \n 1 : Create a new neural network and train it\n 2 : Load an existing neural network\n 3 : Start a new genetic network and train it\n 4 [path] : Load an existing genetic network\n");
	}
	else
	{
		//Initialize the random seed value
		srand(12345);
		
		// Create a color for the entity that will move
		SDL_Color entityColor = {80, 160, 160, 255};
		SDL_Color wantedPositionColor = {160, 80, 80, 255};

		//Initialisation and generation of a field :
			Field *theField = initialiseField(50, 50, EMPTY);
			generateEnv(theField);
		//Creation of a field by using a custom field in CUSTOM_FIELD_PATH (prototype/h)
			//Field *theField = createCustomField("grande");
		
		//Declaration of basic constants
		int fieldHeight = theField->height;
		int fieldWidth = theField->width;

		//Size of a tile (ex: wall)
		const int tileSize = 5;

		// init a width and height for the windows
		int windowWidth = ((fieldWidth*tileSize) + 40 + (RADIUS_VIEWPOINT*2*tileSize));
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

		//Declaration of the nodes where the entity will start and where it wants to go
		node* startNode = NULL;
		node* endNode = NULL;
		//Use to store the path found by the pathfinding
		node* path = NULL;
		//Declaration of the entity wich will be used by the neural network
		Entity* entity = NULL;
		//Used to make the entity move along the path found
		int positionInPath = 0;
		node* nodePosition = NULL; //The position of the entity
		node* wantedPosition = NULL; //The position the entity want to explore
	
		char savingPathNN[256] = "../NN/Reseau1.nn";
		char savingPathGN[256] = "../GN";

		NeuralNetwork* neuralNetwork = NULL;
		LabelingWeights* labelingWeights = NULL;
		
		int menuChoice = atoi(argv[1]);

		switch (menuChoice)
		{
			//New neural network
			case TRAIN_NN:			
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
		        if (argc == 3)
                    labelingWeights = loadGeneticNetwork(argv[2]);
                else
                    printf("Error : Invalid arguments\n");
			    break;
			default:
				printf("Error : Invalid arguments\n");
				break;
		}
	    
		if (menuChoice == TRAIN_NN)
		{
			//We create a neural network
			int neuronsPerLayers[5] = {surface2DCircle(RADIUS_VIEWPOINT) + 1, (surface2DCircle(RADIUS_VIEWPOINT) + 1)*2, surface2DCircle(RADIUS_VIEWPOINT) + 1, 2, 1};
			neuralNetwork = createNeuralNetwork(5, neuronsPerLayers, -0.5, 0.5);
			
			//Some variables used for the neural network
			Field* fieldInput = NULL; //use to store the field of view that will be used as an input for our neural network
			InputNeuralNetwork* inputs = NULL; //use to store the inputs of the neural network
			float referenceOutputs[1] = {0}; //use to store the expected output of the neural network
			
			//Some variables used for the learning
			int correctAnswer = 0;
			float successRate = 0;
			int nbLearning = 0;
			// While the neural network is not correct 100% of the time
			while ((successRate < 0.95 && data->endEvent == false) || nbLearning < 100000)
			{
				//We create a new field of view
				fieldInput = generateRandomFieldOfView(RADIUS_VIEWPOINT, true);
				
				//We create new random coordinates for the position of an entity and for the end coordinate
				int xPosition = rand()%fieldWidth;
				int yPosition = rand()%fieldHeight;
				int xFinalPosition = rand()%fieldWidth;
				int yFinalPosition = rand()%fieldHeight;
				//We convert the field of view to an input for the neural network
				inputs = createInput(fieldInput, xPosition, yPosition, xFinalPosition, yFinalPosition);
				//We create the expected output with the labeling function
				referenceOutputs[0] = labeling2(fieldInput, xPosition, yPosition, xFinalPosition, yFinalPosition);
				
				//We make the neural network learn and if he andswered correctly
				if(superviseLearningNeuralNetwork(neuralNetwork, inputs->data, referenceOutputs, 0.001, 0.1))
				{
					// We count it as a new correct answer
					correctAnswer++;
				}
				
				//We increment the number of learning
				nbLearning++;
				//Every 200 learnings
				if(nbLearning%200 == 0)
				{
					//We calculate the average success
					successRate = ((float) correctAnswer)/200;
					printf("successRate : %f, %d\n", successRate, nbLearning);
					//We reset the number of learning and the number of success
					correctAnswer = 0;
				}
				
				//We free the inputs for the next loop
				destructInput(&inputs);
				destructField(&fieldInput);
			}
			
			saveNeuralNetwork(neuralNetwork, savingPathNN);
		}
		
		if (menuChoice == TRAIN_GN)
		{
		    //Initialisation and generation of a field :
			theField = initialiseField(fieldWidth, fieldHeight, EMPTY);
			generateEnv(theField);
			
			//We create a random batch of genetic network
			int nbMember = 100;
			int nbGeneration = 5;
			GeneticNetworks* geneticNetworks = NULL;
			if (argc == 2)
			    geneticNetworks = initialiseGeneticNetworks(nbMember);
			else if (argc == 3)
			    geneticNetworks = initialiseGeneticNetworksFrom_(nbMember, argv[2], 0.01);
			
			//For each generation
			int generationIndex;
			for (generationIndex = 0; generationIndex < nbGeneration; generationIndex++)
			{
		        //We create a new generation absed on half the best individuals
		        GeneticNetworks* temp = createNewGeneration(geneticNetworks, nbMember/2, 0.01);
		        destructGeneticNetworks(&geneticNetworks);
		        geneticNetworks = temp;
			    //For each genetic networks of the generation
			    int networkIndex;
			    for (networkIndex = 0; networkIndex < geneticNetworks->size; networkIndex++)
			    {
			        printf("Generation : %d, member : %d\n", generationIndex, networkIndex);
			        //Initialise the entity
			        entity = initialiseEntity(0, 0, RADIUS_VIEWPOINT, fieldWidth, fieldHeight);
			        //Initialisation of the nodes
			        startNode = nearestNode(theField, entity->x, entity->y);
			        //Updates the position of the entity for the nearest starting node
			        entity->x = startNode->x;
			        entity->y = startNode->y;
			        endNode = nearestNode(theField, fieldWidth, fieldHeight);
			
			        //While the entity hasn't arrived
			        while (
			            (entity->x != endNode->x || entity->y != endNode->y) && 
			            geneticNetworks->score[networkIndex] < fieldHeight*fieldWidth)
			        {

				        //--- Pathfinding algorithm and movement along it
				
				        //Updates the field of view of our entity
				        updateFieldOfViewEntity(theField, entity);
				        //Updates the mental map of our entity with its new field of view
				        updateMentalMapEntity(entity);
				
				        //We initialize an interest field
				        InterestField* interestField = initialiseInterestField(entity->mentalMap->width, entity->mentalMap->height);
				        //We update each values of the interest field with what our genetic network think
				        updateInterestField2(interestField, entity->mentalMap, endNode->x, endNode->y, entity->visionRange, geneticNetworks->list[networkIndex]);
				
				        //We set a default wanted node
				        wantedPosition = cpyNode(endNode);
				        //We update the start node of the pathfinding
				        startNode->x = entity->x;
				        startNode->y = entity->y;
				        //We try to find a path
				        while((path == startNode || path == NULL))
				        {
					        destructNodes(&path);
					        //We try to find a path
					        path = findPathFrom_To_(startNode, wantedPosition, entity->mentalMap, &(data->endEvent));
					        //If we haven't find a path
					        if ((path == startNode || path == NULL))
					        {
						        //We change our wanted node to the best position found by the neural network
						        updateBestWantedPosition(wantedPosition, interestField);
					        }
				        }
				        //We free the interest field from the memory
				        destructInterestField(&interestField);
				        //We reset the path position
				        positionInPath = 0;
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
						        
						        //Increase the score of the genetic network
						        geneticNetworks->score[networkIndex]++;

						        //Updates the field of view of our entity
						        updateFieldOfViewEntity(theField, entity);

						        //Updates the mental map of our entity with its new field of view
						        updateMentalMapEntity(entity);
					        }
				        }while(nodePosition != NULL);
				        //Free the memory of all the nodes use for the pathfinding
				        destructNodes(&path);
				        path = NULL;
				        destructNodes(&startNode);
				        destructNodes(&endNode);
				        // We free the wantedPosition from the memory
				        destructNodes(&wantedPosition);
			        }
			    }
			    
			}
			//Free the memory of the field
			destructField(&theField);
			//We sort the last generation
			sortGeneticNetworks(geneticNetworks);
			//Then, we get the best of the generation to be the labeling weights
			labelingWeights = geneticNetworks->list[0];
			//We save the neural network
			int nbGN = getNumberOfFilesInDirectory(savingPathGN);
			char strBuffer[256] = "";
			sprintf(strBuffer, "%s/genome%ds%fg%dm%d.gn", savingPathGN, nbGN, geneticNetworks->score[0], nbGeneration, nbMember);
			saveGeneticNetwork(labelingWeights, strBuffer);
		}
	
		//--- Main loop
		
		while(data->endEvent == false)
		{
		    //Initialisation and generation of a field :
			    theField = initialiseField(fieldWidth, fieldHeight, EMPTY);
			    generateEnv(theField);
		    //Creation of a field by using a custom field in CUSTOM_FIELD_PATH (prototype/h)
			    //theField = createCustomField("grande");
			//Declaration of basic constants
		    fieldHeight = theField->height;
		    fieldWidth = theField->width;
			    
			//Initialise the entity
			entity = initialiseEntity(0, 0, RADIUS_VIEWPOINT, fieldWidth, fieldHeight);
			//Initialisation of the nodes
			startNode = nearestNode(theField, entity->x, entity->y);
			//Updates the position of the entity for the nearest starting node
			entity->x = startNode->x;
			entity->y = startNode->y;
			endNode = nearestNode(theField, fieldWidth, fieldHeight);
			
			//While the entity hasn't arrived
			while ((entity->x != endNode->x || entity->y != endNode->y) && !data->endEvent)
			{

				//--- Pathfinding algorithm and movement along it
				
				//Updates the field of view of our entity
				updateFieldOfViewEntity(theField, entity);
				//Updates the mental map of our entity with its new field of view
				updateMentalMapEntity(entity);
				
				//We initialize an interest field
				InterestField* interestField = initialiseInterestField(entity->mentalMap->width, entity->mentalMap->height);
				if ((menuChoice == TRAIN_NN || menuChoice == LOAD_NN) && neuralNetwork != NULL)
				{
				    //We update each values of the interest field with what our neural network think
				    updateInterestField(interestField, neuralNetwork, entity->mentalMap, endNode->x, endNode->y, entity->visionRange);
				}
				else if (labelingWeights != NULL)
				{
				    //We update each values of the interest field with what our genetic network think
				    updateInterestField2(interestField, entity->mentalMap, endNode->x, endNode->y, entity->visionRange, labelingWeights);
				}
				
				//We set a default wanted node
				wantedPosition = cpyNode(endNode);
				//We update the start node of the pathfinding
				startNode->x = entity->x;
				startNode->y = entity->y;
				//We try to find a path
				while((path == startNode || path == NULL) && !data->endEvent)
				{
					destructNodes(&path);
					//We try to find a path
					path = findPathFrom_To_(startNode, wantedPosition, entity->mentalMap, &(data->endEvent));
					//If we haven't find a path
					if ((path == startNode || path == NULL))
					{
						//We change our wanted node to the best position found by the neural network
						updateBestWantedPosition(wantedPosition, interestField);
					}
				}
				//We free the interest field from the memory
				destructInterestField(&interestField);
				
				//We reset the path position
				positionInPath = 0;
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
						drawFieldOfViewEntity(renderer, entity, theField,tileSize);
						//We draw the wanted position
						viewNodes(&wantedPosition, renderer, wantedPositionColor, tileSize);
						//Draw the entity
						showEntity(entity, renderer, entityColor, tileSize);
						//Refresh the window
						SDL_RenderPresent(renderer);
						//We wait 30ms at each step to see the entity moving
						SDL_Delay(30);
					}
				}while(nodePosition != NULL && !data->endEvent);
				//Free the memory of all the nodes use for the pathfinding
				destructNodes(&path);
				path = NULL;
				destructNodes(&startNode);
				destructNodes(&endNode);
				// We free the wantedPosition from the memory
				destructNodes(&wantedPosition);
			}
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
