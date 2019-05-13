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
 * \fn createInputNN2(Field* field, int entityX, int entityY, int xEnd, int yEnd)
 * \brief function that create the inputs for the neural network based on a field,
 *      the coordinate of the entity and the coordinate the entity wants to get to
 *
 * \param field : A pointer to the field we want to convert
 * \param visionRange : the vision range of the entity
 * \param entityX, entityY : the coordinate of the entity
 * \param xEnd, yEnd : the coordinate the entity wants to get to
 *      
 * \return InputNeuralNetwork*
 */
float* createInputNN2(Field* field, int entityX, int entityY, int xEnd, int yEnd)
{
    int widthIndex, heightIndex;
    int inputIndex = 0;
    float *input = (float*)malloc(( 4 + field->height * field->width) * sizeof(float));
    for(widthIndex = 0; widthIndex < field->width; widthIndex++)
    {
        for(heightIndex = 0; heightIndex < field->height; heightIndex++)
        {
            if(field->data[widthIndex][heightIndex] == EMPTY || field->data[widthIndex][heightIndex] == VISITED)
                input[inputIndex] = 1;
            else
                input[inputIndex] = 0;
            inputIndex++;
        }
    }

    input[inputIndex] = nmap(entityX, 0, field->width - 1, 0, 1);
    input[inputIndex + 1] = nmap(entityY, 0, field->height - 1, 0, 1);
    input[inputIndex + 2] = nmap(xEnd, 0, field->width - 1, 0, 1);
    input[inputIndex + 3] = nmap(yEnd, 0, field->height - 1, 0, 1);

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
 * \fn void updateInterestField2(InterestField* interestField, int xEnd, int yEnd, Entity* entity, LabelingWeights* labelingWeights)
 * \brief function that change the values in an interest field according to the labelisation function
 *
 * \param InterestField* interestField : the interest field that will be update
 * \param int xEnd : x coordinate of the ending point
 * \param int yEnd : y coordinate of the ending point
 * \param Entity* entity : the entity on which we base the update of the interest field
 * \param LabelingWeights* labelingWeights : Ze labeling weights

 * \return void
 */
void updateInterestField2(InterestField* interestField, int xEnd, int yEnd, Entity* entity, LabelingWeights* labelingWeights)
{
    if (interestField != NULL && entity != NULL && labelingWeights != NULL)
    {
        int width, height;
        for(width = 0; width < interestField->width; width++)
        {
            for(height = 0; height < interestField->height; height++)
            {
                Field* fieldOfView = getFieldOfViewFromMap(entity->mentalMap, width, height, entity->visionRange);
                
                interestField->data[width][height] = labeling3(fieldOfView, width, height, xEnd, yEnd, entity, labelingWeights);
                
                destructField(&fieldOfView);
            }
        }
    }
}

/**
 * \fn void updateInterestField3(InterestField* interestField, Entity* entity, LabelingWeights* labelingWeights)
 * \brief function that change the values in an interest field according to the labelisation function
 *
 * \param InterestField* interestField : the interest field that will be update
 * \param Entity* entity : the entity on which we base the update of the interest field
 * \param LabelingWeights* labelingWeights : Ze labeling weights

 * \return void
 */
void updateInterestField3(InterestField* interestField, Entity* entity, LabelingWeights* labelingWeights)
{
    if (interestField != NULL && entity != NULL && labelingWeights != NULL)
    {
        int width, height;
        for(width = 0; width < interestField->width; width++)
        {
            for(height = 0; height < interestField->height; height++)
            {
                Field* fieldOfView = getFieldOfViewFromMap(entity->mentalMap, width, height, entity->visionRange);
                
                interestField->data[width][height] = labeling4(fieldOfView, width, height, entity, labelingWeights);
                
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
        float bestPoint = -INFINITY;
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
        interestField->data[wantedPosition->x][wantedPosition->y] = -INFINITY;
    }
}

/**
 * \fn node *findNextPathNN(Entity *entity, node *startNode, node *endNode, dataType *data, NeuralNetwork *neuralNetwork)
 * \brief returns the next path chosen by a given neural network
 *
 * \param entity : entity to move
 * \param endNode : position of destination
 * \param data : structure which define the kind of event we have to raise for interruption
 * \neuralNetwork : neural network used to take the decision
 *  
 * \return node*
 */
node *findNextPathNN(Entity *entity, node *endNode, dataType *data, NeuralNetwork *neuralNetwork)
{
    node *startNode = initNode(entity->x, entity->y, 0, 0);
    //We initialize an interest field
    InterestField* interestField = initialiseInterestField(entity->mentalMap->width, entity->mentalMap->height);

    //We update each values of the interest field with what our neural network think
    updateInterestField(interestField, neuralNetwork, entity->mentalMap, endNode->x, endNode->y, entity->visionRange);
    
    //We set a default wanted node
    node *wantedPosition = cpyNode(endNode);
    
    //Use to store the path found by the pathfinding
    node* path = NULL;
    //We try to find a path
    while((path == startNode || path == NULL) && !data->endEvent)
    {
        destructNodes(&path);
        //We try to find a path
        path = findPathFromStartEnd(startNode, wantedPosition, entity->mentalMap, &(data->endEvent));
        //If we haven't find a path
        if ((path == startNode || path == NULL))
        {
            //We change our wanted node to the best position found by the neural network
            updateBestWantedPosition(wantedPosition, interestField);
        }
    }
    destructInterestField(&interestField);
    destructNodes(&wantedPosition);

    return path;
}

/**
 * \fn node *findNextPathNN2(Entity *entity, dataType *data, NeuralNetwork *neuralNetwork)
 * \brief returns the next path chosen by a given neural network
 *
 * \param entity : entity to move
 * \param data : structure which define the kind of event we have to raise for interruption
 * \param output : output of the neuralNetwork
 *  
 * \return node*
 */
node *findNextPathNN2(Entity *entity, dataType *data, float *output)
{
    //Find the max value in the outputs
    float max = -INFINITY;
    int index, indexMax = 0;
    int fieldWidth = entity->mentalMap->width;
    int fieldHeight = entity->mentalMap->height;
    for(index = 0; index < fieldWidth * fieldHeight; index++)
    {
        if(output[index] > max)
        {
            max = output[index];
            indexMax = index;
        }
    }

    int outputX = indexMax / fieldHeight;
    int outputY = indexMax % fieldHeight;

    node *startNode = initNode(entity->x, entity->y, 0, 0);
    node *endnode = initNode(outputX, outputY, 0, 0);
    
    //We set a default wanted node
    node *wantedPosition = cpyNode(endnode);

    //Use to store the path found by the pathfinding
    node* path = NULL;
    //We try to find a path
    while((path == startNode || path == NULL) && !data->endEvent)
    {   
        destructNodes(&path);
        //We try to find a path
        path = findPathFromStartEnd(startNode, wantedPosition, entity->mentalMap, &(data->endEvent));
        //If we haven't find a path
        if ((path == startNode || path == NULL))
        {            
            //We change our wanted node to the nearest position available
            node *tmp = nearestNode(entity->mentalMap, wantedPosition->x, wantedPosition->y);
            if(tmp->x == wantedPosition->x && tmp->y == wantedPosition->y)
            {
                destructNodes(&tmp);
                break;
            }
            else
            {
                destructNodes(&wantedPosition);
                wantedPosition = tmp;
            }
        }
    }
    return path;
}

/**
 *  \fn node *findNextPathGN(Entity *entity, node *startNode, node *endNode, dataType *data, LabelingWeights* labelingWeights)
 * \brief returns the next path chosen by a given neural network
 *
 * \param entity : entity to move
 * \param endNode : position of destination
 * \param data : structure which define the kind of event we have to raise for interruption
 * \labelingWeights : genetic network used to take the decision
 *  
 * \return node*
 */
node *findNextPathGN(Entity *entity, node *endNode, dataType *data, LabelingWeights* labelingWeights)
{
    node *startNode = initNode(entity->x, entity->y, 0, 0);
    //We initialize an interest field
    InterestField* interestField = initialiseInterestField(entity->mentalMap->width, entity->mentalMap->height);

    //We update each values of the interest field with what our neural network think
    updateInterestField2(interestField, endNode->x, endNode->y, entity, labelingWeights);
    
    //We set a default wanted node
    node *wantedPosition = cpyNode(endNode);
    
    //Use to store the path found by the pathfinding
    node* path = NULL;
    //We try to find a path
    while((path == startNode || path == NULL) && !data->endEvent)
    {
        destructNodes(&path);
        //We try to find a path
        path = findPathFromStartEnd(startNode, wantedPosition, entity->mentalMap, &(data->endEvent));
        //If we haven't find a path
        if ((path == startNode || path == NULL))
        {
            //We change our wanted node to the best position found by the neural network
            updateBestWantedPosition(wantedPosition, interestField);
        }
    }
    destructInterestField(&interestField);
    destructNodes(&wantedPosition);

    return path;
}

/**
 * \fn node *findNextPathGN2(Entity *entity, node *startNode, dataType *data, LabelingWeights* labelingWeights)
 * \brief returns the next path chosen by a given neural network
 *
 * \param entity : entity to move
 * \param data : structure which define the kind of event we have to raise for interruption
 * \labelingWeights : genetic network used to take the decision
 *  
 * \return node*
 */
node *findNextPathGN2(Entity *entity, dataType *data, LabelingWeights* labelingWeights)
{
    node *startNode = initNode(entity->x, entity->y, 0, 0);
    //We initialize an interest field
    InterestField* interestField = initialiseInterestField(entity->mentalMap->width, entity->mentalMap->height);

    //We update each values of the interest field with what our neural network think
    updateInterestField3(interestField, entity, labelingWeights);
    
    //We set a default wanted node
    node *wantedPosition = initNode(0, 0, 0, 0);
    
    //Use to store the path found by the pathfinding
    node* path = NULL;
    //We try to find a path
    while((path == startNode || path == NULL) && !data->endEvent)
    {
        if ((path == startNode || path == NULL))
        {
            //We change our wanted node to the best position found by the neural network
            updateBestWantedPosition(wantedPosition, interestField);
        }
        
        destructNodes(&path);
        //We try to find a path
        path = findPathFromStartEnd(startNode, wantedPosition, entity->mentalMap, &(data->endEvent));
        //If we haven't find a path
       
    }
    destructInterestField(&interestField);
    destructNodes(&wantedPosition);

    return path;
}

/**
 * \fn node* labeling2(Entity *entity, int xEnd, int yEnd, Field *field, dataType *data)
 * \brief function that returns the expected choice for the neural network
 *
 * \param entity : entity that is moving
 * \param xEnd, yEnd : coordinates of the destination
 * \param field : the complete field for the supervised learning
 * \param data : the structure we use to raise a flag for interruption
 * 
 * \return node*
 */
node *labeling2(Entity *entity, int xEnd, int yEnd, Field *field, dataType *data)
{
    node *startNode = initNode(entity->x, entity->y, 0, 0);
    node *endNode = initNode(xEnd, yEnd, 0, 0);
    node *completePath = findPathFromStartEnd(startNode, endNode, field, &data->endEvent);
    node *destination = popNode(&completePath);

    bool endLoop = false;
    while(!(destination->x == endNode->x && destination->y == endNode->y) && endLoop == false)
    {
        int xPath = completePath->x;
        int yPath = completePath->y;
        pointEnum tileValue = entity->mentalMap->data[xPath][yPath];

        if (tileValue != EMPTY && tileValue != VISITED)
            endLoop = true;
        else
        {
            destructNodes(&destination);
            destination = popNode(&completePath);
        }
    }
    destructNodes(&completePath);
    return destination;
}

/**
 * \fn float labeling3(Field* fieldOfView, int xPosition, int yPosition, int xFinalPosition, int yFinalPosition, Entity* entity, LabelingWeights* labelingWeights)
 * \brief function that returns the labeling of the points
 * will be used for labelisation
 *
 * \param Field* fieldOfView : a field of view
 * \param int xPosition : x coordinate of the center of the field of view
 * \param int yPosition : y coordinate of the center of the field of view
 * \param int xFinalPosition : x coordinate of the end point
 * \param int yFinalPosition : y coordinate of the end point
 * \param Entity* entity : the entity which uses the function to navigate (only use his coordiantes)
 * \param LabelingWeights* labelingWeights : Ze labeling weights
 * \return float
 */
float labeling3(Field* fieldOfView, int xPosition, int yPosition, int xFinalPosition, int yFinalPosition, Entity* entity, LabelingWeights* labelingWeights)
{
    float emptyPoint = 0;
    float wallPoint = 0;
    float fogPoint = 0;
    float visitedPoint = 0;
    float dist = 0;
    float avgDistEmpty = 0;
    float avgDistWall = 0;
    float avgDistFog = 0;
    float avgDistVisited = 0;
    float distFromEntity = 0;
    float value = 0;
    float centerPointx = (fieldOfView->width-1)/2;
    float centerPointy = (fieldOfView->height-1)/2;


    if (fieldOfView->data[(int)centerPointx][(int)centerPointy] != EMPTY) 
    {
        return -INFINITY;
    }
    
    for(int width = 0; width < fieldOfView->width; width++)
    {
        for(int height = 0; height < fieldOfView->height; height++)
        {
            switch (fieldOfView->data[width][height])
            {
                case EMPTY:
                    avgDistEmpty += sqrt(pow(width-centerPointx,2) + pow(height-centerPointy,2));
                    emptyPoint++;
                    break;
                case WALL:
                    avgDistWall += sqrt(pow(width-centerPointx,2) + pow(height-centerPointy,2));
                    wallPoint++;
                    break;
                case FOG:
                    if(isVisibleFrom(fieldOfView, (fieldOfView->width-1)/2, (fieldOfView->height-1)/2, width, height))
                    {
                        avgDistFog += sqrt(pow(width-centerPointx,2) + pow(height-centerPointy,2));
                        fogPoint++;
                    }
                    break;
                case VISITED:
                    avgDistVisited += sqrt(pow(width-centerPointx,2) + pow(height-centerPointy,2));
                    visitedPoint++;
                    break;
            }
        }
    }

    dist = sqrt(pow(xFinalPosition-xPosition,2) + pow(yFinalPosition-yPosition,2));
    distFromEntity = sqrt(pow(entity->x-xPosition,2) + pow(entity->y-yPosition,2));

    avgDistEmpty /= emptyPoint;
    avgDistWall /= wallPoint;
    avgDistFog /= fogPoint;
    avgDistVisited /= visitedPoint;

    value = dist*labelingWeights->weights[DIST]+
            emptyPoint*labelingWeights->weights[NB_EMPTY]+
            wallPoint*labelingWeights->weights[NB_WALL]+
            fogPoint*labelingWeights->weights[NB_FOG]+
            visitedPoint*labelingWeights->weights[NB_VISITED]+
            avgDistEmpty*labelingWeights->weights[AVG_DIST_EMPTY]+
            avgDistWall*labelingWeights->weights[AVG_DIST_WALL]+
            avgDistFog*labelingWeights->weights[AVG_DIST_FOG]+
            avgDistVisited*labelingWeights->weights[AVG_DIST_VISITED]+
            distFromEntity*labelingWeights->weights[DIST_FROM_ENTITY];

    return value;
}

/**
 * \fn float labeling4(Field* fieldOfView, int xPosition, int yPosition, Entity* entity, LabelingWeights* labelingWeights)
 * \brief function that returns the labeling of the points
 * will be used for labelisation
 *
 * \param Field* fieldOfView : a field of view
 * \param int xPosition : x coordinate of the center of the field of view
 * \param int yPosition : y coordinate of the center of the field of view
 * \param Entity* entity : the entity which uses the function to navigate (only use his coordiantes)
 * \param LabelingWeights* labelingWeights : Ze labeling weights
 * \return float
 */
float labeling4(Field* fieldOfView, int xPosition, int yPosition, Entity* entity, LabelingWeights* labelingWeights)
{
    float emptyPoint = 0;
    float wallPoint = 0;
    float fogPoint = 0;
    float visitedPoint = 0;
    float avgDistEmpty = 0;
    float avgDistWall = 0;
    float avgDistFog = 0;
    float avgDistVisited = 0;
    float distFromEntity = 0;
    float value = 0;
    float centerPointx = (fieldOfView->width-1)/2;
    float centerPointy = (fieldOfView->height-1)/2;


    if (fieldOfView->data[(int)centerPointx][(int)centerPointy] != EMPTY) 
    {
        return -INFINITY;
    }
    
    for(int width = 0; width < fieldOfView->width; width++)
    {
        for(int height = 0; height < fieldOfView->height; height++)
        {
            switch (fieldOfView->data[width][height])
            {
                case EMPTY:
                    avgDistEmpty += sqrt(pow(width-centerPointx,2) + pow(height-centerPointy,2));
                    emptyPoint++;
                    break;
                case WALL:
                    avgDistWall += sqrt(pow(width-centerPointx,2) + pow(height-centerPointy,2));
                    wallPoint++;
                    break;
                case FOG:
                    if(isVisibleFrom(fieldOfView, (fieldOfView->width-1)/2, (fieldOfView->height-1)/2, width, height))
                    {
                        avgDistFog += sqrt(pow(width-centerPointx,2) + pow(height-centerPointy,2));
                        fogPoint++;
                    }
                    break;
                case VISITED:
                    avgDistVisited += sqrt(pow(width-centerPointx,2) + pow(height-centerPointy,2));
                    visitedPoint++;
                    break;
            }
        }
    }

    distFromEntity = sqrt(pow(entity->x-xPosition,2) + pow(entity->y-yPosition,2));

    avgDistEmpty /= emptyPoint;
    avgDistWall /= wallPoint;
    avgDistFog /= fogPoint;
    avgDistVisited /= visitedPoint;

    value = emptyPoint*labelingWeights->weights[NB_EMPTY]+
            wallPoint*labelingWeights->weights[NB_WALL]+
            fogPoint*labelingWeights->weights[NB_FOG]+
            visitedPoint*labelingWeights->weights[NB_VISITED]+
            avgDistEmpty*labelingWeights->weights[AVG_DIST_EMPTY]+
            avgDistWall*labelingWeights->weights[AVG_DIST_WALL]+
            avgDistFog*labelingWeights->weights[AVG_DIST_FOG]+
            avgDistVisited*labelingWeights->weights[AVG_DIST_VISITED]+
            distFromEntity*labelingWeights->weights[DIST_FROM_ENTITY];

    return value;
}

/**
 * \fn float* convertLabeling2(int fieldWidth, int fieldHeight, node *label)
 * \brief function that converts a label2 into a valid output of neural network
 *
 * \param fieldWidth, fieldHeight : dimensions of the field
 * \param label : node found by function labeling2
 * 
 * \return float*
 */
float *convertLabeling2(int fieldWidth, int fieldHeight, node *label)
{
    int index;
    int outputSize = fieldWidth * fieldHeight;
    float *output = (float*)malloc(outputSize*sizeof(float));
    
    for(index = 0; index < outputSize; index++)
        output[index] = 0;

    output[label->y + label->x * fieldHeight] = 1;

    return output;
}
