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
	while ((successRate < 0.95 && !data->endEvent) || nbLearning < 100000)
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
 * \fn NeuralNetwork* trainingGN1(int visionRange, dataType *data, Field* theField, char *savingPathGN)
 * \brief creates a neural network and trains it on randomly generated fields of view, then saves it
 * 
 * \param
 * 		data : structure which define the kind of event we have to raise for interruption
 *      theField : the field on which the genetic network will be trained
 * 		savingPathGN : path where to save the genetic network
 *      basePathGN : path to a genetic network from which the first generation will be based on.
 *                  If NULL, then a default first generation will be created
 *      nbGeneration : the number of generation the training wil go throught
 *      nbMember : the number of member tested by generation
 * \return
 * 		LabelingWeights*
 */
LabelingWeights *trainingGN1(dataType *data, Field* theField, char *savingPathGN, char* basePathGN, int nbGeneration, int nbMember)
{
    //Initialisation and generation of a field
	InterestField* interestField = initialiseInterestField(theField->width, theField->height);
	
	//Creation of the first generation
	GeneticNetworks* geneticNetworks = initialiseGeneticNetworksFrom(nbMember, basePathGN, 0.05);
	if (geneticNetworks == NULL)
	    geneticNetworks = initialiseGeneticNetworks(nbMember);
	
	int generationIndex;
	for (generationIndex = 0; generationIndex < nbGeneration; generationIndex++)
	{   
	    if (generationIndex > 0)
	    {
	        //We create a new generation based on half the best individuals of the previous one
            GeneticNetworks* temp = createNewGeneration(geneticNetworks, nbMember/2, 0.05);
            destructGeneticNetworks(&geneticNetworks);
            geneticNetworks = temp;
        }
        
	    int timeStartGeneration = clock();
	    int sumScoreGeneration = 0;
	    int networkIndex;
	    for (networkIndex = 0; networkIndex < geneticNetworks->size; networkIndex++)
	    {
	        //Initiate the entity, the start and end of the route according to the field
	        Entity* entity = initialiseEntity(0, 0, RADIUS_VIEWPOINT, theField->width, theField->height);
	        node* startNode = nearestNode(theField, entity->x, entity->y);
	        entity->x = startNode->x;
	        entity->y = startNode->y;
	        node* endNode = nearestNode(theField, theField->width, theField->height);
	        clock_t timeStartMember = clock();
	        //While the entity hasn't arrived
	        while (
	            (entity->x != endNode->x || entity->y != endNode->y) && 
	            geneticNetworks->score[networkIndex] < theField->height*theField->width &&
	            !data->endEvent)
	        {
		        updateFieldOfViewEntity(theField, entity);
		        updateMentalMapEntity(entity);
		        
		        updateInterestField2(interestField, endNode->x, endNode->y, entity, geneticNetworks->list[networkIndex]);
		
		        //We set a default node to which the entity will try to move to
		        node* wantedPosition = cpyNode(endNode);
		        node* path = NULL;
		        //We update the start node of the pathfinding
		        startNode->x = entity->x;
		        startNode->y = entity->y;
		        //We search for a path based on the interest field
		        while((path == startNode || path == NULL) && !data->endEvent)
		        {
			        destructNodes(&path);
			        path = findPathFromStartEnd(startNode, wantedPosition, entity->mentalMap, &(data->endEvent));
			        if ((path == startNode || path == NULL))
			        {
				        updateBestWantedPosition(wantedPosition, interestField);
			        }
		        }
		        free(wantedPosition);
		        
		        geneticNetworks->score[networkIndex] += getNbNode(&path);
		        moveEntityAlongPath(data, entity, path, theField, NULL, 0, 0);
	        }
	        sumScoreGeneration += geneticNetworks->score[networkIndex];
	        geneticNetworks->time[networkIndex] = (clock()-timeStartMember)/((float)CLOCKS_PER_SEC);
	        printf("Gen : %d, member : %d, time : %.3f sec, score : %.0f\n", generationIndex, networkIndex, geneticNetworks->time[networkIndex], geneticNetworks->score[networkIndex]);
	        
            destructEntity(&entity);
	        destructNodes(&startNode);
		    destructNodes(&endNode);
	    }
	    printf("\ttotal time : %.3f sec\n", (clock()-timeStartGeneration)/((float)CLOCKS_PER_SEC));
	    printf("\taverage time : %.3f sec, average score %.3f\n", (clock()-timeStartGeneration)/((float)CLOCKS_PER_SEC)/nbMember, sumScoreGeneration/((float)nbMember));
	    sortGeneticNetworks(geneticNetworks);
	    printf("\tbest : score : %.3f, time : %.3f\n", geneticNetworks->score[0], geneticNetworks->time[0]);
	    
	}
    destructInterestField(&interestField);
    
	LabelingWeights* labelingWeights = geneticNetworks->list[0];

    printf("\n");
    printf("\tdist : %f\n", labelingWeights->weights[DIST]);
    printf("\tnbEmpty : %f\n", labelingWeights->weights[NB_EMPTY]);
    printf("\tnbWall : %f\n", labelingWeights->weights[NB_WALL]);
    printf("\tnbFog : %f\n", labelingWeights->weights[NB_FOG]);
    printf("\tnbVisited : %f\n", labelingWeights->weights[NB_VISITED]);
    printf("\tavgEmpty : %f\n", labelingWeights->weights[AVG_DIST_EMPTY]);
    printf("\tavgWall : %f\n", labelingWeights->weights[AVG_DIST_WALL]);
    printf("\tavgFog : %f\n", labelingWeights->weights[AVG_DIST_FOG]);
    printf("\tavgVisited : %f\n", labelingWeights->weights[AVG_DIST_VISITED]);
    printf("\tdistFromEntity : %f\n", labelingWeights->weights[DIST_FROM_ENTITY]);
	
	if (!data->endEvent)
	{
	    int nbGN = getNumberOfFilesInDirectory(savingPathGN);
	    char strBuffer[256] = "";
	    sprintf(strBuffer, "%s/genomePoint%ds%.3ft%.3fg%dm%d.gn", savingPathGN, nbGN, geneticNetworks->score[0], geneticNetworks->time[0], nbGeneration, nbMember);
	    saveGeneticNetwork(labelingWeights, strBuffer);
	}
	
	return labelingWeights;
}

