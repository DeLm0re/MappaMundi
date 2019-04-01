/**
 * \file core.c
 * \brief Core of the project
 * \author Romain Jacquiez
 * \version 0.2
 * \date 09/10/2018
 *
 * Module that contain all the functions making the core of the project
 *
 */

//Header file containing the prototypes
#include "core.h"


/**
 * \fn int **create2DIntArray(int height, int width)
 * \brief function that creates a 2 dimension array of int
 *
 * \param width : width of the array
 * \param height : height of the array
 * \return int** : A pointer to the created array
 */
int **create2DIntArray(int width, int height)
{
    int **array = (int**)malloc(sizeof(int*) * width);
    
    int widthIndex;
    for(widthIndex = 0; widthIndex < width; widthIndex++)
    {
        array[widthIndex] = (int*)malloc(sizeof(int) * height);
    }

    return array;
}

/**
 * \fn int **create2DIntArray(int width, int height)
 * \brief function that creates a 2 dimension array of int
 *
 * \param width : width of the array
 * \param height : height of the array
 * \return int** : A pointer to the created array
 */
float **create2DFloatArray(int width, int height)
{
    float **array = (float**)malloc(sizeof(float*) * width);
    
    int widthIndex;
    for(widthIndex = 0; widthIndex < width; widthIndex++)
    {
        array[widthIndex] = (float*)malloc(sizeof(float) * height);
    }

    return array;
}


/**
 * \fn Field initialiseField(int width, int height)
 * \brief function that initialise our field to make our environment
 *
 * \param width : width of the field, must be 1 widen than the original size because the edges is initialized with -1
 * \param height : height of the field, must be 1 widen than the original size because the edges is initialized with -1
 * \param defaultValue : the value we initialize each tile of the field with
 * \return Field : Pointer to a Field, which is a tydef declared in core.h (2D array struct)
 */
Field *initialiseField(int width, int height, pointEnum defaultValue)
{
    Field *oneField = (Field*)malloc(sizeof(Field));

    oneField->width = width;
    oneField->height = height;
    oneField->data = create2DIntArray(width, height);

    int widthIndex, heightIndex;

    for(widthIndex = 1; widthIndex < (width - 1); widthIndex++)
    {
        for(heightIndex = 1; heightIndex < (height - 1); heightIndex++)
        {
            oneField->data[widthIndex][heightIndex] = defaultValue;
        }
    }

    for(heightIndex = 0; heightIndex < height; heightIndex++)
    {
        oneField->data[0][heightIndex] = WALL;
        oneField->data[width-1][heightIndex] = WALL;
    }

    for(widthIndex = 0; widthIndex < width; widthIndex++)
    {
        oneField->data[widthIndex][height-1] = WALL;
        oneField->data[widthIndex][0] = WALL;
    }

    return oneField;
}

Field *createCustomField(char *customFieldName)
{
    Field *oneField = (Field*)malloc(sizeof(Field));

    int widthIndex, heightIndex;
    int compteur = 0;

    char buffer[5] = ".bmp";
    char *completeCustomFieldName;
    completeCustomFieldName = (char *)malloc((strlen(customFieldName)+strlen(buffer))*sizeof(char));
    strcpy(customFieldName, buffer);

    DonneesImageRGB *imageCustomField = lisBMPRGB(completeCustomFieldName);

    if(imageCustomField == NULL)
    {
        printf("ERROR : Imposible to read the custom field... \n");
    }

    oneField->width = imageCustomField->largeurImage;
    oneField->height = imageCustomField->hauteurImage;

    oneField->data = create2DIntArray(oneField->width, oneField->height);

    //Copie des valeurs
	for(heightIndex = (oneField->height-1); heightIndex >= 0; heightIndex--)
	{
		for(widthIndex = 0; widthIndex < oneField->width; oneField->width++)
		{
			if(imageCustomField->donneesRGB[compteur] == 0)
            {
                oneField->data[widthIndex][heightIndex] = WALL;
            }
            if(imageCustomField->donneesRGB[compteur] == 255)
            {
                oneField->data[widthIndex][heightIndex] = EMPTY;
            }
            else
            {
                oneField->data[widthIndex][heightIndex] = -1;
            }
			compteur = compteur + 3;
		}
	}

    return oneField;
}

/**
 * \fn Field initialiseInterestField(int width, int height)
 * \brief function that initialise our field to make our environment
 *
 * \param width : width of the interest field
 * \param height : height of the interest field
 * \return Field : Pointer to an InterestField
 */
InterestField *initialiseInterestField(int width, int height)
{
    InterestField *oneField = (InterestField*)malloc(sizeof(InterestField));

    oneField->width = width;
    oneField->height = height;
    oneField->data = create2DFloatArray(width, height);

    int widthIndex, heightIndex;

    for(widthIndex = 0; widthIndex < width; widthIndex++)
    {
        for(heightIndex = 0; heightIndex < height; heightIndex++)
        {
            oneField->data[widthIndex][heightIndex] = 0;
        }
    }

    return oneField;
}

