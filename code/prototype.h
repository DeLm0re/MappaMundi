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

//Typedef of field
typedef struct Field
{
    int width;
    int height;
    int** data;
}Field;


//Enumeration of what can be a point in the field
typedef enum {UNDEFINED = -1, EMPTY = 0, WALL = 1, FOG = 2, VISITED = 3} pointEnum;

//Enumeration of the different menu options
typedef enum {TRAIN_NN = 1, LOAD_NN = 2, TRAIN_GN = 3, LOAD_GN = 4} menuEnum;

//Enumeration of the different weights for the labeling
typedef enum {DIST = 0, NB_EMPTY = 1, NB_WALL = 2, NB_FOG = 3, NB_VISITED = 4, AVG_DIST_EMPTY = 5, AVG_DIST_WALL = 6, AVG_DIST_FOG = 7, AVG_DIST_VISITED = 8, DIST_FROM_ENTITY = 9} weightsEnum;

#endif

