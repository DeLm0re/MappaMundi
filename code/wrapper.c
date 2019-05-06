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

	return neuralNetwork;
}

/**
 * \fn NeuralNetwork* trainingGN1(int visionRange, dataType *data, int fieldHeight, int fieldWidth, char *savingPathGN)
 * \brief creates a neural network and trains it on randomly generated fields of view, then saves it
 * 
 * \param
 * 		data : structure which define the kind of event we have to raise for interruption
 * 		fieldHeight : height of the field
 * 		fieldWidth : width of the field
 * 		savingPathGN : path where to save the genetic network
 *      basePathGN : path to a genetic network from which the first generation will be based on.
 *                  If NULL, then a default first generation will be created
 *      nbGeneration : the number of generation the training wil go throught
 *      nbMember : the number of member tested by generation
 * \return
 * 		LabelingWeights*
 */
LabelingWeights *trainingGN1(dataType *data, int fieldHeight, int fieldWidth, char *savingPathGN, char* basePathGN, int nbGeneration, int nbMember)
{
    //Initialisation and generation of a field :
	Field* theField = initialiseField(fieldWidth, fieldHeight, EMPTY);
	generateEnv(theField);
	
	GeneticNetworks* geneticNetworks = NULL;
	if (basePathGN == NULL)
	    geneticNetworks = initialiseGeneticNetworks(nbMember);
	else
	    geneticNetworks = initialiseGeneticNetworksFrom(nbMember, basePathGN, 0.01);
	
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
	        Entity* entity = initialiseEntity(0, 0, RADIUS_VIEWPOINT, fieldWidth, fieldHeight);
	        //Initialisation of the nodes
	        node* startNode = nearestNode(theField, entity->x, entity->y);
	        //Updates the position of the entity for the nearest starting node
	        entity->x = startNode->x;
	        entity->y = startNode->y;
	        node* endNode = nearestNode(theField, fieldWidth, fieldHeight);
	
	        //While the entity hasn't arrived
	        while (
	            (entity->x != endNode->x || entity->y != endNode->y) && 
	            geneticNetworks->score[networkIndex] < fieldHeight*fieldWidth)
	        {
		
		        //Updates the field of view of our entity
		        updateFieldOfViewEntity(theField, entity);
		        //Updates the mental map of our entity with its new field of view
		        updateMentalMapEntity(entity);
		
		        //We initialize an interest field
		        InterestField* interestField = initialiseInterestField(entity->mentalMap->width, entity->mentalMap->height);
		        //We update each values of the interest field with what our genetic network think
		        updateInterestField2(interestField, entity->mentalMap, endNode->x, endNode->y, entity->visionRange, geneticNetworks->list[networkIndex]);
		
		        //We set a default wanted node
		        node* wantedPosition = cpyNode(endNode);
		        node* path = NULL;
		        //We update the start node of the pathfinding
		        startNode->x = entity->x;
		        startNode->y = entity->y;
		        //We try to find a path
		        while((path == startNode || path == NULL))
		        {
			        destructNodes(&path);
			        //We try to find a path
			        path = findPathFromStartEnd(startNode, wantedPosition, entity->mentalMap, &(data->endEvent));
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
		        int positionInPath = 0;
		        node* nodePosition = NULL;
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
		        // We free the wantedPosition from the memory
		        destructNodes(&wantedPosition);
	        }
	        destructEntity(&entity);
	        destructNodes(&startNode);
		    destructNodes(&endNode);
	    }
	}
	//Free the memory of the field
	destructField(&theField);
	//We sort the last generation
	sortGeneticNetworks(geneticNetworks);
	//Then, we get the best of the generation to be the labeling weights
	LabelingWeights* labelingWeights = geneticNetworks->list[0];
	//We save the neural network
	int nbGN = getNumberOfFilesInDirectory(savingPathGN);
	char strBuffer[256] = "";
	sprintf(strBuffer, "%s/genome%ds%fg%dm%d.gn", savingPathGN, nbGN, geneticNetworks->score[0], nbGeneration, nbMember);
	saveGeneticNetwork(labelingWeights, strBuffer);
	
	return labelingWeights;
}
