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
#include <SDL2/SDL.h>
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
 * \fn NeuralNetwork* trainingNN2(dataType *data, Field *field, char *savingPathNN)
 * \brief creates a neural network and trains it on a field, then saves it
 * 
 * \param
 * 		data : structure which define the kind of event we have to raise for interruption
 * 		fieldName : the name of the field where to train the neural network
 * 		savingPathNN : path where to save the neural network
 * 		renderer : renderer used to draw with the SDL
 * 		tileSize : size of a tile for display
 *      entityColor : color of the entity to display
 * \return
 * 		NeuralNetwork*
 */
NeuralNetwork *trainingNN2(dataType *data, char* fieldName, char *savingPathNN, SDL_Renderer *renderer, const int tileSize, SDL_Color entityColor);