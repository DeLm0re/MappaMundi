/**
 * \file geneticAlgorithm.c
 * \brief Functions of the genetic Algorithm
 * \author Antoine Poirier-Rouillard
 * \version 0.1
 * \date 04/04/2019
 *
 * Module that contains all functions of genetic algorithm
 *
 */

#include "geneticAlgorithm.h"

/**
 * \fn int **create2DIntArray(int width, int height)
 * \brief function that creates a 2 dimension array of int
 *
 * \param width : width of the array
 * \param height : height of the array
 * \return int** : A pointer to the created array
 */
float **create2DFloatArray(int width, int height)
{
    float **array = (float**)malloc(sizeof(float*) * width);
    
    int widthIndex;
    for(widthIndex = 0; widthIndex < width; widthIndex++)
    {
        array[widthIndex] = (float*)malloc(sizeof(float) * height);
    }

    return array;
}

/**
 * \fn Field initialiseInterestField(int width, int height)
 * \brief function that initialise our field to make our environment
 *
 * \param width : width of the interest field
 * \param height : height of the interest field
 * \return Field : Pointer to an InterestField
 */
InterestField *initialiseInterestField(int width, int height)
{
    InterestField *oneField = (InterestField*)malloc(sizeof(InterestField));

    oneField->width = width;
    oneField->height = height;
    oneField->data = create2DFloatArray(width, height);

    int widthIndex, heightIndex;

    for(widthIndex = 0; widthIndex < width; widthIndex++)
    {
        for(heightIndex = 0; heightIndex < height; heightIndex++)
        {
            oneField->data[widthIndex][heightIndex] = 0;
        }
    }

    return oneField;
}

/**
 * \fn void destruct2DFloatArray(int **array, int width)
 * \brief function that free the 2D array out of memory
 *
 * \param **array : The array to free
 * \param width : width of the array
 * \return void
 */
void destruct2DFloatArray(float **array, int width)
{
    int i;
    if(array != NULL)
    {
        for(i = 0; i < width; i++)
        {
            free(array[i]);
        }
        free(array);
    }
}

/**
 * \fn void destructInterestField(InterestField **oneField)
 * \brief function that free the field out of memory
 *
 * \param **oneField : A double pointer on an InterestField
 * \return void
 */
void destructInterestField(InterestField **oneField)
{
    if(oneField != NULL)
    {
        if(*oneField != NULL)
        {
            destruct2DFloatArray((*oneField)->data, (*oneField)->width);
            (*oneField)->data = NULL;
            free(*oneField);
            *oneField = NULL;
        }
    }
}

/**
 * \fn Field* labeling(Field* fieldOfView, int xPosition, int yPosition, int xFinalPosition, int yFinalPosition)
 * \brief function that returns the labeling of the points
 * will be used for labelisation
 *
 * \param Field* fieldOfView : a field of view
 * \param int xPosition : x coordinate of the entity
 * \param int yPosition : y coordinate of the entity
 * \param int xFinalPosition : x coordinate of the end point
 * \param int yFinalPosition : y coordinate of the end point
 * \return float
 */
float labeling(Field* fieldOfView, int xPosition, int yPosition, int xFinalPosition, int yFinalPosition)
{
    int emptyPoint = 0;
    int wallPoint = 0;
    int fogPoint = 0;
    int visitedPoint = 0;
    int distPoint = 0;
    float value = 0;
    float finalValue = 0;

    if (fieldOfView->data[(fieldOfView->height-1)/2][(fieldOfView->width-1)/2] != EMPTY) 
    {
        return 0;
    }
    
    for(int width = 0; width < fieldOfView->width; width++)
    {
        for(int height = 0; height < fieldOfView->height; height++)
        {
            switch (fieldOfView->data[width][height])
            {
                case EMPTY:
                    emptyPoint++;
                    break;
                case WALL:
                    wallPoint++;
                    break;
                case FOG:
                    fogPoint++;
                    break;
                case VISITED:
                    visitedPoint++;
                    break;
            }
        }
    }

    distPoint = (xFinalPosition-xPosition)*(xFinalPosition-xPosition) + (yFinalPosition-yPosition)*(yFinalPosition-yPosition);

    value = fogPoint/(distPoint*0.1);

    finalValue = (1.0/(1+exp(-value)));

    return finalValue;
}