/**
 * \fn NeuralNetwork* trainingGN2(int visionRange, dataType *data, Field* theField, char *savingPathGN)
 * \brief creates a neural network and trains it on randomly generated fields of view, then saves it
 * 
 * \param
 * 		data : structure which define the kind of event we have to raise for interruption
 *      theField : the field on which the genetic network will be trained
 * 		savingPathGN : path where to save the genetic network
 *      basePathGN : path to a genetic network from which the first generation will be based on.
 *                  If NULL, then a default first generation will be created
 *      nbGeneration : the number of generation the training wil go throught
 *      nbMember : the number of member tested by generation
 *      percentReveal : the percentage of map revealed at which we concider that the entity finished his job
 * \return
 * 		LabelingWeights*
 */
LabelingWeights *trainingGN2(dataType *data, Field* theField, char *savingPathGN, char* basePathGN, int nbGeneration, int nbMember, float percentReveal)
{
    //Initialisation and generation of a field
	InterestField* interestField = initialiseInterestField(theField->width, theField->height);
	
	//Creation of the first generation
	GeneticNetworks* geneticNetworks = initialiseGeneticNetworksFrom(nbMember, basePathGN, 0.05);
	if (geneticNetworks == NULL)
	    geneticNetworks = initialiseGeneticNetworks(nbMember);
	
	int generationIndex;
	for (generationIndex = 0; generationIndex < nbGeneration; generationIndex++)
	{   
	    if (generationIndex > 0)
	    {
	        //We create a new generation based on half the best individuals of the previous one
            GeneticNetworks* temp = createNewGeneration(geneticNetworks, nbMember/2, 0.05);
            destructGeneticNetworks(&geneticNetworks);
            geneticNetworks = temp;
        }
        
	    int timeStartGeneration = clock();
	    int sumScoreGeneration = 0;
	    int networkIndex;
	    for (networkIndex = 0; networkIndex < geneticNetworks->size; networkIndex++)
	    {
	        //Initiate the entity, the start and end of the route according to the field
	        Entity* entity = initialiseEntity(0, 0, RADIUS_VIEWPOINT, theField->width, theField->height);
	        node* startNode = nearestNode(theField, entity->x, entity->y);
	        entity->x = startNode->x;
	        entity->y = startNode->y;
	        node* endNode = nearestNode(theField, theField->width, theField->height);
	        clock_t timeStartMember = clock();
	        
	        int remainingFog = getNbFog(entity->mentalMap);
	        const int totalSize = theField->width*theField->height;
	        //While the entity hasn't arrived
	        while (
	            remainingFog > (1-percentReveal)*totalSize && 
	            geneticNetworks->score[networkIndex] < totalSize &&
	            !data->endEvent)
	        {
		        updateFieldOfViewEntity(theField, entity);
		        updateMentalMapEntity(entity);
		        
		        updateInterestField3(interestField, entity, geneticNetworks->list[networkIndex]);
		
		        //We set a default node to which the entity will try to move to
		        node* wantedPosition = cpyNode(endNode);
		        node* path = NULL;
		        //We update the start node of the pathfinding
		        startNode->x = entity->x;
		        startNode->y = entity->y;
		        //We search for a path based on the interest field
		        while((path == startNode || path == NULL) && !data->endEvent)
		        {
			        destructNodes(&path);
			        path = findPathFromStartEnd(startNode, wantedPosition, entity->mentalMap, &(data->endEvent));
			        if ((path == startNode || path == NULL))
			        {
				        updateBestWantedPosition(wantedPosition, interestField);
			        }
		        }
		        free(wantedPosition);
		        
		        remainingFog = getNbFog(entity->mentalMap);
		        geneticNetworks->score[networkIndex] += getNbNode(&path);
		        moveEntityAlongPath(data, entity, path, theField, NULL, 0, 0);
	        }
	        sumScoreGeneration += geneticNetworks->score[networkIndex];
	        geneticNetworks->time[networkIndex] = (clock()-timeStartMember)/((float)CLOCKS_PER_SEC);
	        printf("Gen : %d, member : %d, time : %.3f sec, score : %.0f\n", generationIndex, networkIndex, geneticNetworks->time[networkIndex], geneticNetworks->score[networkIndex]);
	        
            destructEntity(&entity);
	        destructNodes(&startNode);
		    destructNodes(&endNode);
	    }
	    printf("\ttotal time : %.3f sec\n", (clock()-timeStartGeneration)/((float)CLOCKS_PER_SEC));
	    printf("\taverage time : %.3f sec, average score %.3f\n", (clock()-timeStartGeneration)/((float)CLOCKS_PER_SEC)/nbMember, sumScoreGeneration/((float)nbMember));
	    sortGeneticNetworks(geneticNetworks);
	    printf("\tbest : score : %.3f, time : %.3f\n", geneticNetworks->score[0], geneticNetworks->time[0]);
	    
	}
    destructInterestField(&interestField);
    
	LabelingWeights* labelingWeights = geneticNetworks->list[0];

    printf("\n");
    printf("\tdist : %f\n", labelingWeights->weights[DIST]);
    printf("\tnbEmpty : %f\n", labelingWeights->weights[NB_EMPTY]);
    printf("\tnbWall : %f\n", labelingWeights->weights[NB_WALL]);
    printf("\tnbFog : %f\n", labelingWeights->weights[NB_FOG]);
    printf("\tnbVisited : %f\n", labelingWeights->weights[NB_VISITED]);
    printf("\tavgEmpty : %f\n", labelingWeights->weights[AVG_DIST_EMPTY]);
    printf("\tavgWall : %f\n", labelingWeights->weights[AVG_DIST_WALL]);
    printf("\tavgFog : %f\n", labelingWeights->weights[AVG_DIST_FOG]);
    printf("\tavgVisited : %f\n", labelingWeights->weights[AVG_DIST_VISITED]);
    printf("\tdistFromEntity : %f\n", labelingWeights->weights[DIST_FROM_ENTITY]);
	
	if (!data->endEvent)
	{
	    int nbGN = getNumberOfFilesInDirectory(savingPathGN);
	    char strBuffer[256] = "";
	    sprintf(strBuffer, "%s/genomeExplore%ds%.3ft%.3fg%dm%d.gn", savingPathGN, nbGN, geneticNetworks->score[0], geneticNetworks->time[0], nbGeneration, nbMember);
	    saveGeneticNetwork(labelingWeights, strBuffer);
	}
	
	return labelingWeights;
}

