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

#include "core.h"

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
    float weights[9];
} LabelingWeights;

typedef struct
{
    int size;
    LabelingWeights** list;
    float* score;
} GeneticNetworks;

/**
 * \fn float **create2DIntArray(int width, int height)
 * \brief function that creates a 2 dimension array of float
 *
 * \param width : width of the array
 * \param height : height of the array
 * \return float** : A pointer to the created array
 */
float **create2DFloatArray(int width, int height);

/**
 * \fn Field initialiseInterestField(int width, int height)
 * \brief function that initialise our field to make our environment
 *
 * \param width : width of the interest field
 * \param height : height of the interest field
 * \return Field : Pointer to an InterestField
 */
InterestField *initialiseInterestField(int width, int height);

/**
 * \fn void destruct2DFloatArray(int **array, int width)
 * \brief function that free the 2D array out of memory
 *
 * \param **array : The array to free
 * \param width : width of the array
 * \return void
 */
void destruct2DFloatArray(float **array, int width);

/**
 * \fn void destructInterestField(InterestField **oneField)
 * \brief function that free the field out of memory
 *
 * \param **oneField : A double pointer on an InterestField
 * \return void
 */
void destructInterestField(InterestField **oneField);

/**
 * \fn Field* labeling(Field* fieldOfView, int xPosition, int yPosition, int xFinalPosition, int yFinalPosition)
 * \brief function that returns the labeling of the points
 * will be used for labelisation
 *
 * \param Field* fieldOfView : a field of view
 * \param int xPosition : x coordinate of the entity
 * \param int yPosition : y coordinate of the entity
 * \param int xFinalPosition : x coordinate of the end point
 * \param int yFinalPosition : y coordinate of the end point
 * \return float
 */
float labeling(Field* fieldOfView, int xPosition, int yPosition, int xFinalPosition, int yFinalPosition);

/**
 * \fn Field* labeling2(Field* fieldOfView, int xPosition, int yPosition, int xFinalPosition, int yFinalPosition)
 * \brief function that returns the labeling of the points
 * will be used for labelisation
 *
 * \param Field* fieldOfView : a field of view
 * \param int xPosition : x coordinate of the entity
 * \param int yPosition : y coordinate of the entity
 * \param int xFinalPosition : x coordinate of the end point
 * \param int yFinalPosition : y coordinate of the end point
 * \return float
 */
float labeling2(Field* fieldOfView, int xPosition, int yPosition, int xFinalPosition, int yFinalPosition);

/**
 * \fn LabelingWeights* initialiseLabelingWeights()
 * \brief function that returns the initialized labeling weights structure
 *
 * \return LabelingWeights*
 */
LabelingWeights* initialiseLabelingWeights(void);

/**
 * \fn void destructLabelingWeights(LabelingWeights **labelingWeights)
 * \brief function that free the labeling weight structure out of memory
 *
 * \param **labelingWeights : A double pointer on a LabelingWeights
 * \return void
 */
void destructLabelingWeights(LabelingWeights **labelingWeights);

/**
 * \fn Field* labeling3(Field* fieldOfView, int xPosition, int yPosition, int xFinalPosition, int yFinalPosition, LabelingWeights* labelingWeights)
 * \brief function that returns the labeling of the points
 * will be used for labelisation
 *
 * \param Field* fieldOfView : a field of view
 * \param int xPosition : x coordinate of the entity
 * \param int yPosition : y coordinate of the entity
 * \param int xFinalPosition : x coordinate of the end point
 * \param int yFinalPosition : y coordinate of the end point
 * \param LabelingWeights* labelingWeights : Ze labeling weights
 * \return float
 */
float labeling3(Field* fieldOfView, int xPosition, int yPosition, int xFinalPosition, int yFinalPosition, LabelingWeights* labelingWeights);

/**
 * \fn GeneticNetworks *initialiseInterestField(int size)
 * \brief function that initialise a list of genetic networks (presented as LabelingWeights structures)
 *
 * \param size : the number of genetic networks to load in the structure
 * \return GeneticNetworks : Pointer to a GeneticNetworks
 */
GeneticNetworks *initialiseGeneticNetworks(int size);

/**
 * \fn GeneticNetworks *createNewGeneration(GeneticNetworks* geneticNetworks, int numberOfBreeder)
 * \brief function that create a new list of genetic network based on a previous one
 *
 * \param numberOfBreeder : the number of genetic networks that will be selected to reproduce among the best
 * \param mutationChance : the mutation chance of each member of the new generation
 * \return GeneticNetworks : Pointer to a GeneticNetworks
 */
GeneticNetworks *createNewGeneration(GeneticNetworks* geneticNetworks, int numberOfBreeder, float mutationChance);

/**
 * \fn void destructGeneticNetworks(GeneticNetworks **geneticNetworks)
 * \brief function that free a list of genetic network
 *
 * \param **geneticNetworks : A double pointer on a GeneticNetworks
 * \return void
 */
void destructGeneticNetworks(GeneticNetworks **geneticNetworks);

/**
 * \fn void sortGeneticNetworks(GeneticNetworks* geneticNetworks)
 * \brief function that sort genetic networks based on their score
 *
 * \param **geneticNetworks : A double pointer on the GeneticNetworks we want to sort
 * \return void
 */
void sortGeneticNetworks(GeneticNetworks* geneticNetworks);

/**
 * \fn void saveGeneticNetwork(LabelingWeights* labelingWeights, char* path)
 * \brief function that save a genetic network 
 *
 * \param *LabelingWeights : A pointer on the LabelingWeights (genetic network) we want to save
 * \param *path : The path to the save file
 * \return bool
 */
bool saveGeneticNetwork(LabelingWeights* labelingWeights, char* path);

/**
 * \fn LabelingWeights* loadGeneticNetwork(char* path);
 * \brief function that load a genetic network 
 *
 * \param *path : The path to the file
 * \return LabelingWeights*
 */
LabelingWeights* loadGeneticNetwork(char* path);



#endif