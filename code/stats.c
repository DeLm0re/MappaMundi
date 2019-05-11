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

bool writeStatsIntoFile(Statistics *stats, char *path) 
{
    FILE* file;
	if(stats != NULL)
    {
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
                fputs("Map, Neural network, Number of steps, Number of fog tiles revealed, Execution time", file);
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

void startClock(Statistics *stats)
{
	if(stats != NULL)
	{
		stats->startTime = clock();
	}
}

void endClock(Statistics *stats)
{
	if(stats != NULL)
	{
		stats->endTime = clock();
		stats->data[EXECUTION_TIME] = (float)(stats->endTime - stats->startTime) / CLOCKS_PER_SEC;
	}
}

void initStats(Statistics *stats, const char *mapName, const char *networkName)
{
    if(stats != NULL)
    {
        if(mapName != NULL)
            stats->mapId = mapName;
        else
            stats->mapId = "Random";
        if(networkName != NULL)
            stats->nnId = networkName;
        else
            stats->nnId = "Unknown name";

        int i;
        for(i = 0; i < SIZEOFSTAT; i++)
            stats->data[i] = 0;
    }
}