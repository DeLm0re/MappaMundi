/**
 * \file core.c
 * \brief Core of the project
 * \author Romain Jacquiez
 * \version 0.2
 * \date 09/10/2018
 *
 * Module that contain all the functions making the core of the project
 *
 */

//Header file containing the prototypes
#include "core.h"

/**
 * \fn Field initialiseField(int height, int width)
 * \brief function that initialise our field to make our environment
 *
 * \param height : height of the field, must be 1 widen than the original size because the edges is initialized with -1
 * \param width : width of the field, must be 1 widen than the original size because the edges is initialized with -1
 * \return Field : A field, which is a tydef declared in core.h (2D array)
 */
Field initialiseField(int height, int width)
{

    int i;
    int j;

    Field oneField = malloc(sizeof(int*) * width);
    for(i = 0; i < width; i++)
    {
        oneField[i] = malloc(sizeof(int) * height);
    }

    for(i = 1; i < (height-1); i++)
    {
        for(j = 1; j < (width-1); j++)
        {
            oneField[j][i] = EMPTY;
        }
    }

    for(i = 0; i < height; i++)
    {
        oneField[0][i] = WALL;
        oneField[width-1][i] = WALL;
    }
    for(i = 0; i < width; i++)
    {
        oneField[i][height-1] = WALL;
        oneField[i][0] = WALL;
    }
    return oneField;
}

/**
 * \fn void generateEnv(field oneField)
 * \brief function that generate our field to make our environment
 *
 * \param oneField : A field, which is a tydef declared in core.h (2D array)
 * \param height : height of the field, must be 1 widen than the original size because the edges is initialized with -1
 * \param width : width of the field, must be 1 widen than the original size because the edges is initialized with -1
 * \return void
 */
void generateEnv(Field oneField, int height, int width)
{

    int i;
    int j;
    srand(time(0));
    int monRand;
    int sum_neigh;

    //First loop, random generation (obstacle or not, Bernoulli)
    for(i = 1; i < (height-1); i++)
    {
        for(j = 1; j < (width-1); j++)
        {
            monRand = rand()%10 + 1;

            if(monRand < 5)
            {
                oneField[j][i] = WALL;
            }
        }
    }

    //Second loop to clean and prevent stuck situation
    for(i = 1; i < (height-1); i++)
    {
        for(j = 1; j < (width-1); j++)
        {
            sum_neigh = oneField[j-1][i-1] + oneField[j-1][i] + oneField[j][i-1] + oneField[j-1][i+1]
                        + oneField[j+1][i-1] + oneField[j+1][i] + oneField[j][i+1] + oneField[j+1][i+1];

            //Clean some obstacles
            if(sum_neigh < 2)
            {
                oneField[j][i] = EMPTY;
            }
            //Prevent stuck
            else if(sum_neigh >= 4)
            {
                oneField[j][i] = EMPTY;
            }
        }
    }

    //Third loop to fill blank
    for(i = 1; i < (height-1); i++)
    {
        for(j = 1; j < (width-1); j++)
        {
            sum_neigh = oneField[j][i-1] + oneField[j-1][i] + oneField[j][i+1] + oneField[j+1][i];

            if(sum_neigh >= 3)
            {
                oneField[j][i] = WALL;
            }
        }
    }
}

/**
 * \fn void destructField(Field oneField, int width)
 * \brief function that free the field out of memory
 *
 * \param *oneField : A pointer on a field, which is a tydef declared in core.h (2D array)
 * \param width : width of the field, must be 1 widen than the original size because the edges is initialized with -1
 * \return void
 */
void destructField(Field *oneField, int width)
{
    int i;
    if(oneField != NULL)
    {
        if(*oneField != NULL)
        {
            for(i = 0; i < width; i++)
            {
                free((*oneField)[i]);
            }
            free(*(oneField));
            *oneField = NULL;
        }
    }
}

/**
 * \fn int surface2DCircle(int radius)
 * \brief function that return the surface of a 2D circle
 *
 * \param int radius : the radius of the circle
 * \return int : the surface
 */
int surface2DCircle(int radius)
{
    int rayonCarre = radius * radius;
    float distanceCarre;
    int initialX = 0;
    int initialY = 0;
    int increment = 0;
    int x, y;

    for(x = initialX - radius; x < initialX + radius; x++)
    {
        for(y = initialY - radius; y < initialY + radius; y++)
        {
            distanceCarre = (x - initialX)*(x - initialX) + (y - initialY)*(y - initialY);
                    
            if(distanceCarre < rayonCarre)
            {
                increment++;
            }
        }
    }

    return(increment);
}
