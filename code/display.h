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

#include <SDL2/SDL.h>
#include "prototype.h"
#include "entity.h"

/**
 * \fn int drawField(SDL_Renderer *renderer, Field *oneField, int size)
 * \brief function that draw our field in a graphical display
 *
 * \param SDL_Renderer *renderer which is our renderer displayed by our graphical window using SDL
 * \param oneField* : Pointer to a field, which is a tydef declared in core.h (2D array structure)
 * \param size : the size of a single unique obstacle
 * \return int, can return an error during the drawing
 */
int drawField(SDL_Renderer *renderer, Field *oneField, int size);

/**
 * \fn int drawFieldOfViewEntity(SDL_Renderer *renderer, Entity *oneEntity, Field *theField, int size)
 * \brief function that draw the field of view of an entity in a graphical display
 *
 * \param SDL_Renderer *renderer which is our renderer displayed by our graphical window using SDL
 * \param oneEntity : Pointer to a field of view of an entity, which is a tydef declared in entity.h (2D array structure)
 * \param theField : Pointer to a field
 * \param size : the size of a single unique obstacle
 * \return int, can return an error during the drawing
 */
int drawFieldOfViewEntity(SDL_Renderer *renderer, Entity *oneEntity, Field *theField, int size);

/**
 * \fn int draw(SDL_Renderer *renderer)
 * \brief function that make a drawing test
 *
 * \param SDL_Renderer *renderer which is our renderer displayed by our graphical window using SDL
 * \return int, can return an error during the drawing
 */
int draw(SDL_Renderer *renderer);


#endif
