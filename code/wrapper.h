/**
 * \file wrapper.h
 * \brief wrapper functions to keep the main.c clear
 * \author Théo Hipault
 * \version 0.1
 * \date 30/04/2019
 *
 * Module that contain all the prototypes of functions in wrapper.c
 *
 */

#include <pthread.h>
#include "prototype.h"
#include "eventhandler.h"
#include "rioFunction.h"
#include "neuralNetwork.h"
#include "core.h"
#include "geneticAlgorithm.h"
#include "pathfinding.h"
#include "entity.h"
#include "display.h"


/**
 * \fn NeuralNetwork* trainingNN1(int visionRange, dataType *data, int fieldHeight, int fieldWidth, char *savingPathNN)
 * \brief creates a neural network and trains it on randomly generated fields of view, then saves it
 * 
 * \param
 * 		visionRange : size of the field of view used by the IA
 * 		data : structure which define the kind of event we have to raise for interruption
 * 		fieldHeight : height of the field
 * 		fieldWidth : width of the field
 * 		savingPathNN : path where to save the neural network
 * \return
 * 		NeuralNetwork*
 */
NeuralNetwork *trainingNN1(int visionRange, dataType *data, int fieldHeight, int fieldWidth, char *savingPathNN);

/**
 * \fn NeuralNetwork* trainingGN1(int visionRange, dataType *data, int fieldHeight, int fieldWidth, char *savingPathGN)
 * \brief creates a neural network and trains it on randomly generated fields of view, then saves it
 * 
 * \param
 * 		data : structure which define the kind of event we have to raise for interruption
 * 		fieldHeight : height of the field
 * 		fieldWidth : width of the field
 * 		savingPathGN : path where to save the genetic network
 *      basePathGN : path to a genetic network from which the first generation will be based on.
 *                  If NULL, then a default first generation will be created
 *      nbGeneration : the number of generation the training wil go throught
 *      nbMember : the number of member tested by generation
 * \return
 * 		LabelingWeights*
 */
LabelingWeights *trainingGN1(dataType *data, int fieldHeight, int fieldWidth, char *savingPathGN, char* basePathGN, int nbGeneration, int nbMember);
