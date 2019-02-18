/**
 * \file display.c
 * \brief Display of project
 * \author Romain Jacquiez
 * \version 0.3
 * \date 09/10/2018
 *
 * Module that contain all the functions making the display of the project
 *
 */

//Header file containing the prototypes
#include "display.h"

/**
 * \fn int drawField(SDL_Renderer *renderer, field oneField)
 * \brief function that draw our field in a graphical display
 *
 * \param SDL_Renderer *renderer which is our renderer displayed by our graphical window using SDL
 * \param oneField : A field, which is a tydef declared in core.h (2D array)
 * \param height : height of the field, must be 1 widen than the original size because the edges is initialized with -1
 * \param width : width of the field, must be 1 widen than the original size because the edges is initialized with -1
 * \param size : the size of a single unique obstacle
 * \return int, can return an error during the drawing
 */
int drawField(SDL_Renderer *renderer, Field oneField, int height, int width, int size)
{
    int i;
    int j;
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color black = {0, 0, 0, 255};
    SDL_Rect obstacle = {0, 0, 0, 0};

    //Set a white background
    if(SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a) < 0)
        return -1;
    if(SDL_RenderClear(renderer) < 0)
        return -1;

    //Set black color
    if(SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a) < 0)
    {
        return -1;
    }

    for(i = 0; i < height; i++)
    {
        for(j = 0; j < width; j++)
        {
            if(oneField[j][i] == WALL)
            {
                obstacle.x = size*j;
                obstacle.y = size*i;
                obstacle.h = size;
                obstacle.w = size;
                //We draw a black square at the same position.
                if(SDL_RenderFillRect(renderer, &obstacle) < 0)
                {
		            return -1;
                }
            }

        }
    }
    return 0;
}

/**
 * \fn int draw(SDL_Renderer *renderer)
 * \brief function that make a drawing test
 *
 * \param SDL_Renderer *renderer which is our renderer displayed by our graphical window using SDL
 * \return int, can return an error during the drawing
 */
int draw(SDL_Renderer *renderer)
{
    SDL_Color noir = {0, 0, 0, 255}; 
    SDL_Color rouge = {255, 0, 0, 255}; 
    SDL_Color vert = {0, 255, 0, 255};
    SDL_Rect rectangle1 = {100, 50, 500, 50}; //x, y, largeur, hauteur
    SDL_Rect rectangle2 = {40, 300, 80, 150}; //x, y, largeur, hauteur
    
    //Efface toute la fenêtre en noir
    if(SDL_SetRenderDrawColor(renderer, noir.r, noir.g, noir.b, noir.a) < 0)
        return -1;
    if(SDL_RenderClear(renderer) < 0)
        return -1;
        
    //Dessine un point rouge
    if(SDL_SetRenderDrawColor(renderer, rouge.r, rouge.g, rouge.b, rouge.a) < 0)
        return -1;
    if(SDL_RenderDrawPoint(renderer, 10, 10) < 0)
		return -1;

    //Dessine une ligne verte
    if(SDL_SetRenderDrawColor(renderer, vert.r, vert.g, vert.b, vert.a) < 0)
        return -1;
    if(SDL_RenderDrawLine(renderer, 50, 40, 600, 410) < 0)
		return -1;	

    //Dessine un rectangle
    if(SDL_SetRenderDrawColor(renderer, vert.r, vert.g, vert.b, vert.a) < 0)
        return -1;
    if(SDL_RenderDrawRect(renderer, &rectangle1) < 0)
		return -1;	

    //Dessine un rectangle plein
    if(SDL_SetRenderDrawColor(renderer, rouge.r, rouge.g, rouge.b, rouge.a) < 0)
        return -1;
    if(SDL_RenderFillRect(renderer, &rectangle2) < 0)
		return -1;	        
    return 0;  
}