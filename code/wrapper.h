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

#ifndef H_WRAPPER
    #define H_WRAPPER

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
#include "stats.h"


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
 * \fn NeuralNetwork* trainingGN1(int visionRange, dataType *data, Field* theField, char *savingPathGN)
 * \brief creates a neural network and trains it on randomly generated fields of view, then saves it
 * 
 * \param
 * 		data : structure which define the kind of event we have to raise for interruption
 *      theField : the field on which the genetic network will be trained
 * 		savingPathGN : path where to save the genetic network
 *      basePathGN : path to a genetic network from which the first generation will be based on.
 *                  If NULL, then a default first generation will be created
 *      nbGeneration : the number of generation the training wil go throught
 *      nbMember : the number of member tested by generation
 * \return
 * 		LabelingWeights*
 */
LabelingWeights *trainingGN1(dataType *data, Field* theField, char *savingPathGN, char* basePathGN, int nbGeneration, int nbMember);

/**
 * \fn void moveEntityAlongPath(Entity* entity, node* pathToFollow, Field* theField, SDL_Renderer* renderer, int tileSize, dataType* data, bool displayOn)
 * \brief make an entity follow a path and update its mental map
 * 
 * \param
 * 		data : structure which define the kind of event we have to raise for interruption
 *      entity : the entity that wil follow the path and wil update its mental map
 *      pathToFollow : the path that the entity will follow
 *      theField : the field where the entity is moving
 *      renderer : the SDL renderer, use to visualize the entity on the map
 *      tileSize : the size of one tile on the map
 *      animationDelay : the amount of milliseconds the function will wait before each step of the entity
 *		stats : the structure used to store statistics
 * 
 * \return
 * 		LabelingWeights*
 */
void moveEntityAlongPath(dataType *data, Entity* entity, node* pathToFollow, Field* theField, SDL_Renderer* renderer, int tileSize, int animationDelay, Statistics *stats);

/**
 * \fn void waitForInstruction(dataType *data)
 * \brief wait until the repeat key is pressed (R) or the quit key is pressed (Q)
 * 
 * \param
 * 		data : structure which define the kind of event we have to raise for interruption
 *
 * \return
 * 		void
 */
void waitForInstruction(dataType *data);

/**
 * \fn NeuralNetwork* trainingNN2(dataType *data, char* fieldName, char *savingPathNN, SDL_Renderer *renderer, const int tileSize, SDL_Color entityColor)
 * \brief creates a neural network and trains it on random fields, then saves it
 * 
 * \param
 * 		fieldWidth, fieldHeight : dimensions of the random fields
 * 		data : structure which define the kind of event we have to raise for interruption
 * 		savingPathNN : path where to save the neural network
 * 		renderer : renderer used to draw with the SDL
 * 		tileSize : size of a tile for display
 * \return
 * 		NeuralNetwork*
 */
NeuralNetwork *trainingNN2(int fieldWidth, int fieldHeight, dataType *data, char *savingPathNN, SDL_Renderer *renderer, const int tileSize);

/**
 * \fn void trainNN2onField(NeuralNetwork *neuralNetwork, dataType *data, Field* field, SDL_Renderer *renderer, const int tileSize, SDL_Color entityColor)
 * \brief trains a neural network on a single field
 * 
 * \param
 * 		neuralNetwork : the neural network to train
 * 		data : structure which define the kind of event we have to raise for interruption
 * 		field : the field where to train the neural network
 * 		renderer : renderer used to draw with the SDL
 * 		tileSize : size of a tile for display
 * \return
 * 		void
 */
void trainNN2onField(NeuralNetwork *neuralNetwork, dataType *data, Field* field, SDL_Renderer *renderer, const int tileSize);

#endif