/**
 * \file entity.h
 * \brief Declaration of the functions prototypes
 * \author Louis Schuck
 * \version 0.1
 * \date 29/11/2018
 *
 * Module that contain the prototypes of the functions which are used to deal with the entity
 *
 */

//Header file
#include "prototype.h"

//The structure entity used as a token which will be control by our neural network
typedef struct Entity
{
    int x;
    int y;
    int visionRange;
    Point* fieldOfView[50];
    //The Field of the mental map is to be added with his dimmension
    //The list of the valid visible position is to be added
    //The neural network structure is to be added
} Entity;

//The structure point used as a point of a field which can be a wall, empty or fog
typedef struct Point
{
    int x;
    int y;
    pointEnum pointValue;
} Point;

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
 * \brief function that update the field of view of an entity
 *
 * \param entity : the Entity to update
 * \return void
 */
void updateFieldOfViewEntity(Entity *entity);