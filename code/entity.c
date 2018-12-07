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
    return entity;
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
 * \fn void showEntity(Entity* entity, SDL_Color color, int tileSize)
 * \brief function used to free an Entity
 *
 * \param entity : the Entity to display
 * \param renderer : the renderer used with the window. The renderer must have been set using SDL functions
 * \param color : the color which will be used to display the entity
 * \param tileSize : the size of one tile in the window
 * \return void
 */
void showEntity(Entity* entity, SDL_Renderer* renderer, SDL_Color color, int tileSize)
{
    //If the size of the tile is large enought to draw a rectangle
	if (tileSize > 3)
	{
		//We set the drawing color
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        //We draw a rectangle at the correct coordiante
        SDL_RenderFillRect(renderer, &((SDL_Rect) {
            entity->x*tileSize + 2, 
            entity->y*tileSize + 2, 
            tileSize-4, 
            tileSize-4}));
	}
}

/*
 * \fn void updateFieldOfViewEntity(Entity *entity)
 * \brief function that update the field of view of an entity
 *
 * \param entity : the Entity to update
 * \return void
 */
void updateFieldOfViewEntity(Entity *entity)
{
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
