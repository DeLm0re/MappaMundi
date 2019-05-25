/**
 * \file entity.h
 * \brief Prototype of functions in entity.c
 * \author Louis Schuck
 * \version 0.1
 * \date 30/11/2018
 *
 * Module that contain the functions and the structure used by the neural network to navigate in the Field
 *
 */

#ifndef H_ENTITY
    #define H_ENTITY

//Define the maximum number of points a field of view can have
#define RADIUS_VIEWPOINT (5)

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "stats.h"
#include "eventhandler.h"
#include "pathfinding.h"
#include "neuralNetwork.h"
#include "geneticAlgorithm.h"

//The structure point used as a point of a field which can be a wall, empty or fog
typedef struct Point
{
    int x;
    int y;
    pointEnum pointValue;
} Point;

//The structure entity used as a token which will be control by our neural network
typedef struct Entity
{
    int x;
    int y;
    int visionRange;
    Point** fieldOfView;

    Field* mentalMap;
} Entity;

//The structure used to store the inputs of the neural network
typedef struct InputNeuralNetwork
{
    float* data;
    int size;
} InputNeuralNetwork;

/**
 * \fn Entity* initialiseEntity(int x, int y, int visionRange, int fieldWidth, int fieldHeight)
 * \brief Function that initialize an Entity. An Entity is used by the neural network to navigate in a Field
 *
 * \param x, y : the coordinates of the Entity
 * \param visionRange : the maximum distance at which the entity can see the Field
 * \param fieldWidth, fieldHeight : dimention of the field of the environment
 * 
 * \return Entity* : The initialized Entity
 */
Entity* initialiseEntity(int x, int y, int visionRange, int fieldWidth, int fieldHeight);

/**
 * \fn initializeMentalMapEntity(Entity *entity, int width, int height)
 * \brief Function that initialize the mental map of a given entity
 * 
 * \param *entity : the entity which have the mental map to be updated
 * \param width : width of the mental map we want to initialize
 * \param height : height of the mental map we want to initialize
 * 
 * \return void
 */
void initializeMentalMapEntity(Entity *entity, int width, int height);

/**
 * \fn void initialiseFieldOfViewEntity(Entity *entity)
 * \brief Function that initializes the field of view of an entity
 *
 * \param *entity : the entity which have the field of view we want to update
 * 
 * \return void
 */
void initialiseFieldOfViewEntity(Entity *entity);

/**
 * \fn void destructEntity(Entity** entity)
 * \brief Function used to free an Entity
 *
 * \param entity : the Entity to free
 * 
 * \return void
 */
void destructEntity(Entity** entity);

/**
 * \fn void destructFieldOfViewEntity(Entity *entity)
 * \brief Function used to free a field of view of an Entity
 *
 * \param entity : the Entity that contains the field of view we want to free
 * 
 * \return void
 */
void destructFieldOfViewEntity(Entity *entity);

/**
 * \fn void showEntity(Entity* entity, SDL_Renderer* renderer, SDL_Color color, int tileSize)
 * \brief Function used to display an Entity
 *
 * \param entity : the Entity to display
 * \param renderer : the renderer used with the window. The renderer must have been set using SDL functions
 * \param color : the color which will be used to display the entity
 * \param tileSize : the size of one tile in the window
 * 
 * \return void
 */
void showEntity(Entity* entity, SDL_Renderer* renderer, SDL_Color color, int tileSize);

/**
 * \fn void updateMentalMapEntity(Entity *entity)
 * \brief Function that updates the mental map of an entity
 *
 * \param entity : the Entity to update
 * \param stats : the structure we use to store statistics
 * 
 * \return void
 */
void updateMentalMapEntity(Entity *entity, Statistics *stats);

/**
 * \fn void updateFieldOfViewEntity(Field *aField, Entity *entity)
 * \brief Function that updates the field of view of an entity
 *
 * \param entity : the Entity to update
 * \param field* : A pointer to the field on which we are based
 * 
 * \return void
 */
void updateFieldOfViewEntity(Field *aField, Entity *entity);

/**
 * \fn Field* getFieldOfViewFromMap(Field* map, int x, int y, int visionRange)
 * \brief Function that returns the field of view from a map on a given coordinate
 *
 * \param *mentalMap : a pointer to the map from which we want to extrat the field of view
 * \param x, y : the coordinates in the map from wihc we will extract the field of view
 * \param visionRange : the radius of the field of view
 * 
 * \return Field* : The field of view
 */
