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
 * \param height : height of the field 
 * \param width : width of the field
 * \param defaultValue : value used to set each point of the Field
 * \return Field : A field, which is a tydef declared in core.h (2D array)
 */
Field *initialiseField(int height, int width, pointEnum defaultValue)
{

    int i;
    int j;

    Field *oneField = malloc(sizeof(Field));
    oneField->height = height;
    oneField->length = width;

    oneField->values = malloc(sizeof(int*) * width);
    
    for(i = 0; i < width; i++)
    {
        oneField->values[i] = malloc(sizeof(int) * height);
    }

    for(i = 1; i < (height-1); i++)
    {
        for(j = 1; j < (width-1); j++)
        {
            oneField->values[i][j] = defaultValue;
        }
    }

    for(i = 0; i < height; i++)
    {
        oneField->values[i][0] = WALL;
        oneField->values[0][i] = WALL;
        oneField->values[i][width-1] = WALL;
        oneField->values[height-1][i] = WALL;
    }
    return oneField;
}

/**
 * \fn void generateEnv(field oneField)
 * \brief function that generate our field to make our environment
 *
 * \param oneField : A field, which is a tydef declared in core.h (2D array)
 * \return void
 */
void generateEnv(Field *oneField)
{

    int i;
    int j;
    srand(time(0));
    int monRand;
    int sum_neigh;

    //First loop, random generation (obstacle or not, Bernoulli)
    for(i = 1; i < (oneField->height-1); i++)
    {
        for(j = 1; j < (oneField->length-1); j++)
        {
            monRand = rand()%10 + 1;

            if(monRand < 5)
            {
                oneField->values[i][j] = WALL;
            }
        }
    }

    //Second loop to clean and prevent stuck situation
    for(i = 1; i < (oneField->height-1); i++)
    {
        for(j = 1; j < (oneField->length-1); j++)
        {
            sum_neigh = oneField->values[i-1][j-1] + oneField->values[i-1][j] + oneField->values[i][j-1] + oneField->values[i-1][j+1]
                        + oneField->values[i+1][j-1] + oneField->values[i+1][j] + oneField->values[i][j+1] + oneField->values[i+1][j+1];

            //Clean some obstacles
            if(sum_neigh < 2)
            {
                oneField->values[i][j] = EMPTY;
            }
            //Prevent stuck
            else if(sum_neigh >= 4)
            {
                oneField->values[i][j] = EMPTY;
            }
        }
    }

    //Third loop to fill blank
    for(i = 1; i < (oneField->height-1); i++)
    {
        for(j = 1; j < (oneField->length-1); j++)
        {
            sum_neigh = oneField->values[i-1][j] + oneField->values[i][j-1] + oneField->values[i+1][j] + oneField->values[i][j+1];

            if(sum_neigh >= 3)
            {
                oneField->values[i][j] = WALL;
            }
        }
    }
}

/**
 * \fn void destructField(Field oneField, int height)
 * \brief function that free the field out of memory
 *
 * \param oneField : A field, which is a tydef declared in core.h (2D array)
 * \return void
 */
void destructField(Field **oneField)
{
    if(oneField != NULL)
    {
        if(*oneField != NULL)
        {
            int i;
            for(i = 0; i < (*oneField)->length; i++)
            {
                free((*oneField)->values[i]);
            }
            free((*oneField)->values);
            free(*oneField);
            oneField = NULL;
        }
    }
}
