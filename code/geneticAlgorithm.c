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
 * \fn float **create2DFloatArray(int width, int height)
 * \brief Function that creates a 2 dimension array of float
 *
 * \param width : width of the array
 * \param height : height of the array
 * 
 * \return float** : A pointer to the created array
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
 * \brief Function that initializes our field to make our environment
 *
 * \param width : width of the interest field
 * \param height : height of the interest field
 * 
 * \return InterestField : The created InterestField
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
 * \fn void destruct2DFloatArray(float **array, int width)
 * \brief Function that free the 2D array out of memory
 *
 * \param **array : The array to free
 * \param width : width of the array
 * 
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
 * \brief Function that frees an InterestField out of memory
 *
 * \param **oneField : address of the InterestField to free
 * 
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
 * \fn float labeling1(Field* fieldOfView, int xPosition, int yPosition, int xFinalPosition, int yFinalPosition)
 * \brief Function that returns the labeling of the points, will be used for labelisation
 *
 * \param Field* fieldOfView : a field of view
 * \param int xPosition : x coordinate of the entity
 * \param int yPosition : y coordinate of the entity
 * \param int xFinalPosition : x coordinate of the end point
 * \param int yFinalPosition : y coordinate of the end point
 * 
 * \return float : The labels of each point
 */
float labeling1(Field* fieldOfView, int xPosition, int yPosition, int xFinalPosition, int yFinalPosition)
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
 * \brief Function that returns the initialized labeling weights structure
 *
 * \return LabelingWeights* : The initilized weights
 */
LabelingWeights* initialiseLabelingWeights(void)
{
    LabelingWeights* labelingWeights = (LabelingWeights*)malloc(sizeof(LabelingWeights));
    int i;
    for(i = 0; i < 10; i++)
    {
        labelingWeights->weights[i] = ((float)(rand()%1000))/1000-0.5;
    }
    return labelingWeights;
}

/**
 * \fn LabelingWeights* copyLabelingWeights(LabelingWeights* labelingWeights)
 * \brief Function that returns the copy of the given labeling Weights
 * 
 * \param *labelingWeights : A pointer on the LabelingWeights to copy
 *
 * \return LabelingWeights* : The copy of the weights
 */
LabelingWeights* copyLabelingWeights(LabelingWeights* labelingWeights)
{
    LabelingWeights* returnLabelingWeights = NULL;
    if (labelingWeights != NULL)
    {
        returnLabelingWeights = initialiseLabelingWeights();
        int i;
        for(i = 0; i < 10; i++)
            returnLabelingWeights->weights[i] = labelingWeights->weights[i];
    }
    return returnLabelingWeights;
}

/**
 * \fn void destructLabelingWeights(LabelingWeights **labelingWeights)
 * \brief Function that frees the labeling weight structure out of memory
 *
 * \param **labelingWeights : address of the labelingWeights to destruct
 * 
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
 * \fn GeneticNetworks *initialiseGeneticNetworks(int size)
 * \brief Function that initializes a list of genetic networks (presented as LabelingWeights structures)
 *
 * \param size : the number of genetic networks to load in the structure
 * 
 * \return GeneticNetworks : List of genetic networks initilized
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
    geneticNetworks->time = (float*) malloc(sizeof(float) * size);
    for (i = 0; i < size; i++)
    {
        geneticNetworks->score[i] = 0;
        geneticNetworks->time[i] = 0;
    }
    return geneticNetworks;
}

/**
 * \fn GeneticNetworks *initialiseGeneticNetworksFrom(int size, LabelingWeights* labelingWeights)
 * \brief function that initializes a list of genetic networks (presented as LabelingWeights structures)
 *  based on an existing genetic network
 *
 * \param size : the number of genetic networks to load in the structure
 * \param *pathOfGeneticNetwork : a pointer to the path of a genetic network
 * \param variation : variation from the original genetic network. Must be superior to 0
 * 
 * \return GeneticNetworks : Initilized list of genetic network
 */
