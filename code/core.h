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
 * \param height : height of the field 
 * \param width : width of the field
 * \param defaultValue : value used to set each point of the Field
 * \return Field : A field, which is a tydef declared in core.h (2D array)
 */
Field *initialiseField(int height, int width, pointEnum defaultValue);

/**
 * \fn void generateEnv(field oneField)
 * \brief function that generate our field to make our environment
 *
 * \param oneField : A field, which is a tydef declared in core.h (2D array)
 * \return void
 */
void generateEnv(Field *oneField);

/**
 * \fn void destructField(Field oneField, int height)
 * \brief function that free the field out of memory
 *
 * \param oneField : A field, which is a tydef declared in core.h (2D array)
 * \return void
 */
void destructField(Field **oneField);

#endif