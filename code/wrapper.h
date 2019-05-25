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
 * \brief Creates a neural network and trains it on randomly generated fields of view, then saves it
 * 
 * \param visionRange : size of the field of view used by the IA
 * \param data : structure which define the kind of event we have to raise for interruption
 * \param fieldHeight : height of the field
 * \param fieldWidth : width of the field
 * \param savingPathNN : path where to save the neural network
 * 
 * \return NeuralNetwork* : The created and trained neural network
 */
NeuralNetwork *trainingNN1(int visionRange, dataType *data, int fieldHeight, int fieldWidth, char *savingPathNN);

/**
 * \fn NeuralNetwork* trainingGN1(dataType *data, Field* theField, char *savingPathGN, char* basePathGN, int nbGeneration, int nbMember)
 * \brief Creates a neural network and trains it on randomly generated fields of view, then saves it
 * 
 * \param data : structure which define the kind of event we have to raise for interruption
 * \param theField : the field on which the genetic network will be trained
 * \param savingPathGN : path where to save the genetic network
 * \param basePathGN : path to a genetic network from which the first generation will be based on.
 * If NULL, then a default first generation will be created
 * \param nbGeneration : the number of generations the training will go through
 * \param nbMember : the number of members tested by generation
 * 
 * \return LabelingWeights* : The weights of the created and trained genetic network
 */
LabelingWeights *trainingGN1(dataType *data, Field* theField, char *savingPathGN, char* basePathGN, int nbGeneration, int nbMember);

/**
 * \fn NeuralNetwork* trainingGN2(dataType *data, Field* theField, char *savingPathGN, char* basePathGN, int nbGeneration, int nbMember, float percentReveal)
 * \brief Creates a neural network and trains it on randomly generated fields of view, then saves it
 * 
 * \param data : structure which define the kind of event we have to raise for interruption
 * \param theField : the field on which the genetic network will be trained
 * \param savingPathGN : path where to save the genetic network
 * \param basePathGN : path to a genetic network from which the first generation will be based on.
 * If NULL, then a default first generation will be created
 * \param nbGeneration : the number of generations the training will go through
 * \param nbMember : the number of members tested by generation
 * \param percentReveal : the percentage of maps revealed at which we concider that the entity finished his job
 * 
 * \return LabelingWeights* : The weights of the created and trained genetic network
 */
LabelingWeights *trainingGN2(dataType *data, Field* theField, char *savingPathGN, char* basePathGN, int nbGeneration, int nbMember, float percentReveal);

/**
 * \fn void moveEntityAlongPath(dataType *data, Entity* entity, node* pathToFollow, Field* theField, SDL_Renderer* renderer, int tileSize, int animationDelay, Statistics *stats)
 * \brief Make an entity follow a path and update its mental map
 * 
 * \param data : structure which define the kind of event we have to raise for interruption
 * \param entity : the entity that wil follow the path and wil update its mental map
 * \param pathToFollow : the path that the entity will follow
 * \param theField : the field where the entity is moving
 * \param renderer : the SDL renderer, use to visualize the entity on the map
 * \param tileSize : the size of one tile on the map
 * \param animationDelay : the amount of milliseconds the function will wait before each step of the entity
 * \param stats : the structure used to store statistics
 * 
 * \return void
 */
void moveEntityAlongPath(dataType *data, Entity* entity, node* pathToFollow, Field* theField, SDL_Renderer* renderer, int tileSize, int animationDelay, Statistics *stats);

/**
 * \fn void waitForInstruction(dataType *data)
 * \brief Wait until the repeat key is pressed (R) or the quit key is pressed (Q)
 * 
 * \param data : structure which defines the kind of event we have to raise for interruption
 *
 * \return void
 */
void waitForInstruction(dataType *data);

/**
 * \fn NeuralNetwork* trainingNN2(int fieldWidth, int fieldHeight, dataType *data, char *savingPathNN, SDL_Renderer *renderer, const int tileSize)
 * \brief Creates a neural network and trains it on random fields, then saves it
 * 
 * \param fieldWidth, fieldHeight : dimensions of the random fields
 * \param data : structure which define the kind of event we have to raise for interruption
 * \param savingPathNN : path where to save the neural network
 * \param renderer : renderer used to draw with the SDL
 * \param tileSize : size of a tile for display
 * 
 * \return NeuralNetwork* : The created and trained neural network
 */