/**
 * \fn void moveEntityAlongPath(Entity* entity, node* pathToFollow, Field* theField, SDL_Renderer* renderer, int tileSize, dataType* data, bool displayOn)
 * \brief make an entity follow a path and update its mental map
 * 
 * \param
 * 		data : structure which define the kind of event we have to raise for interruption
 *      entity : the entity that wil follow the path and wil update its mental map
 *      pathToFollow : the path that the entity will follow
 *      theField : the field where the entity is moving
 *      renderer : the SDL renderer, use to visualize the entity on the map
 *      tileSize : the size of one tile on the map
 *      animationDelay : the amount of milliseconds the function will wait before each step of the entity
 *
 * \return
 * 		LabelingWeights*
 */
void moveEntityAlongPath(dataType *data, Entity* entity, node* pathToFollow, Field* theField, SDL_Renderer* renderer, int tileSize, int animationDelay)
{
    node* nodePosition = popNode(&pathToFollow);
    //Move the entity along the path
    while(entity != NULL && nodePosition != NULL && !data->endEvent)
    {
        entity->x = nodePosition->x;
        entity->y = nodePosition->y;

        updateFieldOfViewEntity(theField, entity);
        updateMentalMapEntity(entity);
        
        free(nodePosition);
        nodePosition = popNode(&pathToFollow);
        
        if (renderer != NULL)
        {
            //Clear the screen
		    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		    SDL_RenderClear(renderer);
		    //Draw
		    drawField(renderer, entity->mentalMap, tileSize);
		    drawFieldOfViewEntity(renderer, entity, theField, tileSize);
		    SDL_Color entityColor = {80, 160, 160, 255};
		    showEntity(entity, renderer, entityColor, tileSize);
		    SDL_Color colorLastNodeInPath = {255, 0, 0, 255};
			node* lastNodeInPath = getLastNode(&pathToFollow);
		    viewNodes(&lastNodeInPath, renderer, colorLastNodeInPath, tileSize);
		    //Refresh the window
		    SDL_RenderPresent(renderer);
		    SDL_Delay(animationDelay);
		}
    }
    free(nodePosition);
}

