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

//Header file for graphical display
#include <SDL2/SDL.h>

//Typedef of field
typedef int field[52][52];

void initialiseField(field unField, int height, int width);

void generateEnv(field unField, int height, int width);