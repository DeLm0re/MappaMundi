/**
 * \file stats.c
 * \brief functions to gather statistics on the NN and GN performances, and to parse this data into files
 * \author Théo Hipault
 * \version 0.1
 * \date 10/05/2019
 *
 * Module that contain all the functions used to generate statistics on the neural networks
 *
 */

#include "stats.h"

bool fileExists(const char * fileName)
{
    FILE *file;
    if( (file = fopen(fileName, "r")) )
    {
        fclose(file);
        return true;
    }
    return false;
}

bool writeStatsIntoFile(Statistics *stats, char *folderPath)
{
    FILE* file;
	if(stats != NULL)
    {
        int pathLength = strlen(folderPath) + strlen(stats->mapId) + strlen(stats->nnId) + sizeof("/m-net.stat");
	    char *path = (char*)malloc(pathLength * sizeof(char));
	    sprintf(path, "%s/m%s-net%s.stat", folderPath, stats->mapId, stats->nnId);

        bool fileIsNew = true;
        if(fileExists(path))
            fileIsNew = false;

        char *fopenParam = "a";
        if(fileIsNew)
            fopenParam = "w";

        if((file = fopen(path, fopenParam)))
        {	
            const char *separator = ",";
            const char *lineJump = "\n";
            
            if(fileIsNew)
            {
                fputs("Map, Neural network, Number of steps, Number of fog tiles revealed, Number of decisions taken, Average time to take a decision", file);
            }
            
            fputs(lineJump, file);
            
            fputs(stats->mapId, file);
            fputs(separator, file);
            fputs(stats->nnId, file);
            fputs(separator, file);

            int i;
            for(i = 0; i < SIZEOFSTAT; i++)
            {
                fprintf(file, "%f", stats->data[i]);
                fputs(separator, file);
            }

            fclose(file);
            return true;
        }

        printf("Statistics : Unable to write file %s\n", path);        
    }

    return false;
}

void startDecisionClock(Statistics *stats)
{
	if(stats != NULL)
	{
		stats->startTime = clock();
	}
}

void endDecisionClock(Statistics *stats)
{
	if(stats != NULL)
	{
		stats->endTime = clock();
		stats->data[AVG_EXECUTION_TIME] += (float)(stats->endTime - stats->startTime) / CLOCKS_PER_SEC;
	}
}

void initStats(Statistics *stats, const char *mapPath, const char *networkPath)
{
    if(stats != NULL)
    {
        if(mapPath != NULL)
        {
            char *mapName = (char*)malloc(strlen(mapPath) * sizeof(char)) ;
            mapName = getLastElementOfString(mapPath, "/", 3);
            stats->mapId = mapName;
        }
        else
            stats->mapId = "Random";
        if(networkPath != NULL)
        {
            char *networkName = (char*)malloc(strlen(networkPath) * sizeof(char));
            networkName = getLastElementOfString(networkPath, "/", 3);
            stats->nnId = networkName;
        }
        else
            stats->nnId = "Unknown name";

        int i;
        for(i = 0; i < SIZEOFSTAT; i++)
            stats->data[i] = 0;
    }
}

void endStatsComputations(Statistics *stats)
{
    if(stats != NULL)
    {
        if(stats->data[NB_DECISIONS] > 0)
            stats->data[AVG_EXECUTION_TIME] = stats->data[AVG_EXECUTION_TIME] / stats->data[NB_DECISIONS];
    }
}

char *getLastElementOfString(const char *path, const char *delimiters, int nbElements)
{
    char copy[strlen(path)];
    strcpy(copy, path);
    char *part = strtok(copy, delimiters);
    char *array[nbElements];
    int partIndex = 0;

    while (part != NULL)
    {
        array[partIndex++] = part;
        part = strtok (NULL, delimiters);
    }

    int sizeOfResult = strlen(array[partIndex - 1]);
    char *result = (char*)malloc(sizeOfResult*sizeof(char));
    strcpy(result, array[partIndex - 1]);
    
    return result;    
}