NeuralNetwork *trainingNN2(int fieldWidth, int fieldHeight, dataType *data, char *savingPathNN, SDL_Renderer *renderer, const int tileSize);

/**
 * \fn void trainNN2onField(NeuralNetwork *neuralNetwork, dataType *data, Field* field, SDL_Renderer *renderer, const int tileSize)
 * \brief Trains a neural network on a single field
 * 
 * \param neuralNetwork : the neural network to train
 * \param data : structure which define the kind of event we have to raise for interruption
 * \param field : the field where to train the neural network
 * \param renderer : renderer used to draw with the SDL
 * \param tileSize : size of a tile for display
 * 
 * \return void
 */
void trainNN2onField(NeuralNetwork *neuralNetwork, dataType *data, Field* field, SDL_Renderer *renderer, const int tileSize);


/**
 * \fn void searchForEndPointNN(NeuralNetwork *neuralNetwork, dataType *data, Field** field, SDL_Renderer *renderer, const int tileSize, bool fieldIsFromImage, Statistics* stats)
 * \brief Shows the entity, starting in the top left corner, trying to go to the bottom right corner on the given field by using a neural network
 * 
 * \param neuralNetwork : the neural network to use to take decisions
 * \param data : structure which define the kind of event we have to raise for interruption
 * \param field : the field where the entity will move
 * \param renderer : renderer used to draw with the SDL
 * \param tileSize : size of a tile for display
 * \param fieldIsFromImage : to know if the field as been loaded from an image. If it is set to true, the map will not be update each loop
 * \param stats : the structure used to store the stats
 * 
 * \return void
 */
void searchForEndPointNN(NeuralNetwork *neuralNetwork, dataType *data, Field** field, SDL_Renderer *renderer, const int tileSize, bool fieldIsFromImage, Statistics* stats);

/**
 * \fn void searchForEndPointNN(LabelingWeights *labelingWeights, dataType *data, Field** field, SDL_Renderer *renderer, const int tileSize, bool fieldIsFromImage, Statistics* stats)
 * \brief Shows the entity, starting in the top left corner, trying to go to the bottom right corner on the given field by using a genetic algorithm
 * 
 * \param labelingWeights : the genetic algorithm to use to take decisions
 * \param data : structure which define the kind of event we have to raise for interruption
 * \param field : the field where the entity will move
 * \param renderer : renderer used to draw with the SDL
 * \param tileSize : size of a tile for display
 * \param fieldIsFromImage : to know if the field as been loaded from an image. If it is set to true, the map will not be update each loop
 * \param stats : the structure used to store the stats
 * 
 * \return void
 */
void searchForEndPointGN(LabelingWeights *labelingWeights, dataType *data, Field** field, SDL_Renderer *renderer, const int tileSize, bool fieldIsFromImage, Statistics* stats);

/**
 * \fn void exploreGN(LabelingWeights *labelingWeights, dataType *data, Field** field, SDL_Renderer *renderer, const int tileSize, bool fieldIsFromImage, int maxDecisions, float percentReveal, Statistics* stats)
 * \brief Shows the entity, starting in the top left corner, trying to explore as much of the given field as possible by using a genetic algorithm
 * 
 * \param labelingWeights : the genetic algorithm to use to take decisions
 * \param data : structure which define the kind of event we have to raise for interruption
 * \param field : the field where the entity will move
 * \param renderer : renderer used to draw with the SDL
 * \param tileSize : size of a tile for display
 * \param fieldIsFromImage : to know if the field as been loaded from an image. If it is set to true, the map will not be update each loop
 * \param maxDecisions : the maximum number of moves that the entity is allow to do before ending a loop
 * \param percentReveal : the percentage of map revealed at which we concider that the entity finished his job
 * \param stats : the structure used to store the stats
 * 
 * \return void
 */
void exploreGN(LabelingWeights *labelingWeights, dataType *data, Field** field, SDL_Renderer *renderer, const int tileSize, bool fieldIsFromImage, int maxDecisions, float percentReveal, Statistics* stats);

#endif