Field* getFieldOfViewFromMap(Field* map, int x, int y, int visionRange);

/**
 * \fn createInput(Field* fieldOfView, int x, int y, int xEnd, int yEnd)
 * \brief Function that creates the inputs for the neural network based on the mental map, the vision range,
 * the coordinate of the point we want to test and the coordinate the entity wants to get to
 *
 * \param mentalMap* : A pointer to the mental map of the entity
 * \param visionRange : the vision range of the entity
 * \param x, y : the coordinate of the point we want to test in the neural network
 * \param xEnd, yEnd : the coordinate the entity wants to get to
 *      
 * \return InputNeuralNetwork* : The created input for the neural network
 */
InputNeuralNetwork* createInput(Field* fieldOfView, int x, int y, int xEnd, int yEnd);

/**
 * \fn createInputNN2(Field* field, int entityX, int entityY, int xEnd, int yEnd)
 * \brief Second version of the function that creates the inputs for the neural network based on a field,
 *  the coordinate of the entity and the coordinate the entity wants to get to
 *
 * \param field : A pointer to the field we want to convert
 * \param visionRange : the vision range of the entity
 * \param entityX, entityY : the coordinate of the entity
 * \param xEnd, yEnd : the coordinate the entity wants to get to
 *      
 * \return InputNeuralNetwork* : The created input for the neural network
 */
float* createInputNN2(Field* field, int entityX, int entityY, int xEnd, int yEnd);

/**
 * \fn void destructInput(InputNeuralNetwork** input)
 * \brief Frees a structure InputNeuralNetwork from the memory
 *
 * \param InputNeuralNetwork** input : a double pointer to the structure wwe want to free
 *      
 * \return void
 */
void destructInput(InputNeuralNetwork** input);

/**
 * \fn void updateInterestField(InterestField* interestField, NeuralNetwork* neuralNetwork, Field* mentalMap, int xEnd, int yEnd, int visionRange)
 * \brief Function that changes the values in an interest field according to the output of a trained neural network
 * will be used for labelisation
 *
 * \param InterestField* interestField : the interest field that will be update
 * \param NeuralNetwork* neuralNetwork : the neural network that will give their values to the point in the interest field
 * \param Field* mentalMap : the mental map on wich the updated values will be based on
 * \param int xEnd : x coordinate of the ending point
 * \param int yEnd : y coordinate of the ending point
 * 
 * \return void
 */
void updateInterestField(InterestField* interestField, NeuralNetwork* neuralNetwork, Field* mentalMap, int xEnd, int yEnd, int visionRange);

/**
 * \fn void updateInterestField2(InterestField* interestField, int xEnd, int yEnd, Entity* entity, LabelingWeights* labelingWeights)
 * \brief Function that changes the values in an interest field according to the labelisation function
 *
 * \param InterestField* interestField : the interest field that will be update
 * \param int xEnd : x coordinate of the ending point
 * \param int yEnd : y coordinate of the ending point
 * \param Entity* entity : the entity on which we base the update of the interest field
 * \param LabelingWeights* labelingWeights : Ze labeling weights

 * \return void
 */
void updateInterestField2(InterestField* interestField, int xEnd, int yEnd, Entity* entity, LabelingWeights* labelingWeights);

/**
 * \fn void updateInterestField3(InterestField* interestField, Entity* entity, LabelingWeights* labelingWeights)
 * \brief Function that changes the values in an interest field according to the labelisation function
 *
 * \param InterestField* interestField : the interest field that will be update
 * \param Entity* entity : the entity on which we base the update of the interest field
 * \param LabelingWeights* labelingWeights : Ze labeling weights

 * \return void
 */
void updateInterestField3(InterestField* interestField, Entity* entity, LabelingWeights* labelingWeights);

/**
 * \fn void updateBestWantedPosition(node* wantedPosition, InterestField* interestField)
 * \brief Function that changes the coordinate of a node to the best coordinate on the interest field.
 * This is use to get the position where our entity will go next.
 * Each time this function is called, the choosen position will be set to 0 (no interest)
 * will be used for labelisation
 *
 * \param node* wantedPosition : the node which will be updated to the best position on the interest field
 * \param InterestField* interestField : the interest field that will be used to get the best position
 * 
 * \return void
 */
void updateBestWantedPosition(node* wantedPosition, InterestField* interestField);

