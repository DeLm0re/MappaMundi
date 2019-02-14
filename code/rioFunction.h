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

/* @function
 *		transform a value which vary between vmin and vmax, to an other one that vary between min and max
 *
 * @param
 *		value : 	the reference value that we want to transform
 *		vmin :		the minimum the value can get
 *		vmax :		the maximum the value can get
 *		min :		the new minimum we want to set the value to
 *		max :		the new maximum we want to set the value to
 *
 * @return
 *		return a float number between min and max
 */
float nmap(float value, float vmin, float vmax, float min, float max);

/* @function
 *		copy an existing list of float
 *
 * @param
 *		list : 	the original list we want to copy
 *		size :	the size of the list
 *
 * @return
 *		return a list of float
 */
float* cpyList(float* list, int size);

/* @function
 *		free a 2D matrice from the memory
 *
 * @param
 *		matrice : 	the matrice we want to free from the memory
 *		size :	the size of the first dimmension of the list
 *
 * @return
 *		return a list of float
 */
void destructMatrice2D(void** matrice, int size);

/* @function
 *		shuffle the values in a given list
 *
 * @param
 *		list : 	the list we want to shuffle
 *		size :	the size of the list
 */
void shuffleList(float* list, int size);

/* @function
 *		get the number of files in a given directory
 *
 * @param
 *		path : 	the path of the directory
 *
 * @return
 		the number of files in the given directory
 */
int getNumberOfFilesInDirectory(char* path);

/* @function
 *		get the name of each files in the given directory
 *
 * @param
 *		path : 	the path of the directory
 *
 * @return
 		a list of string containing the names of each files
 */
char** getAllFileNameInDirectory(char* path);
