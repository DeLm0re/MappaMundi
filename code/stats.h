/**
 * \file stats.h
 * \brief functions to gather statistics on the NN and GN performances, and to parse this data into files
 * \author Théo Hipault
 * \version 0.1
 * \date 10/05/2019
 *
 * Module that contain all the prototypes of functions in stats.c
 *
 */

#ifndef H_STATS
    #define H_STATS

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "rioFunction.h"

typedef enum {NB_STEPS = 0, NB_FOG_REVEALED, NB_DECISIONS, AVG_EXECUTION_TIME, SIZEOFSTAT} StatIndex;

typedef struct Statistics
{
    const char *mapId, *nnId;
    float data[SIZEOFSTAT];
    clock_t startTime, endTime;
} Statistics;

/**
 * \fn bool fileExists(const char * fileName)
 * \brief Verify if a file exists or not given its path.
 * 
 * \param filePath : path to the file to verify
 *
 * \return bool : True if it exists, False otherwise
 */
bool fileExists(const char * filePath);

/**
 * \fn bool writeStatsIntoFile(Statistics *stats, char *folderPath)
 * \brief Writes given statistics in a file in the folder specified in parameter, then prompts a message. 
 * The file name is generated with the statistics informations.       
 * 
 * \param stats : statistics to save
 * \param folderPath : path where to save the statistics
 *
 * \return bool : True if successfully saves, False otherwise.  
 */
bool writeStatsIntoFile(Statistics *stats, char *folderPath);

/**
 * \fn void startDecisionClock(Statistics *stats)
 * \brief Start the clock used to measure execution time of a neural network
 * 
 * \param stats : structure used to store statistics
 *
 * \return void
 */
void startDecisionClock(Statistics *stats);

/**
 * \fn void endDecisionClock(Statistics *stats)
 * \brief End the clock used to measure execution time of a neural network and add it the the stat structure.
 * Must call endStatsComputations function at the end of the statistics gathering to compute the average execution time.
 * 
 * \param stats : structure used to store statistics
 * \return void
 */
void endDecisionClock(Statistics *stats);

/**
 * \fn void initStats(Statistics *stats, char *mapPath, char *networkPath)
 * \brief Initializes a strucutre statistics with default values and the name of the map and neural network used
 * 
 * \param stats : structure to initialize
 * \param mapPath : path of the map used in this simulation
 * \param networkPath : path of the neural network used on this map
 *
 * \return void
 */
void initStats(Statistics *stats, char *mapPath, char *networkPath);

/**
 * \fn void resetStats(Statistics *stats)
 * \brief Reset a strucutre statistics with default values
 * 
 * \param stats : structure to reset
 *
 * \return void
 */
void resetStats(Statistics *stats);

/**
 * \fn void endStatsComputations(Statistics *stats)
 * \brief Computes the final values of the statistics, like averages
 * 
 * \param stats : statistics to finalize
 *
 * \return void
 */
void endStatsComputations(Statistics *stats);

/**
 * \fn char *getLastElementOfString(char *path, const char delimiters)
 * \brief Returns the last element of a string, given the delimiters between elements and the number of elements
 * 
 * \param str : string to split
 * \param delimiters : delimiters between each element
 * 
 * \return char* : The last element of the string
 */
char *getLastElementOfString(char *str, const char delimiters);

#endif
