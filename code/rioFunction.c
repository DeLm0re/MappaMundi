/**
 * \file rioFunction.c
 * \brief Processing functions
 * \author Louis Schuck
 * \version 0.2
 * \date 18/02/2019
 *
 * Module that contains multiple processing functions
 *
 */

//Header file containing the prototypes
#include "rioFunction.h"

/* \fn nmap(float value, float vmin, float vmax, float min, float max);
 * \brief function that transforms a value which vary between vmin and vmax, to an other one that vary between min and max
 * 
 * \param value :	the reference value that we want to transform
 * \param vmin :	the minimum the value can get
 * \param vmax :	the maximum the value can get
 * \param min :	the new minimum we want to set the value to
 * \param max :	the new maximum we want to set the value to
 *
 * \return
 *		return a float number between min and max
 */
float nmap(float value, float vmin, float vmax, float min, float max)
{
	// Original lenght
	float vlen = vmax - vmin;
	// Futur lenght
	float len = max - min;
	// Mapping the value
	float mapValue = (value-vmin)/vlen;
	// Return the new walue
	return min + mapValue * len;
}

/* \fn cpyList(float* list, int size);
 * \brief function that copies an existing list of float
 *
 * \param list : 	the original list we want to copy
 * \param size :	the size of the list
 *
 * \return
 *		return a list of float
 */
float* cpyList(float* list, int size)
{
	// We initialize the new list
	float* newList = malloc(sizeof(float)*size);
	// For each value in the list
	int i;
	for(i = 0; i < size; i++)
	{
		// We copy it to the new list
		newList[i] = list[i];
	}
	// Then, we return the new list
	return newList;
}

/* \fn destructMatrice2D(void** matrice, int size);
 * \brief function that frees a 2D matrice from the memory
 *
 * \param matrice : the matrice we want to free from the memory
 * \param size : the size of the first dimmension of the list
 *
 * \return
 *		return a list of float
 */
void destructMatrice2D(void** matrice, int size)
{
	// For each row of the matrice
	int i;
	for(i = 0; i < size; i++)
	{
		// We free it from the memory
		free(matrice[i]);
	}
	// Then we free the matrice
	free(matrice);
}

/* \fn shuffleList(float* list, int size);
 * \brief function that shuffles the values in a given list
 *
 * \param list : the list we want to shuffle
 * \param size : the size of the list
 * \return
 * 		void
 */
void shuffleList(float* list, int size)
{
	float temp; // use to store temporarly a value of a list which will be move
	// Both these indexes will be use to invert the position of two values in the list
	int index1;
	int index2;
	// We do a fixed number of index inversion
	int i;
	for(i = 0; i < size; i++)
	{
		// We choose two random index
		index1 = rand()%size;
		index2 = rand()%size;
		// We store the value of the first index in the temporary variable
		temp = list[index1];
		// We store the value of the second index in the first idnex
		list[index1] = list[index2];
		// We put the first value back to the second index
		list[index2] = temp;
	}
}

/* \fn getNumberOfFilesInDirectory(char* path);
 * \brief function that gets the number of files in a given directory
 *
 * \param
 *		path : 	the path of the directory
 *
 * \return
 		the number of files in the given directory
 */
int getNumberOfFilesInDirectory(char* path)
{
	int nbFile = 0; // use to store the number of files found
	struct dirent* directoryEntry; // use as a pointer that store the directory entry
	// We open the dirrectory we want to check
	DIR* directory = opendir(path);
	// If it has correctly been opened
	if (directory != NULL)
	{
		// While we can read files in the directory
		while ((directoryEntry = readdir(directory)))
		{
			// If the entry is a regular file
			if (directoryEntry->d_type == DT_REG)
			{
				// We increase the number of files in the directory
				nbFile++;
			}
		}
		// Then, we close the directory
		closedir(directory);
	}
	// The, we return the number of files in the directory
	return nbFile;
}

/* \fn getAllFileNameInDirectory(char* path);
 * \brief function that gets the name of each files in the given directory
 *
 * \param
 *		path : 	the path of the directory
 *
 * \return
 		a list of string containing the names of each files
 */
char** getAllFileNameInDirectory(char* path)
{
	// We find and store the number of files in the given directory
	int nbFile = getNumberOfFilesInDirectory(path);
	
	// We initialize the list of names
	char** fileNames = malloc(sizeof(char*)*nbFile);
	// And for each names
	int indexName;
	for(indexName = 0; indexName < nbFile; indexName++)
	{
		// We allocate the memory to store it using the standards of the dirent library
		fileNames[indexName] = malloc(sizeof(char)*256);
	}
	
	struct dirent* directoryEntry; // use as a pointer that store the directory entry
	// We open the dirrectory we want to check
	DIR* directory = opendir(path);
	// If it has correctly been opened
	if (directory != NULL)
	{
		// We reset the index name
		indexName = 0;
		// While we can read files in the directory and whilethe list of name isn't full
		while ((directoryEntry = readdir(directory)) && indexName < nbFile)
		{
			// If the entry is a regular file
			if (directoryEntry->d_type == DT_REG)
			{
				// We add the name to the list of string
				strcpy(fileNames[indexName], directoryEntry->d_name);
				// We go to the next file
				indexName++;
			}
		}
		// Then, we close the directory
		closedir(directory);
	}
	// The, we return the list of names
	return fileNames;
}
