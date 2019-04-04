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

    // random between -0.5 & 0.5
    labelingWeights->distance=((float)(rand()%1000))/1000-0.5;
    labelingWeights->nbEmpty=((float)(rand()%1000))/1000-0.5;
    labelingWeights->nbWall=((float)(rand()%1000))/1000-0.5;
    labelingWeights->nbFog=((float)(rand()%1000))/1000-0.5;
    labelingWeights->nbVisited=((float)(rand()%1000))/1000-0.5;
    labelingWeights->avgDistEmpty=((float)(rand()%1000))/1000-0.5;
    labelingWeights->avgDistWall=((float)(rand()%1000))/1000-0.5;
    labelingWeights->avgDistFog=((float)(rand()%1000))/1000-0.5;
    labelingWeights->avgDistVisited=((float)(rand()%1000))/1000-0.5;

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
    float finalValue = 0;
    float centerPointx = (fieldOfView->width-1)/2;
    float centerPointy = (fieldOfView->height-1)/2;


    if (fieldOfView->data[(int)centerPointx][(int)centerPointy] != EMPTY) 
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

    value = dist*labelingWeights->distance+
            emptyPoint*labelingWeights->nbEmpty+
            wallPoint*labelingWeights->nbWall+
            fogPoint*labelingWeights->nbFog+
            visitedPoint*labelingWeights->nbVisited+
            avgDistEmpty*labelingWeights->avgDistEmpty+
            avgDistWall*labelingWeights->avgDistWall+
            avgDistFog*labelingWeights->avgDistFog+
            avgDistVisited*labelingWeights->avgDistVisited;

    finalValue = (1.0/(1+exp(-value)));

    return finalValue;
}