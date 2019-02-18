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
 * \fn int **create2DIntArray(int height, int width)
 * \brief function that creates a 2 dimension array of int
 *
 * \param width : width of the array
 * \param height : height of the array
 * \return int** : A pointer to the created array
 */
int **create2DIntArray(int width, int height)
{
    int **array = malloc(sizeof(int*) * width);
    
    int widthIndex;
    for(widthIndex = 0; widthIndex < width; widthIndex++)
    {
        array[widthIndex] = malloc(sizeof(int) * height);
    }

    return array;
}


/**
 * \fn Field initialiseField(int width, int height)
 * \brief function that initialise our field to make our environment
 *
 * \param width : width of the field, must be 1 widen than the original size because the edges is initialized with -1
 * \param height : height of the field, must be 1 widen than the original size because the edges is initialized with -1
 * \return Field : A field, which is a tydef declared in core.h (2D array struct)
 */
Field initialiseField(int width, int height)
{

    Field oneField;

    oneField.width = width;
    oneField.height = height;
    oneField.data = create2DIntArray(width, height);

    int widthIndex, heightIndex;

    for(widthIndex = 1; widthIndex < (widthIndex - 1); widthIndex++)
    {
        for(heightIndex = 1; heightIndex < (height - 1); heightIndex++)
        {
            oneField.data[widthIndex][heightIndex] = EMPTY;
        }
    }

    for(heightIndex = 0; heightIndex < height; heightIndex++)
    {
        oneField.data[0][heightIndex] = WALL;
        oneField.data[width-1][heightIndex] = WALL;
    }

    for(widthIndex = 0; widthIndex < width; widthIndex++)
    {
        oneField.data[widthIndex][height-1] = WALL;
        oneField.data[widthIndex][0] = WALL;
    }

    return oneField;
}

/**
 * \fn void generateEnv(field oneField)
 * \brief function that generate our field to make our environment
 *
 * \param oneField : A field, which is a tydef declared in core.h (2D array structure)
 * \return void
 */
void generateEnv(Field oneField)
{

    int w; //i
    int h; //j
    int monRand;
    int sum_neigh;

    //First loop, random generation (obstacle or not, Bernoulli)
    for(w = 1; w < (oneField.width-1); w++)
    {
        for(h = 1; h < (oneField.height-1); h++)
        {
            monRand = rand()%10 + 1;

            if(monRand < 5)
            {
                oneField.data[w][h] = WALL;
            }
        }
    }

    //Second loop to clean and prevent stuck situation
    for(w = 1; w < (oneField.width-1); w++)
    {
        for(h = 1; h < (oneField.height-1); h++)
        {
            sum_neigh = oneField.data[w-1][h-1] + oneField.data[w-1][h] + oneField.data[w][h-1] + oneField.data[w-1][h+1]
                        + oneField.data[w+1][h-1] + oneField.data[w+1][h] + oneField.data[w][h+1] + oneField.data[w+1][h+1];

            //Clean some obstacles
            if(sum_neigh < 2)
            {
                oneField.data[w][h] = EMPTY;
            }
            //Prevent stuck
            else if(sum_neigh >= 4)
            {
                oneField.data[w][h] = EMPTY;
            }
        }
    }

    //Third loop to fill blank
    for(w = 1; w < (oneField.width-1); w++)
    {
        for(h = 1; h < (oneField.height-1); h++)
        {
            sum_neigh = oneField.data[w][h-1] + oneField.data[w-1][h] + oneField.data[w][h+1] + oneField.data[w+1][h];

            if(sum_neigh >= 3)
            {
                oneField.data[w][h] = WALL;
            }
        }
    }
}

/**
 * \fn void destruct2DIntArray(int **array, int width)
 * \brief function that free the 2D array out of memory
 *
 * \param **array : The array to free
 * \param width : width of the array
 * \return void
 */
void destruct2DIntArray(int **array, int width)
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
 * \fn void destructField(Field oneField)
 * \brief function that free the field out of memory
 *
 * \param *oneField : A pointer on a field, which is a tydef declared in core.h (2D array structure)
 * \return void
 */
void destructField(Field *oneField)
{
    if(oneField != NULL)
    {
        destruct2DIntArray(oneField->data, oneField->width);
        free(oneField);
        oneField = NULL;
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
