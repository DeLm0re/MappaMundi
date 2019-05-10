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

typedef enum StatIndex {NB_STEPS = 0, NB_FOG_REVEALED, EXECUTION_TIME, SIZEOFSTAT};

typedef struct Statistics
{
    float data[SIZEOFSTAT];
} Statistics;

bool writeStatsIntoFile(Statistics *stats, char *path, bool fileIsNew);

#endif