/**
 * \fn Field* labeling2(Field* fieldOfView, int xPosition, int yPosition, int xFinalPosition, int yFinalPosition)
 * \brief function that returns the labeling of the points
 * will be used for labelisation
 *
 * \param Field* fieldOfView : a field of view
 * \param int xPosition : x coordinate of the entity
 * \param int yPosition : y coordinate of the entity
 * \param int xFinalPosition : x coordinate of the end point
 * \param int yFinalPosition : y coordinate of the end point
 * \return float
 */
float labeling2(Field* fieldOfView, int xPosition, int yPosition, int xFinalPosition, int yFinalPosition)
{
    int emptyPoint = 0;
    int wallPoint = 0;
    int fogPoint = 0;
    int visitedPoint = 0;
    int distPoint = 0;
    float value = 0;
    float finalValue = 0;

    if (fieldOfView->data[(fieldOfView->height-1)/2][(fieldOfView->width-1)/2] != EMPTY) 
    {
        return 0;
    }
    
    for(int width = 0; width < fieldOfView->width; width++)
    {
        for(int height = 0; height < fieldOfView->height; height++)
        {
            switch (fieldOfView->data[width][height])
            {
                case EMPTY:
                    emptyPoint++;
                    break;
                case WALL:
                    wallPoint++;
                    break;
                case FOG:
                    if(isVisibleFrom(fieldOfView, (fieldOfView->height-1)/2, (fieldOfView->width-1)/2, width, height))
                    {
                        fogPoint++;
                    }
                    break;
                case VISITED:
                    visitedPoint++;
                    break;
            }
        }
    }

    distPoint = (xFinalPosition-xPosition)*(xFinalPosition-xPosition) + (yFinalPosition-yPosition)*(yFinalPosition-yPosition);

    value = fogPoint/(distPoint*0.1);

    finalValue = (1.0/(1+exp(-value)));

    return finalValue;
}

/**
 * \fn LabelingWeights* initialiseLabelingWeights()
 * \brief function that returns the initialized labeling weights structure
 *
 * \return LabelingWeights*
 */
LabelingWeights* initialiseLabelingWeights(void)
{
    LabelingWeights* labelingWeights = (LabelingWeights*)malloc(sizeof(LabelingWeights));
    int i;
    for(i = 0; i < 9; i++)
    {
        labelingWeights->weights[i] = ((float)(rand()%1000))/1000-0.5;
    }
    return labelingWeights;
}

/**
 * \fn void destructLabelingWeights(LabelingWeights **labelingWeights)
 * \brief function that free the labeling weight structure out of memory
 *
 * \param **labelingWeights : A double pointer on a LabelingWeights
 * \return void
 */
void destructLabelingWeights(LabelingWeights **labelingWeights)
{
    if(labelingWeights != NULL)
    {
        if(*labelingWeights != NULL)
        {
            free(*labelingWeights);
            *labelingWeights = NULL;
        }
    }
}

/**
 * \fn Field* labeling3(Field* fieldOfView, int xPosition, int yPosition, int xFinalPosition, int yFinalPosition, LabelingWeights* labelingWeights)
 * \brief function that returns the labeling of the points
 * will be used for labelisation
 *
 * \param Field* fieldOfView : a field of view
 * \param int xPosition : x coordinate of the entity
 * \param int yPosition : y coordinate of the entity
 * \param int xFinalPosition : x coordinate of the end point
 * \param int yFinalPosition : y coordinate of the end point
 * \param LabelingWeights* labelingWeights : Ze labeling weights
 * \return float
 */
