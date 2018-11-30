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
 * \fn int** initialiseField(int height, int width)
 * \brief function that initialise our field to make our environment
 *
 * \param oneField : A field, which is a tydef declared in core.h (2D array)
 * \param height : height of the field, must be 1 widen than the original size because the edges is initialized with -1
 * \param width : width of the field, must be 1 widen than the original size because the edges is initialized with -1
 * \return int**
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
            oneField[i][j] = EMPTY;
        }
    }

    for(i = 0; i < height; i++)
    {
        oneField[i][0] = WALL;
        oneField[0][i] = WALL;
        oneField[i][width-1] = WALL;
        oneField[height-1][i] = WALL;
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
                oneField[i][j] = WALL;
            }
        }
    }

    //Second loop to clean and prevent stuck situation
    for(i = 1; i < (height-1); i++)
    {
        for(j = 1; j < (width-1); j++)
        {
            sum_neigh = oneField[i-1][j-1] + oneField[i-1][j] + oneField[i][j-1] + oneField[i-1][j+1]
                        + oneField[i+1][j-1] + oneField[i+1][j] + oneField[i][j+1] + oneField[i+1][j+1];

            //Clean some obstacles
            if(sum_neigh < 2)
            {
                oneField[i][j] = EMPTY;
            }
            //Prevent stuck
            else if(sum_neigh >= 4)
            {
                oneField[i][j] = EMPTY;
            }
        }
    }

    //Third loop to fill blank
    for(i = 1; i < (height-1); i++)
    {
        for(j = 1; j < (width-1); j++)
        {
            sum_neigh = oneField[i-1][j] + oneField[i][j-1] + oneField[i+1][j] + oneField[i][j+1];

            if(sum_neigh >= 3)
            {
                oneField[i][j] = WALL;
            }
        }
    }
}

/**
 * \fn void destructField(Field oneField, int height)
 * \brief function that free the field out of memory
 *
 * \param oneField : A field, which is a tydef declared in core.h (2D array)
 * \param height : height of the field, must be 1 widen than the original size because the edges is initialized with -1
 * \return void
 */
void destructField(Field oneField, int height)
{
    int i;
    for(i = 0; i < height; i++)
    {
        free(oneField[i]);
    }
    free(oneField);
}