/**
 * \fn void waitForInstruction(dataType *data)
 * \brief wait until the repeat key is pressed (R) or the quit key is pressed (Q)
 * 
 * \param
 * 		data : structure which define the kind of event we have to raise for interruption
 *
 * \return
 * 		void
 */
void waitForInstruction(dataType *data)
{
    data->waitForInstruction = true;
	while(data->waitForInstruction && !data->endEvent){SDL_Delay(50);}
}

/**
 * \fn NeuralNetwork* trainingNN2(dataType *data, char* fieldName, char *savingPathNN, SDL_Renderer *renderer, const int tileSize, SDL_Color entityColor)
 * \brief creates a neural network and trains it on random fields, then saves it
 * 
 * \param
 * 		fieldWidth, fieldHeight : dimensions of the random fields
 * 		data : structure which define the kind of event we have to raise for interruption
 * 		savingPathNN : path where to save the neural network
 * 		renderer : renderer used to draw with the SDL
 * 		tileSize : size of a tile for display
 * \return
 * 		NeuralNetwork*
 */
NeuralNetwork *trainingNN2(int fieldWidth, int fieldHeight, dataType *data, char *savingPathNN, SDL_Renderer *renderer, const int tileSize)
{
	//We create the field
	Field *field = initialiseField(fieldWidth, fieldHeight, EMPTY);
	generateEnv(field);

	//We create a neural network
	int nbTiles = field->height * field->width;
	int neuronsPerLayers[4] = {nbTiles + 4, nbTiles*2, nbTiles*2, nbTiles};
	NeuralNetwork *neuralNetwork = createNeuralNetwork(4, neuronsPerLayers, -0.5, 0.5);
	
	int nbMap = 0;

	// While the neural network is not correct 100% of the time
	while (nbMap <= 1000000 && !data->endEvent)
	{
		printf("Step n°%d\n", nbMap);
		trainNN2onField(neuralNetwork, data, field, renderer, tileSize);
		generateEnv(field);
		nbMap++;
	}

	saveNeuralNetwork(neuralNetwork, savingPathNN);

	return neuralNetwork;
}

/**
 * \fn void trainNN2onField(NeuralNetwork *neuralNetwork, dataType *data, Field* field, SDL_Renderer *renderer, const int tileSize, SDL_Color entityColor)
 * \brief trains a neural network on a single field
 * 
 * \param
 * 		neuralNetwork : the neural network to train
 * 		data : structure which define the kind of event we have to raise for interruption
 * 		field : the field where to train the neural network
 * 		renderer : renderer used to draw with the SDL
 * 		tileSize : size of a tile for display
 * \return
 * 		void
 */
