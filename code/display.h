/**
 * \file display.h
 * \brief Prototypes of the displayed project
 * \author Romain Jacquiez
 * \version 0.2
 * \date 09/10/2018
 *
 * Module that contain all prototypes of function in display.c
 *
 */

#ifndef H_DISPLAY
    #define H_DISPLAY

//Header file containing the prototypes
#include "prototype.h"

/**
 * \fn int drawField(SDL_Renderer *renderer, field oneField)
 * \brief function that draw our field in a graphical display
 *
 * \param SDL_Renderer *renderer which is our renderer displayed by our graphical window using SDL
 * \param oneField : A field, which is a tydef declared in core.h (2D array)
 * \param height : height of the field, must be 1 widen than the original size because the edges is initialized with -1
 * \param width : width of the field, must be 1 widen than the original size because the edges is initialized with -1
 * \param size : the size of a single unique obstacle
 * \return int, can return an error during the drawing
 */
int drawField(SDL_Renderer *renderer, Field oneField, int height, int width, int size);

/**
 * \fn int draw(SDL_Renderer *renderer)
 * \brief function that make a drawing test
 *
 * \param SDL_Renderer *renderer which is our renderer displayed by our graphical window using SDL
 * \return int, can return an error during the drawing
 */
int draw(SDL_Renderer *renderer);

#endif