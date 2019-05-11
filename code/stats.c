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
                fputs("Number of steps, Number of fog tiles revealed, Execution time", file);
            }
            
            fputs(lineJump, file);
            
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