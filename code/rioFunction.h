/**
 * \file rioFunction.h
 * \brief Processing functions
 * \author Louis Schuck
 * \version 0.2
 * \date 18/02/2019
 *
 * Module that contains multiple processing functions
 *
 */

//Header file containing the prototypes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <dirent.h>

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

#ifndef DT_REG
	#define DT_REG 8
#endif
#ifndef DT_DIR
	#define DT_DIR 4
#endif

/* \fn nmap(float value, float vmin, float vmax, float min, float max);
 * \brief function that transforms a value which vary between vmin and vmax, to an other one that vary between min and max
 * 
 * \param
 * 	value :	the reference value that we want to transform
 * 	vmin :	the minimum the value can get
 *	vmax :	the maximum the value can get
 *	min :	the new minimum we want to set the value to
 *	max :	the new maximum we want to set the value to
 *
 * \return
 *		return a float number between min and max
 */
float nmap(float value, float vmin, float vmax, float min, float max);

/* \fn cpyList(float* list, int size);
 * \brief function that copies an existing list of float
 *
 * \param
 *		list : 	the original list we want to copy
 *		size :	the size of the list
 *
 * \return
 *		return a list of float
 */
float* cpyList(float* list, int size);

/* \fn destructMatrice2D(void** matrice, int size);
 * \brief function that frees a 2D matrice from the memory
 *
 * \param
 *		matrice : 	the matrice we want to free from the memory
 *		size :	the size of the first dimmension of the list
 *
 * \return
 *		return a list of float
 */
void destructMatrice2D(void** matrice, int size);

/* \fn shuffleList(float* list, int size);
 * \brief function that shuffles the values in a given list
 *
 * \param
 *		list : 	the list we want to shuffle
 *		size :	the size of the list
 * \return
 * 		void
 */
void shuffleList(float* list, int size);

/* \fn getNumberOfFilesInDirectory(char* path);
 * \brief function that gets the number of files in a given directory
 *
 * \param
 *		path : 	the path of the directory
 *
 * \return
 		the number of files in the given directory
 */
int getNumberOfFilesInDirectory(char* path);

/* \fn getAllFileNameInDirectory(char* path);
 * \brief function that gets the name of each files in the given directory
 *
 * \param
 *		path : 	the path of the directory
 *
 * \return
 		a list of string containing the names of each files
 */
char** getAllFileNameInDirectory(char* path);
