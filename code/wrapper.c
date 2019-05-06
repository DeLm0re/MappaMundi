/**
 * \file wrapper.c
 * \brief Wrapper functions
 * \author Théo Hipault
 * \version 0.1
 * \date 30/04/2019
 *
 * Module that contain all the functions to do the final steps of the program
 *
 */

//Header files containing the prototypes
#include "wrapper.h"


/**
 * \fn NeuralNetwork* trainingNN1(int visionRange, dataType *data, int fieldHeight, int fieldWidth, char *savingPathNN)
 * \brief creates a neural network and trains it on randomly generated fields of view, then saves it
 * 
 * \param
 * 		visionRange : size of the field of view used by the IA
 * 		data : structure which define the kind of event we have to raise for interruption
 * 		fieldHeight : height of the field
 * 		fieldWidth : width of the field
 * 		savingPathNN : path where to save the neural network
 * \return
 * 		NeuralNetwork*
 */
NeuralNetwork *trainingNN1(int visionRange, dataType *data, int fieldHeight, int fieldWidth, char *savingPathNN)
{
	//We create a neural network
	int neuronsPerLayers[5] = {surface2DCircle(visionRange) + 1, (surface2DCircle(visionRange) + 1)*2, surface2DCircle(visionRange) + 1, 2, 1};
	NeuralNetwork *neuralNetwork = createNeuralNetwork(5, neuronsPerLayers, -0.5, 0.5);
	
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
		fieldInput = generateRandomFieldOfView(visionRange, true);
		
		//We create new random coordinates for the position of an entity and for the end coordinate
		int xPosition = rand()%fieldWidth;
		int yPosition = rand()%fieldHeight;
		int xFinalPosition = rand()%fieldWidth;
		int yFinalPosition = rand()%fieldHeight;
		//We convert the field of view to an input for the neural network
		inputs = createInput(fieldInput, xPosition, yPosition, xFinalPosition, yFinalPosition);
		//We create the expected output with the labeling function
		referenceOutputs[0] = labeling1(fieldInput, xPosition, yPosition, xFinalPosition, yFinalPosition);
		
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

	return neuralNetwork;
}

/**
 * \fn NeuralNetwork* trainingNN2(dataType *data, Field *field, char *savingPathNN)
 * \brief creates a neural network and trains it on a field, then saves it
 * 
 * \param
 * 		data : structure which define the kind of event we have to raise for interruption
 * 		fieldName : the name of the field where to train the neural network
 * 		savingPathNN : path where to save the neural network
 * 		renderer : renderer used to draw with the SDL
 * 		tileSize : size of a tile for display
 *      entityColor : color of the entity to display
 * \return
 * 		NeuralNetwork*
 */
NeuralNetwork *trainingNN2(dataType *data, char* fieldName, char *savingPathNN, SDL_Renderer *renderer, const int tileSize, SDL_Color entityColor)
{
	//We create the field
	Field* field = createCustomField(fieldName);

	//We create a neural network
	int nbTiles = field->height * field->width;
	int neuronsPerLayers[5] = {nbTiles + 4, nbTiles*2, nbTiles, 8, 2};
	NeuralNetwork *neuralNetwork = createNeuralNetwork(5, neuronsPerLayers, -0.5, 0.5);
	
	//Some variables used for the neural network
	InputNeuralNetwork* inputs = NULL; //use to store the inputs of the neural network
	
	//Some variables used for the learning
	int correctAnswer = 0;
	float successRate = 0;
	int nbLearning = 0;
	// While the neural network is not correct 100% of the time
	while ((successRate < 0.95 && data->endEvent == false))
	{
		//Initialise the entity
		Entity *entity = initialiseEntity(0, 0, RADIUS_VIEWPOINT, field->width, field->height);
		//Initialisation of the nodes
		node* startNode = nearestNode(field, entity->x, entity->y);
		//Updates the position of the entity for the nearest starting node
		entity->x = startNode->x;
		entity->y = startNode->y;
		destructNodes(&startNode);
		node* endNode = nearestNode(field, field->width, field->height);
		

		//While the entity hasn't arrived
		while ((entity->x != endNode->x || entity->y != endNode->y) && !data->endEvent)
		{	
			//Updates the field of view of our entity
			updateFieldOfViewEntity(field, entity);
			//Updates the mental map of our entity with its new field of view
			updateMentalMapEntity(entity);

			float *input = createInputNN2(entity->mentalMap, entity->x, entity->y, endNode->x, endNode->y);
			node *path = findNextPathNN2(entity, data, neuralNetwork, input);
			
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
					//Find the expected choice
					float *expectedOutput = labeling2(entity, endNode->x, endNode->y, field, data);
					node *expectedNode = initNode(expectedOutput[0], expectedOutput[1], 0, 0);

					//Updates the position of the entity for the nearest starting node
					entity->x = nodePosition->x;
					entity->y = nodePosition->y;

					//Updates the field of view of our entity
					updateFieldOfViewEntity(field, entity);

					//Updates the mental map of our entity with its new field of view
					updateMentalMapEntity(entity);
					
					//Clear the screen
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
					SDL_RenderClear(renderer);
					drawField(renderer, entity->mentalMap, tileSize);
					drawFieldOfViewEntity(renderer, entity, field,tileSize);

					//SDL_Color decisionColor = {255, 0, 0, 255};
					//viewNodes(&path, renderer, decisionColor, tileSize);

					SDL_Color decisionColor = {255, 0, 0, 255};
					viewNodes(&expectedNode, renderer, decisionColor, tileSize);

					showEntity(entity, renderer, entityColor, tileSize);

					

					//Refresh the window
					SDL_RenderPresent(renderer);
					SDL_Delay(30);
					
					//We make the neural network learn and if he andswered correctly
					if(superviseLearningNeuralNetwork(neuralNetwork, input, expectedOutput, 0.001, 0.1))
					{
						// We count it as a new correct answer
						correctAnswer++;
					}
					//We increment the number of learning
					nbLearning++;

				}
			}while(nodePosition != NULL && !data->endEvent);
			destructNodes(&path);
		}
		
		//At the end of this map we calculate the average success
		successRate = ((float) correctAnswer)/nbLearning;
		printf("successRate : %f, %d\n", successRate, nbLearning);
		//We reset the number of learning and the number of success
		correctAnswer = 0;
	}
	destructInput(&inputs);

	saveNeuralNetwork(neuralNetwork, savingPathNN);

	return neuralNetwork;
}