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
    return entity;
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
    int j;
    int diameter = (2*entity->visionRange + 1);

    entity->fieldOfView = NULL;

    entity->fieldOfView = (Point**)malloc(sizeof(Point*) * diameter);

    for(i = 0; i < diameter; i++)
    {
        entity->fieldOfView[i] = (Point*)malloc(sizeof(Point) * diameter);
    }

    for(i = 0; i < diameter; i++)
    {
        for(j = 0; j < diameter; j++)
        {
            entity->fieldOfView[j][i].x = j;
            entity->fieldOfView[j][i].y = i;
            entity->fieldOfView[j][i].pointValue = FOG;
        }
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
    destructFieldOfViewEntity(*entity);

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
    int diameter = (2*entity->visionRange + 1);

    if(entity->fieldOfView != NULL)
    {
        for(i = 0; i < diameter; i++)
        {
            free(entity->fieldOfView[i]);
        }

        free(entity->fieldOfView);
        entity->fieldOfView = NULL;
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
 *        field* : A pointer to the field on which we are based
 * \return void
 */
void updateFieldOfViewEntity(Field *aField, Entity *entity)
{
    int i,j;

    for(i = 0; i < (2*entity->visionRange + 1); i++)
    {
        for(j = 0; j < (2*entity->visionRange + 1); j++)
        {
            entity->fieldOfView[j][i].x = j;
            entity->fieldOfView[j][i].y = i;
            entity->fieldOfView[j][i].pointValue = FOG;
        }
    }

    // ================

    float angle;
    float stepAngle = 0.1; //tan(1 / entity->visionRange);
    float stepDistance = 0.5;
    float distance;
    bool aWall;

    for(angle = 0; angle < 2*M_PI; angle += stepAngle)
    {
        aWall = false;

        for(distance = 0; distance < entity->visionRange; distance += stepDistance)
        {
            if(!aWall)
            {
                float x = distance * cos(angle);
                float y = distance * sin(angle);

                if( ((entity->x + x) >= 0) && ((entity->x + x) < aField->width) && ((entity->y + y) >= 0) && ((entity->y + y) < aField->height) )
                {
                    int widthFieldOfView = entity->visionRange + 1 + x;
                    int heightFieldOfView = entity->visionRange + 1 + y;

                    if( (widthFieldOfView >= 0) && (widthFieldOfView < 2*entity->visionRange + 1) 
                        &&  (heightFieldOfView >= 0) && (heightFieldOfView < 2*entity->visionRange + 1) )
                    {
                        pointEnum currentValue;

                        entity->fieldOfView[widthFieldOfView][heightFieldOfView].x = (int)(entity->x + x);
                        entity->fieldOfView[widthFieldOfView][heightFieldOfView].y = (int)(entity->y + y);

                        currentValue = aField->data[(int)(entity->x + x)][(int)(entity->y + y)];
                        entity->fieldOfView[widthFieldOfView][heightFieldOfView].pointValue = currentValue;

                        if(currentValue == WALL)
                        {
                            aWall = true;
                        }
                    }
                }
            }
        }
    }
}