GeneticNetworks *initialiseGeneticNetworksFrom(int size, char* pathOfGeneticNetwork, float variation)
{
    GeneticNetworks* geneticNetworks = NULL;
    LabelingWeights* labelingWeights = loadGeneticNetwork(pathOfGeneticNetwork);
    if (labelingWeights != NULL)
    {
        geneticNetworks = (GeneticNetworks*) malloc(sizeof(GeneticNetworks));
        geneticNetworks->size = size;
        geneticNetworks->list = (LabelingWeights**) malloc(sizeof(LabelingWeights*) * size);
        int i;
        for (i = 0; i < size; i++)
        {
            geneticNetworks->list[i] = copyLabelingWeights(labelingWeights);
        }
        geneticNetworks->score = (float*) malloc(sizeof(float) * size);
        geneticNetworks->time = (float*) malloc(sizeof(float) * size);
        for (i = 0; i < size; i++)
        {
            geneticNetworks->score[i] = 0;
            geneticNetworks->time[i] = 0;
            int j;
            for(j = 0; j < 10; j++)
            {
                geneticNetworks->list[i]->weights[j] += nmap(rand()%1000, 0, 1000, -variation, variation);
            }
        }
    }
    return geneticNetworks;
}

/**
 * \fn GeneticNetworks *createNewGeneration(GeneticNetworks* geneticNetworks, int numberOfBreeder, float mutationChance)
 * \brief Function that create a new list of genetic network based on a previous one
 *
 * \param numberOfBreeder : the number of genetic networks that will be selected to reproduce among the best
 * \param mutationChance : the mutation chance of each member of the new generation
 * 
 * \return GeneticNetworks : Created GeneticNetworks
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
            for(j = 0; j < 10; j++)
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
 * \brief Function that free a list of genetic network
 *
 * \param **geneticNetworks : address of the GeneticNetworks to free
 * 
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
            free((*geneticNetworks)->score);
            free((*geneticNetworks)->time);
            free(*geneticNetworks);
            *geneticNetworks = NULL;
        }
    }
}

/**
 * \fn sortGeneticNetworks(GeneticNetworks* geneticNetworks);
 * \brief Function that sort genetic networks based on their score
 *
 * \param **geneticNetworks : address the GeneticNetworks we want to sort
 * 
 * \return void
 */
void sortGeneticNetworks(GeneticNetworks* geneticNetworks)
{
    bool sortingDone = false;
    int i = geneticNetworks->size-1;
    while(i >= 0 && !sortingDone)
    {
        sortingDone = true;
        int j;
        for(j = i; j > 0; j--)
        {
            if (geneticNetworks->score[j] < geneticNetworks->score[j - 1])
            {
                // We swap the two values
                LabelingWeights* tempWeights = geneticNetworks->list[j - 1];
                float tempScore = geneticNetworks->score[j - 1];
                float tempTime = geneticNetworks->time[j - 1];
                geneticNetworks->list[j - 1] = geneticNetworks->list[j];
                geneticNetworks->score[j - 1] = geneticNetworks->score[j];
                geneticNetworks->time[j - 1] = geneticNetworks->time[j];
                geneticNetworks->list[j] = tempWeights;
                geneticNetworks->score[j] = tempScore;
                geneticNetworks->time[j] = tempTime;
                
                sortingDone = false;
            }
        }
        i--;
    }
}

/**
 * \fn void saveGeneticNetwork(LabelingWeights* labelingWeights, char* path)
 * \brief Function that save a genetic network 
 *
 * \param *LabelingWeights : A pointer on the LabelingWeights (genetic network) we want to save
 * \param *path : The path to the save file
 * 
 * \return void
 */
bool saveGeneticNetwork(LabelingWeights* labelingWeights, char* path)
{
    FILE* file; // use to store the file where the game will be save
	// If we managed to open the file
	if((file = fopen(path, "wb+")))
	{
		// We write down the weights
		fwrite(labelingWeights->weights, sizeof(double), 10, file);
		
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
 * \brief Function that loads a genetic network 
 *
 * \param *path : the path to the file
 * 
 * \return LabelingWeights* : The loaded weights
 */
LabelingWeights* loadGeneticNetwork(char* path)
{
	FILE* file;
	if((file = fopen(path, "rb")))
	{
        LabelingWeights* labelingWeights = initialiseLabelingWeights();
		int statut = 0;

        // We read the waights from the file
        statut = fread(labelingWeights->weights, sizeof(double), 10, file);
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
