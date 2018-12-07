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

//Our define
    //Definition of a case's value if it is empty
    #define EMPTY (0)
    //Definition of a case's value if it is a wall
    #define WALL (1)
    //Definition of a case's value if it is fog
    #define FOG (2)

//Typedef of field
typedef int** Field;

//Enumeration of what can be a point in the field
typedef enum {EMPTY = 0, WALL = 1, FOG = 2} pointEnum;