/**
 * \fn void generateEnv(field oneField)
 * \brief function that generate our field to make our environment
 *
 * \param oneField : Poiter to a field, which is a tydef declared in core.h (2D array structure)
 * \return void
 */
void generateEnv(Field *oneField)
{

    int w; //i
    int h; //j
    int monRand;
    int sum_neigh;

    //First loop, random generation (obstacle or not, Bernoulli)
    for(w = 1; w < (oneField->width-1); w++)
    {
        for(h = 1; h < (oneField->height-1); h++)
        {
            monRand = rand()%10 + 1;

            if(monRand < 5)
            {
                oneField->data[w][h] = WALL;
            }
        }
    }

    //Second loop to clean and prevent stuck situation
    for(w = 1; w < (oneField->width-1); w++)
    {
        for(h = 1; h < (oneField->height-1); h++)
        {
            sum_neigh = oneField->data[w-1][h-1] + oneField->data[w-1][h] + oneField->data[w][h-1] + oneField->data[w-1][h+1]
                        + oneField->data[w+1][h-1] + oneField->data[w+1][h] + oneField->data[w][h+1] + oneField->data[w+1][h+1];

            //Clean some obstacles
            if(sum_neigh < 2)
            {
                oneField->data[w][h] = EMPTY;
            }
            //Prevent stuck
            else if(sum_neigh >= 4)
            {
                oneField->data[w][h] = EMPTY;
            }
        }
    }

    //Third loop to fill blank
    for(w = 1; w < (oneField->width-1); w++)
    {
        for(h = 1; h < (oneField->height-1); h++)
        {
            sum_neigh = oneField->data[w][h-1] + oneField->data[w-1][h] + oneField->data[w][h+1] + oneField->data[w+1][h];

            if(sum_neigh >= 3)
            {
                oneField->data[w][h] = WALL;
            }
        }
    }
}

/**
 * \fn void destruct2DIntArray(int **array, int width)
 * \brief function that free the 2D array out of memory
 *
 * \param **array : The array to free
 * \param width : width of the array
 * \return void
 */
void destruct2DIntArray(int **array, int width)
{
    int i;
    if(array != NULL)
    {
        for(i = 0; i < width; i++)
        {
            free(array[i]);
        }
        free(array);
    }
}

/**
 * \fn void destruct2DFloatArray(int **array, int width)
 * \brief function that free the 2D array out of memory
 *
 * \param **array : The array to free
 * \param width : width of the array
 * \return void
 */
void destruct2DFloatArray(float **array, int width)
{
    int i;
    if(array != NULL)
    {
        for(i = 0; i < width; i++)
        {
            free(array[i]);
        }
        free(array);
    }
}

/**
 * \fn void destructField(Field oneField)
 * \brief function that free the field out of memory
 *
 * \param **oneField : A double pointer on a field, which is a tydef declared in core.h (2D array structure)
 * \return void
 */
void destructField(Field **oneField)
{
    if(oneField != NULL)
    {
        if(*oneField != NULL)
        {
            destruct2DIntArray((*oneField)->data, (*oneField)->width);
            (*oneField)->data = NULL;
            free(*oneField);
            *oneField = NULL;
        }
    }
}

/**
 * \fn void destructInterestField(InterestField **oneField)
 * \brief function that free the field out of memory
 *
 * \param **oneField : A double pointer on an InterestField
 * \return void
 */
void destructInterestField(InterestField **oneField)
{
    if(oneField != NULL)
    {
        if(*oneField != NULL)
        {
            destruct2DFloatArray((*oneField)->data, (*oneField)->width);
            (*oneField)->data = NULL;
            free(*oneField);
            *oneField = NULL;
        }
    }
}

/**
 * \fn int surface2DCircle(int radius)
 * \brief function that return the number of 1 by 1 square in a 2D circle
 *
 * \param int radius : the radius of the circle
 * \return int : the surface
 */
int surface2DCircle(int radius)
{
    int radiusSquare = radius * radius;
    float distanceSquare;
    int increment = 0;
    int x, y;

    for(x = -radius; x <= radius; x++)
    {
        for(y = -radius; y <= radius; y++)
        {
            distanceSquare = x*x + y*y;
                    
            if(distanceSquare < radiusSquare)
            {
                increment++;
            }
        }
    }

    return(increment);
}

/**
 * \fn Field* generateRandomFieldOfView(int visionRange)
 * \brief function that returns a random field of view
 * will be used for labelization
 *
 * \param int visionRange : the vision range
 * \param bool isValid : if the random field of view is a valid position to move or not
 * \return Field*
 */
Field* generateRandomFieldOfView(int visionRange, bool isValid)
{
    Field *fieldOfView = initialiseField(2*visionRange+1, 2*visionRange+1, FOG);
    int width, height;
    for(width = 0; width < fieldOfView->width; width++)
    {
        for(height = 0; height < fieldOfView->height; height++)
        {
            fieldOfView->data[width][height] = rand()%3;
        }
    }
    if (isValid)
    { 
        fieldOfView->data[visionRange][visionRange] = EMPTY;
    }
    else
    {
        fieldOfView->data[visionRange][visionRange] = rand()%2 + 1;
    }
    return fieldOfView;
}

