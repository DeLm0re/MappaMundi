//Header files
#include <time.h>
#include <math.h>

//Header file containing the prototypes
#include "core.h"

void initialiseField(field oneField){

    int i;
    int j;

    for(i = 0; i < 50; i++)
    {
        for(j = 0; j < 50; j++)
        {
            oneField[i][j] = 0;
        }
    }
}

void generateEnv(field oneField) {

    int i;
    int j;
    srand(time(NULL));
    int monRand;

    for(i = 0; i < 50; i++)
    {
        for(j = 0; j < 50; j++)
        {
            monRand = rand()%10 + 1;

            if(monRand < 5)
            {
                oneField[i][j] = 1;
            }
        }
    }
}