/**
 * \file prototype.h
 * \brief Prototypes of the displayed project
 * \author Romain Jacquiez
 * \version 0.1
 * \date 30/11/2018
 *
 * Module that contain all prototypes of our data types and define
 *
 */

#ifndef H_PROTOTYPE
    #define H_PROTOTYPE

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

#define CUSTOM_FIELD_PATH "../custom_field/"
#define CUSTOM_FIELD_EXTENSION ".bmp"

//General header files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//More specific headers files
#include <SDL2/SDL.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#include "BmpLib.h"


//Typedef of field
typedef struct Field
{
    int width;
    int height;
    int** data;
}Field;


//Enumeration of what can be a point in the field
typedef enum {UNDEFINED = -1, EMPTY = 0, WALL = 1, FOG = 2, VISITED = 3} pointEnum;

#endif

