/**
 * \file core.h
 * \brief Prototypes of the core project
 * \author Romain Jacquiez
 * \version 0.2
 * \date 09/10/2018
 *
 * Module that contain all prototypes of function in core.c
 *
 */

#ifndef H_CORE
    #define H_CORE

//Header file for graphical display
#include "prototype.h"

/**
 * \fn int **create2DIntArray(int width, int height)
 * \brief function that creates a 2 dimension array of int
 *
 * \param width : width of the array
 * \param height : height of the array
 * \return int** : A pointer to the created array
 */
int **create2DIntArray(int width, int height);

/**
 * \fn Field initialiseField(int width, int height)
 * \brief function that initialise our field to make our environment
 *
 * \param width : width of the field, must be 1 widen than the original size because the edges is initialized with -1
 * \param height : height of the field, must be 1 widen than the original size because the edges is initialized with -1
 * \param defaultValue : the value we initialize each tile of the field with
 * \return Field : Pointer to a Field, which is a tydef declared in core.h (2D array struct)
 */
Field *initialiseField(int width, int height, pointEnum defaultValue);

/**
 * \fn void generateEnv(field oneField)
 * \brief function that generate our field to make our environment
 *
 * \param oneField : Poiter to a field, which is a tydef declared in core.h (2D array structure)
 * \return void
 */
void generateEnv(Field *oneField);

/**
 * \fn void destruct2DIntArray(int **array, int width)
 * \brief function that free the 2D array out of memory
 *
 * \param **array : The array to free
 * \param width : width of the array
 * \return void
 */
void destruct2DIntArray(int **array, int width);

/**
 * \fn void destructField(Field oneField)
 * \brief function that free the field out of memory
 *
 * \param **oneField : A double pointer on a field, which is a tydef declared in core.h (2D array structure)
 * \return void
 */
void destructField(Field **oneField);

/**
 * \fn int surface2DCircle(int radius)
 * \brief function that return the number of 1 by 1 square in a 2D circle
 *
 * \param int radius : the radius of the circle
 * \return int : the surface
 */
int surface2DCircle(int radius);

#endif
