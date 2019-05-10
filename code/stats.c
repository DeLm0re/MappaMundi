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

bool writeStatsIntoFile(Statistics *stats, char *path, bool fileIsNew) 
{
    FILE* file;
	if(stats != NULL)
    {
        char *fopenParam = "a";
        if(fileIsNew)
            fopenParam = "w";

        if((file = fopen(path, fopenParam)))
        {	
            const char *separator = ",";
            const char *lineJump = "\n";
            
            if(fileIsNew)
            {
                const char *header = "Number of steps, Number of fog tiles revealed, Execution time";
                fputs(header, file);
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