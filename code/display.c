//Header file containing the prototypes
#include "display.h"

int drawField(SDL_Renderer *renderer, field oneField)
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

    puts("back");

    //Set black color
    if(SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a) < 0)
    {
        return -1;
    }

    puts("setcolor");

    for(i = 0; i < 50; i++)
    {
        for(j = 0; j < 50; j++)
        {
            if(oneField[i][j] == 1)
            {
                obstacle.x = 20*i;
                obstacle.y = 20*j;
                obstacle.h = 20;
                obstacle.w = 20;
                //We draw a black square at the same position
                if(SDL_RenderDrawRect(renderer, &obstacle) < 0)
                {
		            return -1;
                }
            }

        }
    }
    return 0;
}

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