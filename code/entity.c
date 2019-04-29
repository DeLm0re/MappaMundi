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
 * \fn Entity* initialiseEntity(int x, int y, int visionRange, int fieldWidth, int fieldHeight)
 * \brief function that initialise an Entity. An Entity is used by the neural network to navigate in a Field
 *
 * \param x, y : the coordinates of the Entity
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
 * \fn initializeMentalMapEntity(Entity *entity, int width, int height)
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
 * \fn void initialiseFieldOfViewEntity(Entity *entity)
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
 * \fn void showEntity(Entity* entity, SDL_Renderer* renderer, SDL_Color color, int tileSize)
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
            entity->x*tileSize + 1,
            entity->y*tileSize + 1,
            tileSize-2,
            tileSize-2}));
	}
}

/**
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
                pointEnum mentalMapPoint = entity->mentalMap->data[pointWidth][pointHeight];

                //Check if the old value of the point was FOG before updating it
                if(mentalMapPoint == FOG)
                {
                    entity->mentalMap->data[pointWidth][pointHeight] = pointValue; 
                }
                
                entity->mentalMap->data[entity->x][entity->y] = VISITED;
            }
        }
    }
}

/**
 * \fn void updateFieldOfViewEntity(Field *aField, Entity *entity)
 * \brief function that update the field of view of an entity
 *
 * \param entity : the Entity to update
 * \param field* : A pointer to the field on which we are based
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
    float stepDistance = 0.4;
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
                    int widthFieldOfView = entity->visionRange + x;
                    int heightFieldOfView = entity->visionRange + y;

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

/**
 * \fn Field* getFieldOfViewFromMap(Field* map, int x, int y, int visionRange)
 * \brief function that return the field of view from a map on a given coordinate
 *
 * \param *mentalMap : a pointer to the map from which we want to extrat the field of view
 * \param x, y : the coordinates in the map from wihc we will extract the field of view
 * \param visionRange : the radius of the field of view
 * \return Field* : the field of view
 */
Field* getFieldOfViewFromMap(Field* map, int x, int y, int visionRange)
{
    Field* fieldOfView = NULL;
    // If the mental map exist and if the vision range is greater than 0
    if (map != NULL && visionRange > 0)
    {
        // We initialize the foeld of view
        fieldOfView = initialiseField(visionRange*2 + 1, visionRange*2 + 1, UNDEFINED);
        int radiusSquare = visionRange * visionRange; // use to know the maximum distance a tile can have to be in the vision range
        float distanceSquare; // use to know the distance of a tile from the x and y coordinate
        //For each tile that could be in the vision range (a square around the x and y coordinate)
        int width, height;
        for(width = x - visionRange; width <= x + visionRange; width++)
        {
            for(height = y - visionRange; height <= y + visionRange; height++)
            {
                //We calculate his distance to the x and y coordinate
                distanceSquare = pow(width - x, 2) + pow(height - y, 2);
                //If it is in the vision range
                if(distanceSquare < radiusSquare)
                {
                    // If it doesn't go out of bounds
                    if (0 < width && width < map->width && 0 < height && height < map->height)
                    {
                        // We add it to the mental map
                        fieldOfView->data[width - x + visionRange][height - y + visionRange] = map->data[width][height];
                    }
                }
            }
        }
    }
    // Then, we return the field of view
    return fieldOfView;
}

/**
 * \fn createInput(Field* fieldOfView, int x, int y, int xEnd, int yEnd)
 * \brief function that create the inputs for the neural network based on the mental map, the vision range,
 *      the coordinate of the point we want to test and the coordinate the entity wants to get to
 *
 * \param mentalMap* : A pointer to the mental map of the entity
 * \param visionRange : the vision range of the entity
 * \param x, y : the coordinate of the point we want to test in the neural network
 * \param xEnd, yEnd : the coordinate the entity wants to get to
 *      
 * \return InputNeuralNetwork*
 */
InputNeuralNetwork* createInput(Field* fieldOfView, int x, int y, int xEnd, int yEnd)
{
    InputNeuralNetwork* input = NULL;
    if (fieldOfView != NULL)
    {
        // If the field of view is a square and if the field of view has a correct format
        if (fieldOfView->height == fieldOfView->width && fieldOfView->height%2 == 1)
        {
            // We calculate the vision range from the length of the field fo view
            int visionRange = (fieldOfView->height - 1)/2;
            //We initialize the input
            input = (InputNeuralNetwork*) malloc(sizeof(InputNeuralNetwork));
            //We calculate his size which is the total tile in the vision range plus one for each coordinates
            input->size = surface2DCircle(visionRange) + 1;
            //We initialize the input's data
            input->data = (float*) malloc(sizeof(float)*input->size);
            int radiusSquare = visionRange * visionRange; // use to know the maximum distance a tile can have to be in the vision range
            int distanceSquare; // use to know the distance of a tile from the x and y coordinate
            int dataIndex = 0; // use to navigate through the input's data
            //For each tile that could be in the vision range (a square around the x and y coordinate)
            int width, height;
            for(width = -visionRange; width <= visionRange; width++)
            {
                for(height = -visionRange; height <= visionRange; height++)
                {
                    //We calculate his distance to the center coordinate
                    distanceSquare = width*width + height*height;
                    //If it is in the vision range
                    if (distanceSquare < radiusSquare)
                    {
                        //We add it to the inputs
                        input->data[dataIndex] = neuroneTransferFunction(fieldOfView->data[width + visionRange][height + visionRange]);
                        //We go to the next input's data
                        dataIndex++;
                    }
                }
            }
            //Once the map is fully added, we add the 4 cordinates
            input->data[dataIndex] = pow(x-xEnd, 2) + pow(y-yEnd, 2);
        }
    }
    // We return the input
    return input;
}

