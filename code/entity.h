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

//The structure entity used as a token which will be control by our neural network
typedef struct Entity
{
    int x;
    int y;
    int visionRange;
    //The Field of the mental map is to be added with his dimmension
    //The list of the valid visible position is to be added
    //The neural network structure is to be added
} Entity;

//Initialisation and destruction function
Entity* initialiseEntity(int x, int y, int visionRange);
void destructEntity(Entity** entity);