/**
 * \fn node *findNextPathNN(Entity *entity, node *endNode, dataType *data, NeuralNetwork *neuralNetwork)
 * \brief Returns the next path chosen by a given neural network
 *
 * \param entity : entity to move
 * \param endNode : position of destination
 * \param data : structure which define the kind of event we have to raise for interruption
 * \neuralNetwork : neural network used to take the decision
 *  
 * \return node* : The chosen path
 */
node *findNextPathNN(Entity *entity, node *endNode, dataType *data, NeuralNetwork *neuralNetwork);

/**
 * \fn node *findNextPathNN2(Entity *entity, dataType *data, float *output)
 * \brief returns the next path chosen by a given neural network
 *
 * \param entity : entity to move
 * \param data : structure which define the kind of event we have to raise for interruption
 * \param output : output of the neuralNetwork
 *  
 * \return node* : The chosen path
 */
node *findNextPathNN2(Entity *entity, dataType *data, float *output);

/**
 * \fn node *findNextPathGN(Entity *entity, node *endNode, dataType *data, LabelingWeights* labelingWeights)
 * \brief Returns the next path chosen by a given neural network
 *
 * \param entity : entity to move
 * \param endNode : position of destination
 * \param data : structure which define the kind of event we have to raise for interruption
 * \labelingWeights : genetic network used to take the decision
 *  
 * \return node* : The chosen path
 */
node *findNextPathGN(Entity *entity, node *endNode, dataType *data, LabelingWeights* labelingWeights);

/**
 * \fn node *findNextPathGN2(Entity *entity, dataType *data, LabelingWeights* labelingWeights)
 * \brief Returns the next path chosen by a given neural network
 *
 * \param entity : entity to move
 * \param data : structure which define the kind of event we have to raise for interruption
 * \labelingWeights : genetic network used to take the decision
 *  
 * \return node* : The chosen path
 */
node *findNextPathGN2(Entity *entity, dataType *data, LabelingWeights* labelingWeights);

/**
 * \fn node* labeling2(Entity *entity, int xEnd, int yEnd, Field *field, dataType *data)
 * \brief Function that returns the expected choice for the neural network
 *
 * \param entity : entity that is moving
 * \param xEnd, yEnd : coordinates of the destination
 * \param field : the complete field for the supervised learning
 * \param data : the structure we use to raise a flag for interruption
 * 
 * \return node* : The expected path 
 */
node *labeling2(Entity *entity, int xEnd, int yEnd, Field *field, dataType *data);

/**
 * \fn float labeling3(Field* fieldOfView, int xPosition, int yPosition, int xFinalPosition, int yFinalPosition, Entity* entity, LabelingWeights* labelingWeights)
 * \brief Function that returns the labeling of the points, will be used for labelisation
 *
 * \param Field* fieldOfView : a field of view
 * \param int xPosition : x coordinate of the center of the field of view
 * \param int yPosition : y coordinate of the center of the field of view
 * \param int xFinalPosition : x coordinate of the end point
 * \param int yFinalPosition : y coordinate of the end point
 * \param Entity* entity : the entity which uses the function to navigate (only use his coordiantes)
 * \param LabelingWeights* labelingWeights : Ze labeling weights
 * 
 * \return float : The labels of each point
 */
float labeling3(Field* fieldOfView, int xPosition, int yPosition, int xFinalPosition, int yFinalPosition, Entity* entity, LabelingWeights* labelingWeights);

/**
 * \fn float labeling4(Field* fieldOfView, int xPosition, int yPosition, Entity* entity, LabelingWeights* labelingWeights)
 * \brief Function that returns the labeling of the points, will be used for labelisation
 *
 * \param Field* fieldOfView : a field of view
 * \param int xPosition : x coordinate of the center of the field of view
 * \param int yPosition : y coordinate of the center of the field of view
 * \param Entity* entity : the entity which uses the function to navigate (only use his coordiantes)
 * \param LabelingWeights* labelingWeights : Ze labeling weights
 * 
 * \return float : the labels of each point
 */
float labeling4(Field* fieldOfView, int xPosition, int yPosition, Entity* entity, LabelingWeights* labelingWeights);

/**
 * \fn float* convertLabeling2(int fieldWidth, int fieldHeight, node *label)
 * \brief Function that converts a label2 into a valid output of neural network
 *
 * \param fieldWidth, fieldHeight : dimensions of the field
 * \param label : node found by function labeling2
 * 
 * \return float* : Transformed label
 */
float *convertLabeling2(int fieldWidth, int fieldHeight, node *label);

#endif
