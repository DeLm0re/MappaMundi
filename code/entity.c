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
    int diameter = 2*entity->visionRange;

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
    int diameter = 2*entity->visionRange;

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
 *        field : the field on which we are based
 * \return void
 */
void updateFieldOfViewEntity(Field aField, Entity *entity)
{
    int i,j;
    int height;
    int width;
    int heightEntity = entity->y;
    int widthEntity = entity->x;
    int rayonCarre = entity->visionRange * entity->visionRange;
    float distanceCarre;

    for(i = 0; i < 2*entity->visionRange; i++)
    {
        for(j = 0; j < 2*entity->visionRange; j++)
        {
            entity->fieldOfView[j][i].x = j;
            entity->fieldOfView[j][i].y = i;
            entity->fieldOfView[j][i].pointValue = FOG;
        }
    }

    for(height = heightEntity - entity->visionRange; height < heightEntity + entity->visionRange; height++)
    {
        for(width = widthEntity - entity->visionRange; width < widthEntity + entity->visionRange; width++)
        {
            if( (height >= 0) && (height < aField.height) && (width >= 0) && (width < aField.width) )
            {
                distanceCarre = (height - heightEntity)*(height - heightEntity) + (width - widthEntity)*(width - widthEntity);
                    
                    if(distanceCarre < rayonCarre)
                    {
                        if(behindAWall(aField, entity, height, width) != true)
                        {
                            entity->fieldOfView[width][height].pointValue = aField.data[width][height];
                        }
                    }
            }
        }
    }
}

/*
 * \fn bool behindAWall(Field aField, Entity *entity, int heigh, int width)
 * \brief function that says if a point of our field is behind a wall or not from a POV of the entity
 *
 * \param entity : the Entity from where we have the POV
 *        field : the field on which we are based
 *        height : the heigh of the point we want to study
 *        width : the width of the point we want to
 * \return bool : true if the point is behind a wall, false if not
 */
bool behindAWall(Field aField, Entity *entity, int height, int width)
{
    float a;
    float b;
    float y;
    float x;
    int heightEntity = entity->y;
    int widthEntity = entity->x;
    float step = 0.1;

    a = (height - heightEntity)/(width - widthEntity);
    
    //b = ya - a*xa
    b = heightEntity - a * widthEntity;

    //If x entity > x of the point
    if(widthEntity >= width)
    {
        for(x = widthEntity; x >= width; x = x - step)
        {
            y = a * x + b;

            if( (y != height) && (x != width) )
            {
                if(aField.data[(int)x][(int)y] == WALL)
                {
                    return(true);
                }
            }
        }
    }

    //If x entity < x of the point
    if(widthEntity < width)
    {
        for(x = widthEntity; x <= width; x = x + step)
        {
            y = a * x + b;

            if( (y != height) && (x != width) )
            {
                if(aField.data[(int)x][(int)y] == WALL)
                {
                    return(true);
                }
            }
        }
    }

    //If x entity = x of the point
    if(width == widthEntity)
    {
        //If y entity > y of the point
        if(heightEntity >= height)
        {
            for(y = heightEntity; y >= height; y = y - step)
            {
                if( (y != height) && (x != width) )
                {
                    if(aField.data[(int)width][(int)y] == WALL)
                    {
                        return(true);
                    }
                }
            }
        }
        //If y entity < y of the point
        if(heightEntity < height)
        {
            for(y = heightEntity; y <= height; y = y + step)
            {
                if( (y != height) && (x != width) )
                {
                    if(aField.data[(int)width][(int)y] == WALL)
                    {
                        return(true);
                    }
                }
            }
        }
    }

    return(false);
}