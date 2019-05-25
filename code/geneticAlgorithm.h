/**
 * \file geneticAlgorithm.h
 * \brief Prototypes of the genetic Algorithm
 * \author Antoine Poirier-Rouillard
 * \version 0.1
 * \date 04/04/2019
 *
 * Module that contain all prototypes of function in geneticAlgorithm.c
 *
 */

#ifndef H_GENETICALGORITHM
    #define H_GENETICALGORITHM

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "prototype.h"
#include "core.h"
#include "rioFunction.h"

//Typedef of field
// use to know how interesting each point of the field are
typedef struct InterestField
{
    int width;
    int height;
    float** data;
}InterestField;

typedef struct 
{
    double weights[10];
} LabelingWeights;

typedef struct
{
    int size;
    LabelingWeights** list;
    float* score;
    float* time;
} GeneticNetworks;

/**
 * \fn float **create2DFloatArray(int width, int height)
 * \brief Function that creates a 2 dimension array of float
 *
 * \param width : width of the array
 * \param height : height of the array
 * 
 * \return float** : A pointer to the created array
 */
float **create2DFloatArray(int width, int height);

/**
 * \fn Field initialiseInterestField(int width, int height)
 * \brief Function that initializes our field to make our environment
 *
 * \param width : width of the interest field
 * \param height : height of the interest field
 * 
 * \return InterestField : The created InterestField
 */
InterestField *initialiseInterestField(int width, int height);

/**
 * \fn void destruct2DFloatArray(float **array, int width)
 * \brief Function that free the 2D array out of memory
 *
 * \param **array : The array to free
 * \param width : width of the array
 * 
 * \return void
 */
void destruct2DFloatArray(float **array, int width);

/**
 * \fn void destructInterestField(InterestField **oneField)
 * \brief Function that frees an InterestField out of memory
 *
 * \param **oneField : address of the InterestField to free
 * 
 * \return void
 */
void destructInterestField(InterestField **oneField);

/**
 * \fn float labeling1(Field* fieldOfView, int xPosition, int yPosition, int xFinalPosition, int yFinalPosition)
 * \brief Function that returns the labeling of the points, will be used for labelisation
 *
 * \param Field* fieldOfView : a field of view
 * \param int xPosition : x coordinate of the entity
 * \param int yPosition : y coordinate of the entity
 * \param int xFinalPosition : x coordinate of the end point
 * \param int yFinalPosition : y coordinate of the end point
 * 
 * \return float : The labels of each point
 */
float labeling1(Field* fieldOfView, int xPosition, int yPosition, int xFinalPosition, int yFinalPosition);

/**
 * \fn LabelingWeights* initialiseLabelingWeights()
 * \brief Function that returns the initialized labeling weights structure
 *
 * \return LabelingWeights* : The initilized weights
 */
LabelingWeights* initialiseLabelingWeights(void);

/**
 * \fn LabelingWeights* copyLabelingWeights(LabelingWeights* labelingWeights)
 * \brief Function that returns the copy of the given labeling Weights
 * 
 * \param *labelingWeights : A pointer on the LabelingWeights to copy
 *
 * \return LabelingWeights* : The copy of the weights
 */
LabelingWeights* copyLabelingWeights(LabelingWeights* labelingWeights);

/**
 * \fn void destructLabelingWeights(LabelingWeights **labelingWeights)
 * \brief Function that frees the labeling weight structure out of memory
 *
 * \param **labelingWeights : address of the labelingWeights to destruct
 * 
 * \return void
 */
void destructLabelingWeights(LabelingWeights **labelingWeights);

/**
 * \fn GeneticNetworks *initialiseGeneticNetworks(int size)
 * \brief Function that initializes a list of genetic networks (presented as LabelingWeights structures)
 *
 * \param size : the number of genetic networks to load in the structure
 * 
 * \return GeneticNetworks : List of genetic networks initilized
 */
GeneticNetworks *initialiseGeneticNetworks(int size);

/**
 * \fn GeneticNetworks *initialiseGeneticNetworksFrom(int size, LabelingWeights* labelingWeights)
 * \brief function that initializes a list of genetic networks (presented as LabelingWeights structures)
 *  based on an existing genetic network
 *
 * \param size : the number of genetic networks to load in the structure
 * \param *pathOfGeneticNetwork : a pointer to the path of a genetic network
 * \param variation : variation from the original genetic network. Must be superior to 0
 * 
 * \return GeneticNetworks : Initilized list of genetic network
 */
GeneticNetworks *initialiseGeneticNetworksFrom(int size, char* pathOfGeneticNetwork, float variation);

/**
 * \fn GeneticNetworks *createNewGeneration(GeneticNetworks* geneticNetworks, int numberOfBreeder, float mutationChance)
 * \brief Function that create a new list of genetic network based on a previous one
 *
 * \param numberOfBreeder : the number of genetic networks that will be selected to reproduce among the best
 * \param mutationChance : the mutation chance of each member of the new generation
 * 
 * \return GeneticNetworks : Created GeneticNetworks
 */
GeneticNetworks *createNewGeneration(GeneticNetworks* geneticNetworks, int numberOfBreeder, float mutationChance);

/**
 * \fn void destructGeneticNetworks(GeneticNetworks **geneticNetworks)
 * \brief Function that free a list of genetic network
 *
 * \param **geneticNetworks : address of the GeneticNetworks to free
 * 
 * \return void
 */
void destructGeneticNetworks(GeneticNetworks **geneticNetworks);

/**
 * \fn sortGeneticNetworks(GeneticNetworks* geneticNetworks);
 * \brief Function that sort genetic networks based on their score
 *
 * \param **geneticNetworks : address the GeneticNetworks we want to sort
 * 
 * \return void
 */
void sortGeneticNetworks(GeneticNetworks* geneticNetworks);

/**
 * \fn void saveGeneticNetwork(LabelingWeights* labelingWeights, char* path)
 * \brief Function that save a genetic network 
 *
 * \param *LabelingWeights : A pointer on the LabelingWeights (genetic network) we want to save
 * \param *path : The path to the save file
 * 
 * \return void
 */
bool saveGeneticNetwork(LabelingWeights* labelingWeights, char* path);

/**
 * \fn LabelingWeights* loadGeneticNetwork(char* path);
 * \brief Function that loads a genetic network 
 *
 * \param *path : the path to the file
 * 
 * \return LabelingWeights* : The loaded weights
 */
LabelingWeights* loadGeneticNetwork(char* path);



#endif