float labeling3(Field* fieldOfView, int xPosition, int yPosition, int xFinalPosition, int yFinalPosition, LabelingWeights* labelingWeights)
{
    float emptyPoint = 0;
    float wallPoint = 0;
    float fogPoint = 0;
    float visitedPoint = 0;
    float dist = 0;
    float avgDistEmpty = 0;
    float avgDistWall = 0;
    float avgDistFog = 0;
    float avgDistVisited = 0;
    float value = 0;
    float centerPointx = (fieldOfView->width-1)/2;
    float centerPointy = (fieldOfView->height-1)/2;


    if (fieldOfView->data[(int)centerPointx][(int)centerPointy] != EMPTY) 
    {
        return -INFINITY;
    }
    
    for(int width = 0; width < fieldOfView->width; width++)
    {
        for(int height = 0; height < fieldOfView->height; height++)
        {
            switch (fieldOfView->data[width][height])
            {
                case EMPTY:
                    avgDistEmpty += sqrt(pow(width-centerPointx,2) + pow(height-centerPointy,2));
                    emptyPoint++;
                    break;
                case WALL:
                    avgDistWall += sqrt(pow(width-centerPointx,2) + pow(height-centerPointy,2));
                    wallPoint++;
                    break;
                case FOG:
                    if(isVisibleFrom(fieldOfView, (fieldOfView->height-1)/2, (fieldOfView->width-1)/2, width, height))
                    {
                        avgDistFog += sqrt(pow(width-centerPointx,2) + pow(height-centerPointy,2));
                        fogPoint++;
                    }
                    break;
                case VISITED:
                    avgDistVisited += sqrt(pow(width-centerPointx,2) + pow(height-centerPointy,2));
                    visitedPoint++;
                    break;
            }
        }
    }

    dist = sqrt(pow(xFinalPosition-xPosition,2) + pow(yFinalPosition-yPosition,2));

    avgDistEmpty /= emptyPoint;
    avgDistWall /= wallPoint;
    avgDistFog /= fogPoint;
    avgDistVisited /= visitedPoint;

    value = dist*labelingWeights->weights[DIST]+
            emptyPoint*labelingWeights->weights[NB_EMPTY]+
            wallPoint*labelingWeights->weights[NB_WALL]+
            fogPoint*labelingWeights->weights[NB_FOG]+
            visitedPoint*labelingWeights->weights[NB_VISITED]+
            avgDistEmpty*labelingWeights->weights[AVG_DIST_EMPTY]+
            avgDistWall*labelingWeights->weights[AVG_DIST_WALL]+
            avgDistFog*labelingWeights->weights[AVG_DIST_FOG]+
            avgDistVisited*labelingWeights->weights[AVG_DIST_VISITED];

    return value;
}

/**
 * \fn GeneticNetworks *initialiseInterestField(int size)
 * \brief function that initialise a list of genetic networks (presented as LabelingWeights structures)
 *
 * \param size : the number of genetic networks to load in the structure
 * \return GeneticNetworks : Pointer to a GeneticNetworks
 */
GeneticNetworks *initialiseGeneticNetworks(int size)
{
    GeneticNetworks* geneticNetworks = (GeneticNetworks*) malloc(sizeof(GeneticNetworks));
    geneticNetworks->size = size;
    geneticNetworks->list = (LabelingWeights**) malloc(sizeof(LabelingWeights*) * size);
    int i;
    for (i = 0; i < size; i++)
    {
        geneticNetworks->list[i] = initialiseLabelingWeights();
    }
    geneticNetworks->score = (float*) malloc(sizeof(float) * size);
    for (i = 0; i < size; i++)
    {
        geneticNetworks->score[i] = 0;
    }
    return geneticNetworks;
}

/**
 * \fn GeneticNetworks *initialiseGeneticNetworksFrom_(int size, LabelingWeights* labelingWeights)
 * \brief function that initialise a list of genetic networks (presented as LabelingWeights structures)
 *  based on an existing genetic network
 *
 * \param size : the number of genetic networks to load in the structure
 * \param *pathOfGeneticNetwork : a pointer to the path of a genetic network
 * \param variation : variation from the original genetic network. Must be superior to 0
 * \return GeneticNetworks : Pointer to a GeneticNetworks
 */
GeneticNetworks *initialiseGeneticNetworksFrom_(int size, char* pathOfGeneticNetwork, float variation)
{
    GeneticNetworks* geneticNetworks = (GeneticNetworks*) malloc(sizeof(GeneticNetworks));
    geneticNetworks->size = size;
    geneticNetworks->list = (LabelingWeights**) malloc(sizeof(LabelingWeights*) * size);
    int i;
    for (i = 0; i < size; i++)
    {
        geneticNetworks->list[i] = loadGeneticNetwork(pathOfGeneticNetwork);
    }
    geneticNetworks->score = (float*) malloc(sizeof(float) * size);
    for (i = 0; i < size; i++)
    {
        geneticNetworks->score[i] = 0;
        int j;
        for(j = 0; j < 9; j++)
        {
            geneticNetworks->list[i]->weights[j] += nmap(rand()%1000, 0, 1000, -variation, variation);
        }
    }
    return geneticNetworks;
}

/**
 * \fn GeneticNetworks *createNewGeneration(GeneticNetworks* geneticNetworks, int numberOfBreeder)
 * \brief function that create a new list of genetic network based on a previous one
 *
 * \param numberOfBreeder : the number of genetic networks that will be selected to reproduce among the best
 * \param mutationChance : the mutation chance of each member of the new generation
 * \return GeneticNetworks : Pointer to a GeneticNetworks
 */
