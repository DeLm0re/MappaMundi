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

/**
 * \fn bool fileExists(const char * fileName)
 * \brief verify if a file exists or not given its path. Returns true if it exists, false otherwise
 * 
 * \param
 *      filePath : path to the file to verify
 *
 * \return
 * 		bool
 */
bool fileExists(const char * filePath)
{
    FILE *file;
    if( (file = fopen(filePath, "r")) )
    {
        fclose(file);
        return true;
    }
    return false;
}

/**
 * \fn bool writeStatsIntoFile(Statistics *stats, char *folderPath)
 * \brief writes given statistics in a file in the folder specified in parameter, then prompts a message. 
 *        The file name is generated with the statistics informations. Returns true if successfully saves, false otherwise.         
 * 
 * \param
 *      stats : statistics to save
 *      folderPath : path where to save the statistics
 *
 * \return
 * 		bool
 */
bool writeStatsIntoFile(Statistics *stats, char *folderPath)
{
    FILE* file;
	if(stats != NULL)
    {
        int pathLength = strlen(folderPath) + strlen(stats->mapId) + strlen(stats->nnId) + sizeof("/-.stat");
	    char *path = (char*)malloc(pathLength * sizeof(char));
	    sprintf(path, "%s/%s-%s.stat", folderPath, stats->mapId, stats->nnId);

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
            printf("Statistics : File saved as %s\n", path);        

            return true;
        }

        printf("Statistics : Unable to write file %s\n", path);        
    }

    return false;
}

/**
 * \fn void startDecisionClock(Statistics *stats)
 * \brief start the clock used to measure execution time of a neural network
 * 
 * \param
 *      stats : structure used to store statistics
 *
 * \return
 * 		void
 */
void startDecisionClock(Statistics *stats)
{
	if(stats != NULL)
	{
		stats->startTime = clock();
	}
}

/**
 * \fn void endDecisionClock(Statistics *stats)
 * \brief end the clock used to measure execution time of a neural network and add it the the stat structure.
 *        Must call endStatsComputations function at the end of the statistics gathering to compute the average execution time.
 * 
 * \param
 *      stats : structure used to store statistics
 * \return
 * 		void
 */
void endDecisionClock(Statistics *stats)
{
	if(stats != NULL)
	{
		stats->endTime = clock();
		stats->data[AVG_EXECUTION_TIME] += (float)(stats->endTime - stats->startTime) / CLOCKS_PER_SEC;
	}
}

/**
 * \fn void initStats(Statistics *stats, const char *mapPath, const char *networkPath)
 * \brief initialize a strucutre statistics with default values and the name of the map and neural network used
 * 
 * \param
 *      stats : structure to initialize
 *      mapPath : path of the map used in this simulation
 *      networkPath : path of the neural network used on this map
 *
 * \return
 * 		void
 */
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

/**
 * \fn void endStatsComputations(Statistics *stats)
 * \brief compute the final values of the statistics, like averages
 * 
 * \param
 *      stats : statistics to finalize
 *
 * \return
 * 		void
 */
void endStatsComputations(Statistics *stats)
{
    if(stats != NULL)
    {
        if(stats->data[NB_DECISIONS] > 0)
            stats->data[AVG_EXECUTION_TIME] = stats->data[AVG_EXECUTION_TIME] / stats->data[NB_DECISIONS];
    }
}

/**
 * \fn char *getLastElementOfString(const char *path, const char *delimiters, int nbElements)
 * \brief returns the last element of a string, given the delimitersbetween elements and the number of elements
 * 
 * \param
 *      str : string to split
 *      delimiters : delimiters between each element
 *      nbElements : number of elements in str
 * \return
 * 		char*
 */
char *getLastElementOfString(const char *str, const char *delimiters, int nbElements)
{
    char copy[strlen(str)];
    strcpy(copy, str);
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