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

typedef enum {NB_STEPS = 0, NB_FOG_REVEALED, EXECUTION_TIME, SIZEOFSTAT} StatIndex;

typedef struct Statistics
{
    const char *mapId, *nnId;
    float data[SIZEOFSTAT];
    clock_t startTime, endTime;
} Statistics;

bool fileExists(const char * fileName);
bool writeStatsIntoFile(Statistics *stats, char *path);

void startClock(Statistics *stats);
void endClock(Statistics *stats);

void initStats(Statistics *stats, const char *mapName, const char *networkName);
char *getLastElementOfString(const char *path, const char *delimiters, int nbElements);

#endif