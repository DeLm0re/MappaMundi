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
 * \param fieldWidth, fieldHeight : dimention of the field of the environment
 * \return Entity*
 */
Entity* initialiseEntity(int x, int y, int visionRange, int fieldWidth, int fieldHeight)
{
    Entity* entity = malloc(sizeof(Entity));
    entity->x = x;
    entity->y = y;
    entity->visionRange = visionRange;
    
    initialiseFieldOfViewEntity(entity);
    initializeMentalMapEntity(entity, fieldWidth, fieldHeight);
    return entity;
}

/**
 * \fn void initializeMentalMap(Entity *entity)
 * \brief function that initialize the mental map of an entity
 * 
 * \param *entity : the entity which have the mental map to be updated
 * \param width : width of the mental map we want to initialize
 * \param height : height of the mental map we want to initialize
 * \return void
 */
void initializeMentalMapEntity(Entity *entity, int width, int height)
{
    entity->mentalMap = initialiseField(width, height, FOG);
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
    destructField(&((*entity)->mentalMap));

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
 * \fn void updateMentalMapEntity(Entity *entity)
 * \brief function that update the mental map of an entity
 *
 * \param entity : the Entity to update
 * \return void
 */
void updateMentalMapEntity(Entity *entity)
{
    int widthIndex, heightIndex;
    for(widthIndex = 0; widthIndex < 2*entity->visionRange + 1; widthIndex++)
    {
        for(heightIndex = 0; heightIndex < 2*entity->visionRange + 1; heightIndex++)
        {
            int pointWidth = entity->fieldOfView[widthIndex][heightIndex].x;
            int pointHeight = entity->fieldOfView[widthIndex][heightIndex].y;
            pointEnum pointValue = entity->fieldOfView[widthIndex][heightIndex].pointValue;

            if(pointWidth >= 0 && pointWidth < entity->mentalMap->width && 
                pointHeight >= 0 && pointHeight < entity->mentalMap->height)
            {
                entity->mentalMap->data[pointWidth][pointHeight] = pointValue; 
            }
        }
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