void trainNN2onField(NeuralNetwork *neuralNetwork, dataType *data, Field* field, SDL_Renderer *renderer, const int tileSize)
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
		float *output = getOutputOfNeuralNetwork(neuralNetwork, input);
		node *choice = findNextPathNN2(entity, data, output);
		//Find the expected choice
		node *expectedNode = labeling2(entity, endNode->x, endNode->y, field, data);
		float *expectedOutput = convertLabeling2(field->width, field->height, expectedNode);
		node *expectedPath = findNextPathNN2(entity, data, expectedOutput);

		//We make the neural network learn
		superviseLearningNeuralNetwork(neuralNetwork, input, expectedOutput, 0.2, 0.1);

		moveEntityAlongPath(data, entity, expectedPath, field, renderer, tileSize, 1);
		
		destructNodes(&expectedNode);
		destructNodes(&choice);
		if(expectedOutput != NULL)	
			free(expectedOutput);
		if(output != NULL)	
			free(output);
		if(input != NULL)	
			free(input);
	}
}

/**
 * \fn void searchForEndPointNN(NeuralNetwork *neuralNetwork, dataType *data, Field* field, SDL_Renderer *renderer, const int tileSize, bool fieldIsFromImage)
 * \brief show the entity, starting in the top left corner, trying to go to the bottom right corner on the given field by using a neural network
 * 
 * \param
 * 		neuralNetwork : the neural network to use to take decisions
 * 		data : structure which define the kind of event we have to raise for interruption
 * 		field : the field where the entity will move
 * 		renderer : renderer used to draw with the SDL
 * 		tileSize : size of a tile for display
 *      fieldIsFromImage : to know if the field as been loaded from an image. If it is set to true, the map will not be update each loop
 * \return
 * 		void
 */
void searchForEndPointNN(NeuralNetwork *neuralNetwork, dataType *data, Field** field, SDL_Renderer *renderer, const int tileSize, bool fieldIsFromImage)
{
    while(!data->endEvent)
	{   
		//Initiate the entity, the start and end of the route according to the field
		Entity* entity = initialiseEntity(0, 0, RADIUS_VIEWPOINT, (*field)->width, (*field)->height);
		node* startNode = nearestNode((*field), entity->x, entity->y);
		entity->x = startNode->x;
		entity->y = startNode->y;
		destructNodes(&startNode);
		node* endNode = nearestNode(*field, (*field)->width, (*field)->height);
		
		updateFieldOfViewEntity(*field, entity);
		updateMentalMapEntity(entity);
	    
		//While the entity hasn't arrived at destination
		while ((entity->x != endNode->x || entity->y != endNode->y) && !data->endEvent)
		{
			node* path = findNextPathNN(entity, endNode, data, neuralNetwork);
	        moveEntityAlongPath(data, entity, path, *field, renderer, tileSize, 30);
		}
		destructNodes(&endNode);
		
		// We load a new field if we use a random map
		if(!fieldIsFromImage)
	    {
	        int fieldWidth = (*field)->width;
	        int fieldHeight = (*field)->height;
	        destructField(field);
	        *field = initialiseField(fieldWidth, fieldHeight, EMPTY);
            generateEnv(*field);
	    }
		destructEntity(&entity);
		waitForInstruction(data);
	}
	destructField(field);
}

/**
 * \fn void searchForEndPointNN(LabelingWeights *labelingWeights, dataType *data, Field* field, SDL_Renderer *renderer, const int tileSize, bool fieldIsFromImage)
 * \brief show the entity, starting in the top left corner, trying to go to the bottom right corner on the given field by using a genetic algorithm
 * 
 * \param
 * 		labelingWeights : the genetic algorithm to use to take decisions
 * 		data : structure which define the kind of event we have to raise for interruption
 * 		field : the field where the entity will move
 * 		renderer : renderer used to draw with the SDL
 * 		tileSize : size of a tile for display
 *      fieldIsFromImage : to know if the field as been loaded from an image. If it is set to true, the map will not be update each loop
 * \return
 * 		void
 */
