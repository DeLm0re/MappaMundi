//The structure entity used as a token which will be control by our neural network
typedef struct Entity
{
    int x;
    int y;
    int visionRange;
    //The Field of the mental map is to be added with his dimmension
    //The list of the valid visible position is to be added
    //The neural network structure is to be added
} Entity;

//Initialisation and destruction function
Entity* initialiseEntity(int x, int y, int visionRange);
void destructEntity(Entity** entity);