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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "BmpLib.h"
#include "prototype.h"

/**
 * \fn int **create2DIntArray(int width, int height)
 * \brief Function that creates a 2 dimension array of int
 *
 * \param width : width of the array
 * \param height : height of the array
 * \return int** : A pointer to the created array
 */
int **create2DIntArray(int width, int height);

/**
 * \fn Field initialiseField(int width, int height, pointEnum defaultValue)
 * \brief Function that initialise a field with a specified value
 *
 * \param width : width of the field
 * \param height : height of the field
 * \param defaultValue : the value we initialize each tile of the field with
 * \return Field* : Pointer to a Field
 */
Field *initialiseField(int width, int height, pointEnum defaultValue);

/**
 * \fn Field *createCustomField(char *pathImageField)
 * \brief Function that creates a field using a custom image
 *
 * \param char *pathImageField : the path for the bmp image of the custom field
 * 
 * \return Field* : Pointer to the created Field
 */
Field *createCustomField(char *pathImageField);

/**
 * \fn void generateEnv(Field *oneField)
 * \brief Function that generates a random Field
 *
 * \param oneField : Field to randomize
 * \return void
 */
void generateEnv(Field *oneField);

/**
 * \fn void destruct2DIntArray(int **array, int width)
 * \brief Function that frees a 2D array out of memory
 *
 * \param **array : the array to free
 * \param width : width of the array
 * \return void
 */
void destruct2DIntArray(int **array, int width);

/**
 * \fn void destructField(Field **oneField)
 * \brief Function that frees a field out of memory
 *
 * \param **oneField : address of the Field pointer to free
 * \return void
 */
void destructField(Field **oneField);

/**
 * \fn int surface2DCircle(int radius)
 * \brief Function that computes the number of 1 by 1 squares in a 2D circle
 *
 * \param int radius : the radius of the circle
 * \return int : The surface of the circle
 */
int surface2DCircle(int radius);

/**
 * \fn Field* generateRandomFieldOfView(int visionRange, bool isValid)
 * \brief Function that returns a random field of view that will be used for labelization
 *
 * \param int visionRange : the vision range, or size of the field of view
 * \param bool isValid : if the random field of view is a valid position to move or not
 * \return Field* : A random field of view
 */
Field* generateRandomFieldOfView(int visionRange, bool isValid);

/**
 * \fn bool isVisibleFrom(Field* fieldOfView, int xOrigin, int yOrigin, int xPosition, int yPosition)
 * \brief Function that computes if a given position is visible from actual position in a field of view
 *
 * \param Field* fieldOfView : a field of view
 * \param int xOrigin : x coordinate of actual position
 * \param int yOrigin : y coordinate of actual position
 * \param int xPosition : x coordinate we want to check
 * \param int yPosition : y coordinate we want to check
 * \return bool : True if the gieven position is visible, false otherwise
 */
bool isVisibleFrom(Field* fieldOfView, int xOrigin, int yOrigin, int xPosition, int yPosition);

/**
 * \fn int getNbFog(Field* field)
 * \brief Function that computes the number of fog tile in a given field
 *
 * \param Field* field : the field where we will count the number of fog tile
 * \return int : Number of fog tiles in the field
 */
int getNbFog(Field* field);

#endif
