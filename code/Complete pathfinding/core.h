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

#define EMPTY (0)
#define WALL (1)

//Typedef of field
typedef int** Field;

Field initialiseField(int height, int width);

void generateEnv(Field oneField, int height, int width);

void destructField(Field oneField, int height);