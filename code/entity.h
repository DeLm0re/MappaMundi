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

//Header file for core functions
#include "core.h"

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
    Point *fieldOfView[50];
    Field *mentalMap;
    //The neural network structure is to be added
} Entity;

/**
 * \fn node* initialiseEntity(int x, int y, int visionRange)
 * \brief function that initialise an Entity. An Entity is used by the neural network to navigate in a Field
 *
 * \param x, y : the coordinate of the Entity
 * \param visionRange : the maximum distance at which the entity can see the Field
 * \return Entity*
 */
Entity* initialiseEntity(int x, int y, int visionRange);

/**
 * \fn void destructEntity(Entity** entity)
 * \brief function used to free an Entity
 *
 * \param entity : the Entity to free
 * \return void
 */
void destructEntity(Entity** entity);

/**
 * \fn void updateFieldOfViewEntity(Entity *entity)
 * \brief function that updates the field of view of an entity
 *
 * \param entity : the Entity to update
 * \return void
 */
void updateFieldOfViewEntity(Entity *entity);

/**
 * \fn void updateMentalMapEntity(Entity *entity)
 * \brief function that updates the mental map of an entity with it's actual field of view
 * 
 * \param entity : the Entity to update
 * \return void
 */
void updateMentalMapEntity(Entity *entity);

#endif