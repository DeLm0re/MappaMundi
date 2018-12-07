/**
 * \file entity.c
 * \brief The Entity management
 * \author Louis Schuck
 * \version 0.1
 * \date 30/11/2018
 *
 * Module that contain the functions and the structure used by the neural network to navigate in the Field
 *
 */

//Header files containing the prototypes
#include "entity.h"

/**
 * \fn Entity* initialiseEntity(int x, int y, int visionRange)
 * \brief function that initialise an Entity. An Entity is used by the neural network to navigate in a Field
 *
 * \param x, y : the coordinate of the Entity
 * \param visionRange : the maximum distance at which the entity can see the Field
 * \return Entity*
 */
Entity* initialiseEntity(int x, int y, int visionRange)
{
    Entity* entity = malloc(sizeof(Entity));
    entity->x = x;
    entity->y = y;
    entity->visionRange = visionRange;
    initialiseFieldOfViewEntity(entity);
}

/**
 * \fn void* initialiseFieldOfView(Entity *entity);
 * \brief function that initialise the field of view of an entity
 * 
 * \param *entity : the entity which have the field of view we want to update
 * \return void
 */
void initialiseFieldOfViewEntity(Entity *entity)
{
    int i;

    for(i = 0; i < MAX_VIEWPOINT; i++)
    {
        entity->fieldOfView[i] = NULL;
    }
}

/**
 * \fn void destructEntity(Entity** entity)
 * \brief function used to free an Entity
 *
 * \param entity : the Entity to free
 * \return void
 */
void destructEntity(Entity** entity)
{
    if(entity != NULL)
    {
        if(*entity != NULL)
        {
            free(*entity);
            *entity = NULL;
        }
    }
}

/**
 * \fn void destructFieldOfViewEntity(Entity *entity)
 * \brief function used to free a field of view of an Entity
 *
 * \param entity : the Entity that contains the field of view we want to free
 * \return void
 */
void destructFieldOfViewEntity(Entity *entity)
{
    int i;

    for(i = 0; i < MAX_VIEWPOINT; i++)
    {
        if(entity->fieldOfView[i] != NULL)
        {
            free(entity->fieldOfView[i]);
        }
    }
}

/**
 * \fn void updateFieldOfViewEntity(Entity *entity)
 * \brief function that update the field of view of an entity
 *
 * \param entity : the Entity to update
 * \return void
 */
void updateFieldOfViewEntity(Entity *entity)
{
    destructFieldOfViewEntity(entity);

    int i;
    int j;
    int heighEntity = entity->y;
    int widthEntity = entity->x;
    int visionRange = entity->visionRange;
    int heighDifference;
    int widthDifference;
    int hypothenus;
    int index = 0;

    for(i = heighEntity - visionRange; i < heighEntity + visionRange; i++)
    {
        for(j = widthEntity - visionRange; j < widthEntity + visionRange; j++)
        {
            heighDifference = abs(heighEntity - i);
            widthDifference = abs(widthEntity - j);

            hypothenus = sqrt( (heighDifference* heighDifference) + (widthDifference * widthDifference) );

            if(hypothenus <= visionRange)
            {
                entity->fieldOfView[index] = (Point*)malloc(sizeof(Point));
                entity->fieldOfView[index]->x = j;
                entity->fieldOfView[index]->y = i;
                entity->fieldOfView[index]->pointValue = Field[i][j];
            }
        }
    }
}
