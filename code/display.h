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

//Header file containing the prototypes
#include "core.h"

int drawField(SDL_Renderer *renderer, Field oneField, int height, int width, int size);

int draw(SDL_Renderer *renderer);