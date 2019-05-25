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
 * \fn int **create2DIntArray(int width, int height)
 * \brief Function that creates a 2 dimension array of int
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
 * \fn Field initialiseField(int width, int height, pointEnum defaultValue)
 * \brief Function that initialise a field with a specified value
 *
 * \param width : width of the field
 * \param height : height of the field
 * \param defaultValue : the value we initialize each tile of the field with
 * \return Field* : Pointer to a Field
 */
Field *initialiseField(int width, int height, pointEnum defaultValue)
{
    Field *oneField = (Field*)malloc(sizeof(Field));

    oneField->width = width;
    oneField->height = height;
    oneField->data = create2DIntArray(width, height);

    int widthIndex, heightIndex;

    for(widthIndex = 0; widthIndex < width; widthIndex++)
    {
        for(heightIndex = 0; heightIndex < height; heightIndex++)
        {
            oneField->data[widthIndex][heightIndex] = defaultValue;
        }
    }

    return oneField;
}

/**
 * \fn Field *createCustomField(char *pathImageField)
 * \brief Function that creates a field using a custom image
 *
 * \param char *pathImageField : the path for the bmp image of the custom field
 * 
 * \return Field* : Pointer to the created Field
 */
Field *createCustomField(char *pathImageField)
{
    DonneesImageRGB *imageCustomField = lisBMPRGB(pathImageField);

    if(imageCustomField == NULL)
    {
        return NULL;
    }
    
    Field *oneField = (Field*)malloc(sizeof(Field));
    oneField->width = imageCustomField->largeurImage;
    oneField->height = imageCustomField->hauteurImage;
    oneField->data = create2DIntArray(oneField->width, oneField->height);

    int widthIndex, heightIndex;
    int compteur = 0;
    //Copie des valeurs
	for(heightIndex = (oneField->height-1); heightIndex >= 0; heightIndex--)
	{
		for(widthIndex = 0; widthIndex < oneField->width; widthIndex++)
		{
            if(imageCustomField->donneesRGB[compteur] == 255)
            {
                oneField->data[widthIndex][heightIndex] = EMPTY;
            }
            else
            {
                oneField->data[widthIndex][heightIndex] = WALL;
            }
			compteur = compteur + 3;
		}
	}

    return oneField;
}

/**
 * \fn void generateEnv(Field *oneField)
 * \brief Function that generates a random Field
 *
 * \param oneField : Field to randomize
 * \return void
 */
void generateEnv(Field *oneField)
{

    int w; //i
    int h; //j
    int monRand;
    int sum_neigh;

    for(h = 0; h < oneField->height; h++)
    {
        oneField->data[0][h] = WALL;
        oneField->data[oneField->width-1][h] = WALL;
    }

    for(w = 0; w < oneField->width; w++)
    {
        oneField->data[w][oneField->height-1] = WALL;
        oneField->data[w][0] = WALL;
    }    
    
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
 * \brief Function that frees a 2D array out of memory
 *
 * \param **array : the array to free
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
 * \fn void destructField(Field **oneField)
 * \brief Function that frees a field out of memory
 *
 * \param **oneField : address of the Field pointer to free
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
 * \fn int surface2DCircle(int radius)
 * \brief Function that computes the number of 1 by 1 squares in a 2D circle
 *
 * \param int radius : the radius of the circle
 * \return int : The surface of the circle
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
 * \fn Field* generateRandomFieldOfView(int visionRange, bool isValid)
 * \brief Function that returns a random field of view that will be used for labelization
 *
 * \param int visionRange : the vision range, or size of the field of view
 * \param bool isValid : if the random field of view is a valid position to move or not
 * \return Field* : A random field of view
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
 * \fn bool isVisibleFrom(Field* fieldOfView, int xOrigin, int yOrigin, int xPosition, int yPosition)
 * \brief Function that computes if a given position is visible from actual position in a field of view
 *
 * \param Field* fieldOfView : a field of view
 * \param int xOrigin : x coordinate of actual position
 * \param int yOrigin : y coordinate of actual position
 * \param int xPosition : x coordinate we want to check
 * \param int yPosition : y coordinate we want to check
 * \return bool : True if the gieven position is visible, false otherwise
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

/**
 * \fn int getNbFog(Field* field)
 * \brief Function that computes the number of fog tile in a given field
 *
 * \param Field* field : the field where we will count the number of fog tile
 * \return int : Number of fog tiles in the field
 */
int getNbFog(Field* field)
{
    int nbFog = 0;
    if (field != NULL)
    {
        int width;
        for (width = 0; width < field->width; width++)
        {
            int height;
            for (height = 0; height < field->height; height++)
            {
                if (field->data[width][height] == FOG)
                    nbFog++;
            }
        }
    }
    return nbFog;
}