GeneticNetworks *createNewGeneration(GeneticNetworks* geneticNetworks, int numberOfBreeder, float mutationChance)
{
    GeneticNetworks* newGeneticNetworks = NULL;
    if (numberOfBreeder <= geneticNetworks->size)
    {
        newGeneticNetworks = initialiseGeneticNetworks(geneticNetworks->size);
        sortGeneticNetworks(geneticNetworks);
        int i;
        for(i = 0; i < newGeneticNetworks->size; i++)
        {
            int indexParent1 = rand()%numberOfBreeder;
            int indexParent2 = rand()%numberOfBreeder;
            if (indexParent2 >= indexParent1)
            {
                indexParent2++;
                indexParent2 %= numberOfBreeder;
            }
            
            int j;
            for(j = 0; j < 9; j++)
            {
                float randomMutation;
                if (((float) (rand()%1000)) / 1000 < mutationChance)
                {
                    randomMutation = ((float) (rand()%1000)) / 1000 - 0.5;
                }
                else
                {
                    randomMutation = 0;
                }
                newGeneticNetworks->list[i]->weights[j] = 
                    (geneticNetworks->list[indexParent1]->weights[j] + geneticNetworks->list[indexParent2]->weights[j])/2 + randomMutation;
               }
        }
    }
    return newGeneticNetworks;
}

/**
 * \fn void destructGeneticNetworks(GeneticNetworks **geneticNetworks)
 * \brief function that free a list of genetic network
 *
 * \param **geneticNetworks : A double pointer on a GeneticNetworks
 * \return void
 */
void destructGeneticNetworks(GeneticNetworks **geneticNetworks)
{
    if(geneticNetworks != NULL)
    {
        if(*geneticNetworks != NULL)
        {
            int i;
            for (i = 0; i < (*geneticNetworks)->size; i++)
            {
                destructLabelingWeights(&(*geneticNetworks)->list[i]);
            }
            free((*geneticNetworks)->list);
            free(*geneticNetworks);
            *geneticNetworks = NULL;
        }
    }
}

/**
 * \fn sortGeneticNetworks(GeneticNetworks* geneticNetworks);
 * \brief function that sort genetic networks based on their score
 *
 * \param **geneticNetworks : A double pointer on the GeneticNetworks we want to sort
 * \return void
 */
void sortGeneticNetworks(GeneticNetworks* geneticNetworks)
{
    bool sortingDone = false;
    int i = 0;
    while(i < geneticNetworks->size && !sortingDone)
    {
        sortingDone = true;
        int j;
        for(j = i; j < geneticNetworks->size-1; j++)
        {
            if (geneticNetworks->score[j + 1] > geneticNetworks->score[j])
            {
                // We swap the two values
                LabelingWeights* tempWeights = geneticNetworks->list[j + 1];
                float tempScore = geneticNetworks->score[j + 1];
                geneticNetworks->list[j + 1] = geneticNetworks->list[j];
                geneticNetworks->score[j + 1] = geneticNetworks->score[j];
                geneticNetworks->list[j] = tempWeights;
                geneticNetworks->score[j] = tempScore;
                
                sortingDone = false;
            }
        }
        i++;
    }
}

/**
 * \fn void saveGeneticNetwork(LabelingWeights* labelingWeights, char* path)
 * \brief function that save a genetic network 
 *
 * \param *LabelingWeights : A pointer on the LabelingWeights (genetic network) we want to save
 * \param *path : The path to the save file
 * \return void
 */
bool saveGeneticNetwork(LabelingWeights* labelingWeights, char* path)
{
    FILE* file; // use to store the file where the game will be save
	// If we managed to open the file
	if((file = fopen(path, "wb+")))
	{
		// We write down the weights
		fwrite(labelingWeights->weights, sizeof(float), 9, file);
		
		// We close the file
		fclose(file);
		// We return true to indicate that everything worked correctly
		return true;
	}
	else
	{
		// We return false because the file has not been read correctly
		return false;
	}
}

/**
 * \fn LabelingWeights* loadGeneticNetwork(char* path);
 * \brief function that load a genetic network 
 *
 * \param *path : The path to the file
 * \return LabelingWeights*
 */
LabelingWeights* loadGeneticNetwork(char* path)
{
	FILE* file;
	if((file = fopen(path, "rb")))
	{
        LabelingWeights* labelingWeights = initialiseLabelingWeights();
		int statut = 0;

        // We read the waights from the file
        statut = fread(labelingWeights->weights, sizeof(float), 9, file);
        if (statut == 0) 
        {
            return NULL;
        }
		
		// We close the file
		fclose(file);
		// We return the neural network
		return labelingWeights;
	}
	else
	{
		return NULL;
	}
}
