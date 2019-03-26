/**
 * \file entity.h
 * \brief Prototype of the Entity management
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
#define RADIUS_VIEWPOINT (10)

//Header file for core functions
#include "pathfinding.h"

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
    //The neural network structure is to be added
} Entity;

//The structure used to store the inputs of the neural network
typedef struct InputNeuralNetwork
{
    float* data;
    int size;
} InputNeuralNetwork;

/**
 * \fn node* initialiseEntity(int x, int y, int visionRange)
 * \brief function that initialise an Entity. An Entity is used by the neural network to navigate in a Field
 *
 * \param x, y : the coordinate of the Entity
 * \param visionRange : the maximum distance at which the entity can see the Field
 * \param fieldWidth, fieldHeight : dimention of the field of the environment
 * \return Entity*
 */
Entity* initialiseEntity(int x, int y, int visionRange, int fieldWidth, int fieldHeight);

/**
 * \fn void initializeMentalMap(Entity *entity)
 * \brief function that initialize the mental map of an entity
 * 
 * \param *entity : the entity which have the mental map to be updated
 * \param width : width of the mental map we want to initialize
 * \param height : height of the mental map we want to initialize
 * \return void
 */
void initializeMentalMapEntity(Entity *entity, int width, int height);

/**
 * \fn void initialiseFieldOfView(Entity *entity);
 * \brief function that initialise the field of view of an entity
 *
 * \param *entity : the entity which have the field of view we want to update
 * \return void
 */
void initialiseFieldOfViewEntity(Entity *entity);

/**
 * \fn void destructEntity(Entity** entity)
 * \brief function used to free an Entity
 *
 * \param entity : the Entity to free
 * \return void
 */
void destructEntity(Entity** entity);

/**
 * \fn void destructFieldOfViewEntity(Entity *entity)
 * \brief function used to free a field of view of an Entity
 *
 * \param entity : the Entity that contains the field of view we want to free
 * \return void
 */
void destructFieldOfViewEntity(Entity *entity);

/**
 * \fn void showEntity(Entity* entity, SDL_Color color, int tileSize)
 * \brief function used to free an Entity
 *
 * \param entity : the Entity to display
 * \param renderer : the renderer used with the window. The renderer must have been set using SDL functions
 * \param color : the color which will be used to display the entity
 * \param tileSize : the size of one tile in the window
 * \return void
 */
void showEntity(Entity* entity, SDL_Renderer* renderer, SDL_Color color, int tileSize);

/*
 * \fn void updateMentalMapEntity(Entity *entity)
 * \brief function that update the mental map of an entity
 *
 * \param entity : the Entity to update
 * \return void
 */
void updateMentalMapEntity(Entity *entity);

/*
 * \fn void updateFieldOfViewEntity(Entity *entity)
 * \brief function that update the field of view of an entity
 *
 * \param entity : the Entity to update
 * \param field* : A pointer to the field on which we are based
 * \return void
 */
void updateFieldOfViewEntity(Field *aField, Entity *entity);

/**
 * \fn Field* getFieldOfViewFromMap(Field* map, int x, int y, int visionRange)
 * \brief function that return the field of view from a map on a given coordinate
 *
 * \param *mentalMap : a pointer to the map from which we want to extrat the field of view
 * \param x, y : the coordinate in the map from wihc we will extract the field of view
 * \param visionRange : the radius of the field of view
 * \return Field* : the field of view
 */
Field* getFieldOfViewFromMap(Field* map, int x, int y, int visionRange);

/*
 * \fn createInput(Field* mentalMap, int visionRange, int x, int y, int xEnd, int yEnd)
 * \brief function that create the inputs for the neural network based on the fieldOfView,
 *      the coordinate of the point we want to test and the coordinate the entity wants to get to
 *
 * \param fieldOfView* : A pointer to a field of the mental map of the entity
 * \param x, y : the coordinate of the point we want to test in the neural network
 * \param xEnd, yEnd : the coordinate the entity wants to get to
 *      
 * \return InputNeuralNetwork*
 */
InputNeuralNetwork* createInput(Field* fieldOfView, int x, int y, int xEnd, int yEnd);

/*
 * \fn void destructInput(InputNeuralNetwork** input)
 * \brief free a structure InputNeuralNetwork from the memory
 *
 * \param input** : a double pointer to the structure wwe want to free
 *      
 * \return void
 */
void destructInput(InputNeuralNetwork** input);

/**
 * \fn void updateInterestField(InterestField* interestField, NeuralNetwork* neuralNetwork, Field* mentalMap, int xEnd, int yEnd)
 * \brief function that change the values in an interest field according to the output of a trained neural network
 *
 * \param InterestField* interestField : the interest field that will be update
 * \param NeuralNetwork* neuralNetwork : the neural network that will give their values to the point in the interest field
 * \param Field* mentalMap : the mental map on wich the updated values will be based on
 * \param int xEnd : x coordinate of the ending point
 * \param int yEnd : y coordinate of the ending point
 * \param int visionRange : the vision range of the entity
 * \return void
 */
void updateInterestField(InterestField* interestField, NeuralNetwork* neuralNetwork, Field* mentalMap, int xEnd, int yEnd, int visionRange);

/**
 * \fn void updateInterestFieldCheat(InterestField* interestField, Field* mentalMap, int xEnd, int yEnd)
 * \brief function that change the values in an interest field according to the labelisation function
 *
 * \param InterestField* interestField : the interest field that will be update
 * \param Field* mentalMap : the mental map on wich the updated values will be based on
 * \param int xEnd : x coordinate of the ending point
 * \param int yEnd : y coordinate of the ending point
 * \param int visionRange : the vision range of the entity
 * \return void
 */
void updateInterestFieldCheat(InterestField* interestField, Field* mentalMap, int xEnd, int yEnd, int visionRange);

/**
 * \fn void updateBestWantedPosition(node* wantedPosition, InterestField* interestField)
 * \brief function that change the coordinate of a node to the best coordinate on the interest field.
 * This is use to get the position where our entity will go next.
 * Each time this function is called, the choosen position will be set to 0 (no interest)
 * will be used for labelisation
 *
 * \param node* wantedPosition : the node which will be updated to the best position on the interest field
 * \param InterestField* interestField : the interest field that will be used to get the best position
 * \return void
 */
void updateBestWantedPosition(node* wantedPosition, InterestField* interestField);

#endif