/**
 * \fn Field* labeling(Field* fieldOfView, int xPosition, int yPosition, int xFinalPosition, int yFinalPosition)
 * \brief function that returns the labeling of the points
 * will be used for labelisation
 *
 * \param Field* fieldOfView : a field of view
 * \param int xPosition : x coordinate of the entity
 * \param int yPosition : y coordinate of the entity
 * \param int xFinalPosition : x coordinate of the end point
 * \param int yFinalPosition : y coordinate of the end point
 * \return float
 */
float labeling(Field* fieldOfView, int xPosition, int yPosition, int xFinalPosition, int yFinalPosition)
{
    int emptyPoint = 0;
    int wallPoint = 0;
    int fogPoint = 0;
    int visitedPoint = 0;
    int distPoint = 0;
    float value = 0;
    float finalValue = 0;

    if (fieldOfView->data[(fieldOfView->height-1)/2][(fieldOfView->width-1)/2] != EMPTY) 
    {
        return 0;
    }
    
    for(int width = 0; width < fieldOfView->width; width++)
    {
        for(int height = 0; height < fieldOfView->height; height++)
        {
            switch (fieldOfView->data[width][height])
            {
                case EMPTY:
                    emptyPoint++;
                    break;
                case WALL:
                    wallPoint++;
                    break;
                case FOG:
                    fogPoint++;
                    break;
                case VISITED:
                    visitedPoint++;
                    break;
            }
        }
    }

    distPoint = (xFinalPosition-xPosition)*(xFinalPosition-xPosition) + (yFinalPosition-yPosition)*(yFinalPosition-yPosition);

    value = fogPoint/(distPoint*0.1);

    finalValue = (1.0/(1+exp(-value)));

    return finalValue;
}

/**
 * \fn Field* labeling2(Field* fieldOfView, int xPosition, int yPosition, int xFinalPosition, int yFinalPosition)
 * \brief function that returns the labeling of the points
 * will be used for labelisation
 *
 * \param Field* fieldOfView : a field of view
 * \param int xPosition : x coordinate of the entity
 * \param int yPosition : y coordinate of the entity
 * \param int xFinalPosition : x coordinate of the end point
 * \param int yFinalPosition : y coordinate of the end point
 * \return float
 */
float labeling2(Field* fieldOfView, int xPosition, int yPosition, int xFinalPosition, int yFinalPosition)
{
    int emptyPoint = 0;
    int wallPoint = 0;
    int fogPoint = 0;
    int visitedPoint = 0;
    int distPoint = 0;
    float value = 0;
    float finalValue = 0;

    if (fieldOfView->data[(fieldOfView->height-1)/2][(fieldOfView->width-1)/2] != EMPTY) 
    {
        return 0;
    }
    
    for(int width = 0; width < fieldOfView->width; width++)
    {
        for(int height = 0; height < fieldOfView->height; height++)
        {
            switch (fieldOfView->data[width][height])
            {
                case EMPTY:
                    emptyPoint++;
                    break;
                case WALL:
                    wallPoint++;
                    break;
                case FOG:
                    if(isVisibleFrom(fieldOfView, (fieldOfView->height-1)/2, (fieldOfView->width-1)/2, width, height))
                    {
                        fogPoint++;
                    }
                    break;
                case VISITED:
                    visitedPoint++;
                    break;
            }
        }
    }

    distPoint = (xFinalPosition-xPosition)*(xFinalPosition-xPosition) + (yFinalPosition-yPosition)*(yFinalPosition-yPosition);

    value = fogPoint/(distPoint*0.1);

    finalValue = (1.0/(1+exp(-value)));

    return finalValue;
}

/**
 * \fn bool isVisibleFrom(Field* fieldOfView, int xOrigin, int yOrigin, int xPosition, int yPosition)
 * \brief function that returns true if a given position is visible from actual position
 *
 * \param Field* fieldOfView : a field of view
 * \param int xOrigin : x coordinate of actual position
 * \param int yOrigin : y coordinate of actual position
 * \param int xPosition : x coordinate we want to check
 * \param int yPosition : y coordinate we want to check
 * \return bool
 */
bool isVisibleFrom(Field* fieldOfView, int xOrigin, int yOrigin, int xPosition, int yPosition)
{
    float angle = atan2(yPosition - yOrigin, xPosition - xOrigin);

    float dist = sqrt(pow(xPosition-xOrigin, 2) + pow(yPosition-yOrigin, 2));

    float vect[2] = {cos(angle), sin(angle)};

    bool isVisible = true;

    for(int i = 0; i < dist; i++)
    {
        float x = xOrigin + i * vect[0];
        float y = yOrigin + i * vect[1];

        if ( ((int) x) != xPosition && ((int) y) != yPosition)
        {
            if (fieldOfView->data[(int) x][(int) y] == WALL)
            {
                isVisible = false;
            }
        }
    }
    return isVisible;
}