/**
 * \fn void destructInput(InputNeuralNetwork** input)
 * \brief free a structure InputNeuralNetwork from the memory
 *
 * \param InputNeuralNetwork** input : a double pointer to the structure wwe want to free
 *      
 * \return void
 */
void destructInput(InputNeuralNetwork** input)
{
    if (input != NULL)
    {
        if (*input != NULL)
        {
            free((*input)->data);
            free(*input);
            *input = NULL;
        }
    }
}

/**
 * \fn void updateInterestField(InterestField* interestField, NeuralNetwork* neuralNetwork, Field* mentalMap, int xEnd, int yEnd, int visionRange)
 * \brief function that change the values in an interest field according to the output of a trained neural network
 * will be used for labelisation
 *
 * \param InterestField* interestField : the interest field that will be update
 * \param NeuralNetwork* neuralNetwork : the neural network that will give their values to the point in the interest field
 * \param Field* mentalMap : the mental map on wich the updated values will be based on
 * \param int xEnd : x coordinate of the ending point
 * \param int yEnd : y coordinate of the ending point
 * \return void
 */
void updateInterestField(InterestField* interestField, NeuralNetwork* neuralNetwork, Field* mentalMap, int xEnd, int yEnd, int visionRange)
{
    if (interestField != NULL && neuralNetwork != NULL && mentalMap != NULL)
    {
        int width, height;
        for(width = 0; width < interestField->width; width++)
        {
            for(height = 0; height < interestField->height; height++)
            {
                if (mentalMap->data[width][height] != EMPTY)
                {
                    interestField->data[width][height] = 0;
                }
                else
                {
                    Field* fieldOfView = getFieldOfViewFromMap(mentalMap, width, height, visionRange);
                    InputNeuralNetwork* inputs = createInput(fieldOfView, width, height, xEnd, yEnd);
                    float* outputs = getOutputOfNeuralNetwork(neuralNetwork, inputs->data);
                    
                    interestField->data[width][height] = outputs[0];
                    
                    destructField(&fieldOfView);
                    destructInput(&inputs);
                    free(outputs);
                }
            }
        }
    }
}

/**
 * \fn void updateInterestField2(InterestField* interestField, Field* mentalMap, int xEnd, int yEnd, int visionRange, LabelingWeights* labelingWeights)
 * \brief function that change the values in an interest field according to the labelisation function
 *
 * \param InterestField* interestField : the interest field that will be update
 * \param Field* mentalMap : the mental map on wich the updated values will be based on
 * \param int xEnd : x coordinate of the ending point
 * \param int yEnd : y coordinate of the ending point
 * \param int visionRange : the vision range of the entity
 * \param LabelingWeights* labelingWeights : Ze labeling weights

 * \return void
 */
void updateInterestField2(InterestField* interestField, Field* mentalMap, int xEnd, int yEnd, int visionRange, LabelingWeights* labelingWeights)
{
    if (interestField != NULL && mentalMap != NULL)
    {
        int width, height;
        for(width = 0; width < interestField->width; width++)
        {
            for(height = 0; height < interestField->height; height++)
            {
                Field* fieldOfView = getFieldOfViewFromMap(mentalMap, width, height, visionRange);
                
                interestField->data[width][height] = labeling3(fieldOfView, width, height, xEnd, yEnd, labelingWeights);
                
                destructField(&fieldOfView);
            }
        }
    }
}

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
void updateBestWantedPosition(node* wantedPosition, InterestField* interestField)
{
    if (wantedPosition != NULL && interestField != NULL)
    {
        float bestPoint = 0;
        wantedPosition->x = 0;
        wantedPosition->y = 0;
        int width, height;
        for(width = 0; width < interestField->width; width++)
        {
            for(height = 0; height < interestField->height; height++)
            {
                if (interestField->data[width][height] > bestPoint)
                {
                    bestPoint = interestField->data[width][height];
                    wantedPosition->x = width;
                    wantedPosition->y = height;
                }
            }
        }
        interestField->data[wantedPosition->x][wantedPosition->y] = 0;
    }
}
