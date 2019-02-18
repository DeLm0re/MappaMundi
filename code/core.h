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
#include "display.h"

/**
 * \fn Field initialiseField(int height, int width)
 * \brief function that initialise our field to make our environment
 *
 * \param height : height of the field, must be 1 widen than the original size because the edges is initialized with -1
 * \param width : width of the field, must be 1 widen than the original size because the edges is initialized with -1
 * \return Field : A field, which is a tydef declared in core.h (2D array)
 */
Field initialiseField(int height, int width);

/**
 * \fn void generateEnv(field oneField)
 * \brief function that generate our field to make our environment
 *
 * \param oneField : A field, which is a tydef declared in core.h (2D array)
 * \param height : height of the field, must be 1 widen than the original size because the edges is initialized with -1
 * \param width : width of the field, must be 1 widen than the original size because the edges is initialized with -1
 * \return void
 */
void generateEnv(Field oneField, int height, int width);

/**
 * \fn void destructField(Field oneField, int width)
 * \brief function that free the field out of memory
 *
 * \param *oneField : A pointer on a field, which is a tydef declared in core.h (2D array)
 * \param width : width of the field, must be 1 widen than the original size because the edges is initialized with -1
 * \return void
 */
void destructField(Field *oneField, int width);

/**
 * \fn int surface2DCircle(int radius)
 * \brief function that return the surface of a 2D circle
 *
 * \param int radius : the radius of the circle
 * \return int : the surface
 */
int surface2DCircle(int radius);

#endif