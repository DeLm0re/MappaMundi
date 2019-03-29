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
#include "neuralNetwork.h"

//Typedef of field
// use to know how interesting each point of the field are
typedef struct InterestField
{
    int width;
    int height;
    float** data;
}InterestField;

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
 * \fn float **create2DIntArray(int width, int height)
 * \brief function that creates a 2 dimension array of float
 *
 * \param width : width of the array
 * \param height : height of the array
 * \return float** : A pointer to the created array
 */
float **create2DFloatArray(int width, int height);

/**
 * \fn Field initialiseField(int width, int height, pointEnum defaultValue)
 * \brief function that initialise our field to make our environment
 *
 * \param width : width of the field, must be 1 widen than the original size because the edges is initialized with -1
 * \param height : height of the field, must be 1 widen than the original size because the edges is initialized with -1
 * \param defaultValue : the value we initialize each tile of the field with
 * \return Field : Pointer to a Field, which is a tydef declared in core.h (2D array struct)
 */
Field *initialiseField(int width, int height, pointEnum defaultValue);

/**
 * \fn Field initialiseInterestField(int width, int height)
 * \brief function that initialise our field to make our environment
 *
 * \param width : width of the interest field
 * \param height : height of the interest field
 * \return Field : Pointer to an InterestField
 */
InterestField *initialiseInterestField(int width, int height);

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
 * \fn void destruct2DFloatArray(int **array, int width)
 * \brief function that free the 2D array out of memory
 *
 * \param **array : The array to free
 * \param width : width of the array
 * \return void
 */
void destruct2DFloatArray(float **array, int width);

/**
 * \fn void destructField(Field* oneField)
 * \brief function that free the field out of memory
 *
 * \param **oneField : A double pointer on a field, which is a tydef declared in core.h (2D array structure)
 * \return void
 */
void destructField(Field **oneField);

/**
 * \fn void destructInterestField(InterestField **oneField)
 * \brief function that free the field out of memory
 *
 * \param **oneField : A double pointer on an InterestField
 * \return void
 */
void destructInterestField(InterestField **oneField);

/**
 * \fn int surface2DCircle(int radius)
 * \brief function that return the number of 1 by 1 square in a 2D circle
 *
 * \param radius : the radius of the circle
 * \return int : the surface
 */
int surface2DCircle(int radius);

/**
 * \fn Field* generateRandomFieldOfView(int visionRange)
 * \brief function that returns a random field of view
 * will be used for labelization
 *
 * \param int visionRange : the vision range
 * \param bool isValid : if the random field of view is a valid position to move or not
 * \return Field*
 */
Field* generateRandomFieldOfView(int visionRange, bool isValid);

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
float labeling(Field* fieldOfView, int xPosition, int yPosition, int xFinalPosition, int yFinalPosition);

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
float labeling2(Field* fieldOfView, int xPosition, int yPosition, int xFinalPosition, int yFinalPosition);

/**
 * \fn bool isVisibleFrom(Field* fieldOfView, int xOrigin, int yOrigin, int xPosition, int yPosition)
 * \brief function that returns true if a given position is visible from actual position
 *
 * \param Field* fieldOfView : a field of view
 * \param int xOrigin : x coordinate of actual position
 * \param int yOrigin : y coordinate of actual position
 * \param int xPosition : x coordinate we want to check
 * \param int yPosition : y coordinate we want to check
 * \return bool
 */
bool isVisibleFrom(Field* fieldOfView, int xOrigin, int yOrigin, int xPosition, int yPosition);



#endif
