/**
 * \file core.c
 * \brief Core of the project
 * \author Romain Jacquiez
 * \version 0.1
 * \date 09/10/2018
 *
 * Module that contain all the functions making the core of the project
 *
 */

//Header files
#include <time.h>
#include <math.h>

//Header file containing the prototypes
#include "core.h"

/**
 * \fn void initialiseField(field oneField)
 * \brief function that initialise our field to make our environment
 *
 * \param oneField : A field, which is a tydef declared in core.h (2D array)
 * \return void
 */
void initialiseField(field oneField){

    int i;
    int j;

    for(i = 0; i < 52; i++)
    {
        for(j = 0; j < 52; j++)
        {
            oneField[i][j] = 0;
        }
    }

    for(i = 0; i < 52; i++)
    {
        oneField[i][0] = 1;
        oneField[0][i] = 1;
        oneField[i][51] = 1;
        oneField[51][i] = 1;
    }
}

/**
 * \fn void generateEnv(field oneField)
 * \brief function that generate our field to make our environment
 *
 * \param oneField : A field, which is a tydef declared in core.h (2D array)
 * \return void
 */
void generateEnv(field oneField) {

    int i;
    int j;
    srand(time(NULL));
    int monRand;
    int sum_neigh;

    //First loop, random generation (obstacle or not, Bernoulli)
    for(i = 1; i < 51; i++)
    {
        for(j = 1; j < 51; j++)
        {
            monRand = rand()%10 + 1;

            if(monRand < 5)
            {
                oneField[i][j] = 1;
            }
        }
    }

    //Second loop to clean and prevent stuck situation
    for(i = 1; i < 51; i++)
    {
        for(j = 1; j < 51; j++)
        {
            sum_neigh = oneField[i-1][j-1] + oneField[i-1][j] + oneField[i][j-1] + oneField[i-1][j+1]
                        + oneField[i+1][j-1] + oneField[i+1][j] + oneField[i][j+1] + oneField[i+1][j+1];

            //Clean some obstacles
            if(sum_neigh < 2)
            {
                oneField[i][j] = 0;
            }
            //Prevent stuck
            else if(sum_neigh >= 4)
            {
                oneField[i][j] = 0;
            }
        }
    }

    //Third loop to fill blank
    for(i = 1; i < 51; i++)
    {
        for(j = 1; j < 51; j++)
        {
            sum_neigh = oneField[i-1][j] + oneField[i][j-1] + oneField[i+1][j] + oneField[i][j+1];

            if(sum_neigh >= 3)
            {
                oneField[i][j] = 1;
            }
        }
    }
}