#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

//gitkraken
//gpedit fond noir 
//md editor
//Lecteur PDF
//Dossier partagé

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


int main(int argc, char *argv[])
{
	SDL_Event event;
	SDL_bool quit = SDL_FALSE;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int statut = EXIT_FAILURE;

    /* Initialisation, création de la fenêtre et du renderer. */
    if(0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }
    window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              640, 480, SDL_WINDOW_SHOWN);
    if(NULL == window)
    {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        goto Quit;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(NULL == renderer)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        goto Quit;
    }

    //DESSIN
	draw(renderer);

    SDL_Delay(500);
    SDL_RenderPresent(renderer);
    SDL_Delay(500);


	while(!quit)
	{
		SDL_WaitEvent(&event);
		if(event.type == SDL_QUIT)
			quit = SDL_TRUE;
		    statut = EXIT_SUCCESS;
	}    

Quit:
    if(NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if(NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return statut;
}