void searchForEndPointGN(LabelingWeights *labelingWeights, dataType *data, Field** field, SDL_Renderer *renderer, const int tileSize, bool fieldIsFromImage)
{
    while(!data->endEvent)
	{   
		//Initiate the entity, the start and end of the route according to the field
		Entity* entity = initialiseEntity(0, 0, RADIUS_VIEWPOINT, (*field)->width, (*field)->height);
		node* startNode = nearestNode(*field, entity->x, entity->y);
		entity->x = startNode->x;
		entity->y = startNode->y;
		destructNodes(&startNode);
		node* endNode = nearestNode(*field, (*field)->width, (*field)->height);
		
		updateFieldOfViewEntity(*field, entity);
		updateMentalMapEntity(entity);
	    
		//While the entity hasn't arrived at destination
		while ((entity->x != endNode->x || entity->y != endNode->y) && !data->endEvent)
		{
			node* path = findNextPathGN(entity, endNode, data, labelingWeights);
	        moveEntityAlongPath(data, entity, path, *field, renderer, tileSize, 30);
		}
		destructNodes(&endNode);
		
		// We load a new field if we use a random map
		if(!fieldIsFromImage)
	    {
	        int fieldWidth = (*field)->width;
	        int fieldHeight = (*field)->height;
	        destructField(field);
	        *field = initialiseField(fieldWidth, fieldHeight, EMPTY);
            generateEnv(*field);
	    }
		destructEntity(&entity);
		waitForInstruction(data);
	}
	destructField(field);
}

/**
 * \fn void exploreGN(LabelingWeights *labelingWeights, dataType *data, Field** field, SDL_Renderer *renderer, const int tileSize, bool fieldIsFromImage, int maxDecisions, float percentReveal)
 * \brief show the entity, starting in the top left corner, trying to explore as much of the given field as possible by using a genetic algorithm
 * 
 * \param
 * 		labelingWeights : the genetic algorithm to use to take decisions
 * 		data : structure which define the kind of event we have to raise for interruption
 * 		field : the field where the entity will move
 * 		renderer : renderer used to draw with the SDL
 * 		tileSize : size of a tile for display
 *      fieldIsFromImage : to know if the field as been loaded from an image. If it is set to true, the map will not be update each loop
 *      maxDecisions : the maximum number of moves that the entity is allow to do before ending a loop
 *      percentReveal : the percentage of map revealed at which we concider that the entity finished his job
 * \return
 * 		void
 */
void exploreGN(LabelingWeights *labelingWeights, dataType *data, Field** field, SDL_Renderer *renderer, const int tileSize, bool fieldIsFromImage, int maxDecisions, float percentReveal)
{
    while(!data->endEvent)
	{   
		//Initiate the entity, the start and end of the route according to the field
		Entity* entity = initialiseEntity(0, 0, RADIUS_VIEWPOINT, (*field)->width, (*field)->height);
		node* startNode = nearestNode(*field, entity->x, entity->y);
		entity->x = startNode->x;
		entity->y = startNode->y;
		destructNodes(&startNode);
		
		updateFieldOfViewEntity(*field, entity);
		updateMentalMapEntity(entity);
	    
	    int nbDesisions = 0;
	    int nbMoves = 0;
	    int remainingFog = getNbFog(entity->mentalMap);
	    const int totalFog = (*field)->width*(*field)->height;
	    clock_t timeStart = clock();
		//While the entity hasn't arrived at destination
		while (nbDesisions < maxDecisions && remainingFog > (1-percentReveal)*totalFog && !data->endEvent)
		{
		    nbDesisions++;
			node* path = findNextPathGN2(entity, data, labelingWeights);
			nbMoves += getNbNode(&path);
	        moveEntityAlongPath(data, entity, path, *field, renderer, tileSize, 30);
	        remainingFog =  getNbFog(entity->mentalMap);
		}
		printf("\n\tnumber of decisions : %d\n", nbDesisions);
		printf("\tnumber of moves : %d\n", nbMoves);
		printf("\ttime spent : %f seconds\n", (clock() - timeStart)/((float)CLOCKS_PER_SEC));
		
		// We load a new field if we use a random map
		if(!fieldIsFromImage)
	    {
	        int fieldWidth = (*field)->width;
	        int fieldHeight = (*field)->height;
	        destructField(field);
	        *field = initialiseField(fieldWidth, fieldHeight, EMPTY);
            generateEnv(*field);
	    }
		destructEntity(&entity);
		waitForInstruction(data);
	}
	destructField(field);
}
