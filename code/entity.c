#include "entity.h"


/**
 * \fn node* initialiseEntity(int x, int y, int visionRange)
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