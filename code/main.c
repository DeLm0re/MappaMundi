/**
 * \file main.c
 * \brief main of the project
 * \author Théo Hipault
 * \version 0.5
 * \date 12/10/2018
 *
 * Module that contain the main of the project
 *
 */

#include "wrapper.h"

#define TILESIZE 10;

#define FIELD_WIDTH 40
#define FIELD_HEIGHT 40
#define SAVING_PATH_NN "../NN"
#define SAVING_PATH_GN "../GN/pointToGo"
#define SAVING_PATH_GN_EXPLORE "../GN/explore"

//Main of the programme
int main(int argc, char** argv)
{   
	if (argc < 2) 
	{
		printf("Please, enter an argument : \n");
		printf(" 1 : \n\tCreate a new neural network and train it on a random map\n");
		printf(" 2 pathNeuralNetwork [pathMap] : \n\tLoad an existing neural network and test it on a random map or on an existing map\n");
		printf(" 3 [pathGeneticNetwork] [pathMap] : \n\tStart a new genetic network and train it\n\tIt could be based on an existing map and be based on an existing genetic algorithm\n\tput \"NONE\" if you don't want to use any base genetic algorithm\n\tLearns to go to the bottom right\n");
		printf(" 4 pathGeneticNetwork [pathMap] : \n\tLoad an existing genetic network and test it on a random map or on an existing map\n\tTries to go to the bottom right\n");
		printf(" 5 [pathGeneticNetwork] [pathMap] : \n\tStart a new genetic network and train it\n\tIt could be based on an existing map and be based on an existing genetic algorithm\n\tput \"NONE\" if you don't want to use any base genetic algorithm\n\tLearns to explore\n");
	    printf(" 6 pathGeneticNetwork [pathMap] : \n\tLoad an existing genetic network and test it on a random map or on an existing map\n\tTries to explore\n");
	}
	else
	{
		//Initialize the random seed value
		srand(12345);
		
		const int tileSize = TILESIZE;
		int windowWidth = 640;
		int windowHeight = 480;
		
		// Set some basic variables for the SDL to work
		SDL_Event event;
		SDL_Window *window = NULL;
		SDL_Renderer *renderer = NULL;
		dataType* data = initData(&event);
		
		// Initialisation de la SDL
		if(SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
			goto Quit;
		}
		// Initialisation de la fenêtre
		window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
								windowWidth, windowHeight, SDL_WINDOW_HIDDEN);
		if(window == NULL)
		{
			fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
			goto Quit;
		}
		// Initialisation du renderer
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if(renderer == NULL)
		{
			fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
			goto Quit;
		}

		//Declaration of the thread and the data for the events
		pthread_t thread1;
		pthread_create(&thread1, NULL, eventHandlerFunction, (void*) data);

		NeuralNetwork* neuralNetwork = NULL;
		LabelingWeights* labelingWeights = NULL;
		
		Field *theField = NULL;
		int fieldHeight = FIELD_WIDTH;
		int fieldWidth = FIELD_HEIGHT;
		
		char* pathImageField = NULL;
		char* pathNetwork = NULL;
		
		int menuChoice = atoi(argv[1]);

        // We extract the path pf the image if it exist
        switch (menuChoice)
		{
			//New neural network
			case TRAIN_NN:
			//Load neural network
			case LOAD_NN:
			    if (argc >= 3)
			        pathNetwork = argv[2];
			    if (argc == 4)
			    	pathImageField = argv[3];
				break;
			
			//Load genetic network
			case LOAD_GN:
			case LOAD_GN_EXPLORE:
			if (argc >= 3)
			        pathNetwork = argv[2];
			//New genetic network
			case TRAIN_GN_EXPLORE:
			case TRAIN_GN:
			    if (argc == 4)
	                pathImageField = argv[3];
	            
			    break;
		}
		
	    theField = createCustomField(pathImageField);
	    bool fieldIsFromImage = true;
	    if (theField == NULL)
	    {
	        theField = initialiseField(fieldWidth, fieldHeight, EMPTY);
	        generateEnv(theField);
	        fieldIsFromImage = false;
	    }
        fieldHeight = theField->height;
        fieldWidth = theField->width;
		        
		windowWidth = (fieldWidth*tileSize) + /*offset*/ 2*tileSize + (RADIUS_VIEWPOINT*2*tileSize) + 2*tileSize;
		windowHeight = fieldHeight*tileSize;
		SDL_SetWindowSize(window, windowWidth, windowHeight);
		
		switch (menuChoice)
		{
			//New neural network
			case TRAIN_NN:
				SDL_ShowWindow(window);
				neuralNetwork = trainingNN2(fieldWidth, fieldHeight, data, SAVING_PATH_NN, renderer, tileSize);
				break;
			//Load neural network
			case LOAD_NN:
				SDL_ShowWindow(window);
				if (argc >= 3)
                    neuralNetwork = loadNeuralNetwork(argv[2]);
                else
                    printf("Error : Invalid arguments\n");
				break;
			//New genetic network to go to a point
			case TRAIN_GN:
			    if (argc >= 3)
			        labelingWeights = trainingGN1(data, theField, SAVING_PATH_GN, argv[2], 10, 100);
			    else
			        labelingWeights = trainingGN1(data, theField, SAVING_PATH_GN, NULL, 10, 100);
			    break;
			//new genetic network to explore
			case TRAIN_GN_EXPLORE:
			    if (argc >= 3)
			        labelingWeights = trainingGN2(data, theField, SAVING_PATH_GN_EXPLORE, argv[2], 10, 100, 0.9);
			    else
			        labelingWeights = trainingGN2(data, theField, SAVING_PATH_GN_EXPLORE, NULL, 10, 100, 0.9);
			    break;
			//Load genetic network
			case LOAD_GN:
			case LOAD_GN_EXPLORE:
		        if (argc >= 3)
                    labelingWeights = loadGeneticNetwork(argv[2]);
                else
                    printf("Error : Invalid arguments\n");
			    break;
			default:
				printf("Error : Invalid arguments\n");
				break;
		}
		
		SDL_ShowWindow(window);
		
		//--- Main loops

        //Initialize the statistics
		Statistics stats;
        initStats(&stats, pathImageField, pathNetwork);
			
		if (menuChoice == LOAD_NN || menuChoice == TRAIN_NN)
			searchForEndPointNN(neuralNetwork, data, &theField, renderer, tileSize, fieldIsFromImage, &stats);
		else if (menuChoice == LOAD_GN || menuChoice == TRAIN_GN)
		    searchForEndPointGN(labelingWeights, data, &theField, renderer, tileSize, fieldIsFromImage, &stats);
		else if (menuChoice == LOAD_GN_EXPLORE || menuChoice == TRAIN_GN_EXPLORE)
		    exploreGN(labelingWeights, data, &theField, renderer, tileSize, fieldIsFromImage, 200, 0.9, &stats);

		destructNeuralNetwork(&neuralNetwork);
		destructLabelingWeights(&labelingWeights);
		
		data->endEvent = true;
		pthread_join(thread1, NULL);

		//Process that end the window and the renderer
	Quit:
		if(NULL != renderer)
			SDL_DestroyRenderer(renderer);
		if(NULL != window)
			SDL_DestroyWindow(window);
		SDL_Quit();
		int statut = data->statut;
		free(data);
		return statut;
	}
}
