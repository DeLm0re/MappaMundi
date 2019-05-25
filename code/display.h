/**
 * \file display.h
 * \brief Prototypes of the functions in display.c
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
 * \brief Function that draw our field in a graphical display
 *
 * \param renderer : renderer displayed by our graphical window using SDL
 * \param oneField : Field to display
 * \param size : the size of a single unique obstacle
 * 
 * \return int : -1 if there was an error during display, 0 otherwise
 */
int drawField(SDL_Renderer *renderer, Field *oneField, int size);

/**
 * \fn int drawFieldOfViewEntity(SDL_Renderer *renderer, Entity *oneEntity, Field *theField, int size)
 * \brief Function that draw the field of view of an entity in a graphical display
 *
 * \param renderer : renderer displayed by our graphical window using SDL
 * \param oneEntity : Pointer to a field of view of an entity
 * \param theField : Pointer to a field
 * \param size : the size of a single unique obstacle
 * 
 * \return int : -1 if there was an error during display, 0 otherwise
 */
int drawFieldOfViewEntity(SDL_Renderer *renderer, Entity *oneEntity, Field *theField, int size);

/**
 * \fn int draw(SDL_Renderer *renderer)
 * \brief Function that makes a drawing test
 *
 * \param renderer : renderer displayed by the graphical window using SDL
 * 
 * \return int : -1 if there was an error during display, 0 otherwise
 */
int draw(SDL_Renderer *renderer);


#endif
