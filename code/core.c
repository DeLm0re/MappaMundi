//Header file containing the prototypes
#include "core.h" 

void initialiseField(field unField){

    int i;
    int j;

    for(i = 0; i < 50; i++)
    {
        for(j = 0; j < 50; j++)
        {
            unField[i][j] = 0;
        }
    }

}