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
 * \fn int drawField(SDL_Renderer *renderer, field oneField, int size)
 * \brief function that draw our field in a graphical display
 *
 * \param SDL_Renderer *renderer which is our renderer displayed by our graphical window using SDL
 * \param oneField* : Pointer to a field, which is a tydef declared in core.h (2D array structure)
 * \param size : the size of a single unique obstacle
 * \return int, can return an error during the drawing
 */
int drawField(SDL_Renderer *renderer, Field *oneField, int size)
{
    int w, h;

    SDL_Color emptyColor = {255, 255, 255, 255};
    SDL_Color wallColor = {0, 0, 0, 255};
    SDL_Color fogColor = {100, 100, 100, 255};
    SDL_Color visitedColor = {220, 220, 220, 255};
    SDL_Color defaultColor = {255, 0, 0, 255};

    SDL_Rect square = {0, 0, 0, 0};

    for(w = 0; w < oneField->width; w++)
    {
        for(h = 0; h < oneField->height; h++)
        {
            switch(oneField->data[w][h])
            {
                case WALL:
                    //Set black color
                    if(SDL_SetRenderDrawColor(renderer, wallColor.r, wallColor.g, wallColor.b, wallColor.a) < 0)
                    {
                        return -1;
                    }
                break;
                
                case EMPTY:
                    //Set white color
                    if(SDL_SetRenderDrawColor(renderer, emptyColor.r, emptyColor.g, emptyColor.b, emptyColor.a) < 0)
                    {
                        return -1;
                    }
                break;

                case FOG:
                    //Set fog color
                    if(SDL_SetRenderDrawColor(renderer, fogColor.r, fogColor.g, fogColor.b, fogColor.a) < 0)
                    {
                        return -1;
                    }
                break;
                
                case VISITED:
                    //Set visited color
                    if(SDL_SetRenderDrawColor(renderer, visitedColor.r, visitedColor.g, visitedColor.b, visitedColor.a) < 0)
                    {
                        return -1;
                    }
                    break;
                
                default:
                    //Set default color
                    if(SDL_SetRenderDrawColor(renderer, defaultColor.r, defaultColor.g, defaultColor.b, defaultColor.a) < 0)
                    {
                        return -1;
                    }
                break;
            }

            square.x = size*w;
            square.y = size*h;
            square.h = size;
            square.w = size;

            //We draw a square at the same position.
            if(SDL_RenderFillRect(renderer, &square) < 0)
            {
		        return -1;
            }
        }
    }
    return 0;
}

/**
 * \fn int drawFieldOfViewEntity(SDL_Renderer *renderer, Entity *oneEntity, int size)
 * \brief function that draw the field of view of an entity in a graphical display
 *
 * \param SDL_Renderer *renderer which is our renderer displayed by our graphical window using SDL
 * \param oneEntity : Pointer to a field of view of an entity, which is a tydef declared in entity.h (2D array structure)
 * \param theField : Pointer to a field
 * \param size : the size of a single unique obstacle
 * \return int, can return an error during the drawing
 */
int drawFieldOfViewEntity(SDL_Renderer *renderer, Entity *oneEntity, Field *theField, int size)
{
    int w, h;

    SDL_Color emptyColor = {255, 255, 255, 255};
    SDL_Color wallColor = {0, 0, 0, 255};
    SDL_Color fogColor = {100, 100, 100, 255};
    SDL_Color defaultColor = {255, 0, 0, 255};

    SDL_Rect square = {0, 0, 0, 0};

    for(w = 0; w < 2*oneEntity->visionRange + 2; w++)
    {
        for(h = 0; h < 2*oneEntity->visionRange + 2; h++)
        {
            //Set fog color
            if(SDL_SetRenderDrawColor(renderer, fogColor.r, fogColor.g, fogColor.b, fogColor.a) < 0)
            {
                return -1;
            }

            square.x = size*w + ((theField->width) * size) + /*(offset)*/ 2*size;
            square.y = size*h;
            square.h = size;
            square.w = size;

            //We draw a square at the same position.
            if(SDL_RenderFillRect(renderer, &square) < 0)
            {
		        return -1;
            }
        }
    }

    for(w = 0; w < 2*oneEntity->visionRange; w++)
    {
        for(h = 0; h < 2*oneEntity->visionRange; h++)
        {
            switch(oneEntity->fieldOfView[w][h].pointValue)
            {
                case WALL:
                    //Set black color
                    if(SDL_SetRenderDrawColor(renderer, wallColor.r, wallColor.g, wallColor.b, wallColor.a) < 0)
                    {
                        return -1;
                    }
                break;
            
                case EMPTY:
                    //Set white color
                    if(SDL_SetRenderDrawColor(renderer, emptyColor.r, emptyColor.g, emptyColor.b, emptyColor.a) < 0)
                    {
                        return -1;
                    }
                break;

                case FOG:
                    //Set fog color
                    if(SDL_SetRenderDrawColor(renderer, fogColor.r, fogColor.g, fogColor.b, fogColor.a) < 0)
                    {
                        return -1;
                    }
                break;
                
                default:
                    //Set default color
                    if(SDL_SetRenderDrawColor(renderer, defaultColor.r, defaultColor.g, defaultColor.b, defaultColor.a) < 0)
                    {
                        return -1;
                    }
                break;
            }

            square.x = size*(w+1) + ((theField->width) * size) + /*(offset)*/ 2*size;
            square.y = size*(h+1);
            square.h = size;
            square.w = size;

            //We draw a square at the same position.
            if(SDL_RenderFillRect(renderer, &square) < 0)
            {
		        return -1;
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
