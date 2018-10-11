/**
 * \file pathfinding.c
 * \brief A* pathfinding algorythme
 * \author Louis Schuck
 * \version 0.1
 * \date 11/10/2018
 *
 * Module that contain all the functions to do a A* pathfinding algorithme
 *
 */

//Header files containing the prototypes
#include "pathfinding.h"

/**
 * \fn node* initNode(int x, int y, int cost, int heuristic)
 * \brief function that initialise a node. A node is a structure used in the A* algorithme
 * A node can be used to create a chain list of node
 *
 * \param x, y : the coordinate of the node, cost : the number of step since the starting node
 * heuristic : the cost + the distance between the ending node and the node initialized
 * the function setHeuristic can calculate and set the heuristic of a node so you can just put 0.
 * \return node*
 */
node* initNode(int x, int y, int cost, int heuristic)
{
	node* newNode = malloc(sizeof(node));
	newNode->x = x;
	newNode->y = y;
	newNode->cost = cost;
	newNode->heuristic = heuristic;
	newNode->linkedNode = NULL;
	return newNode;
}

/**
 * \fn void destructNodes(node** frontNode)
 * \brief function used to free all the nodes in a chain list of nodes
 *
 * \param frontNode : the top node of a chain list of node
 * \return void
 */
void destructNodes(node** frontNode)
{
	while((*frontNode)->linkedNode != NULL)
	{
		free(popNode(frontNode));
	}
}

/**
 * \fn node* popNode(node** frontNode)
 * \brief function which pop the top node out of a chain list of node and returns it
 *
 * \param frontNode : the top node of a chain list of node
 * \return node*
 */
node* popNode(node** frontNode)
{
	node* popedNode = *frontNode;
	if(frontNode != NULL)
	{
		*frontNode = (*frontNode)->linkedNode;
	}
	popedNode->linkedNode = NULL;
	return popedNode;
}

/**
 * \fn void rmvNode(node** frontNode, node* nodeToRemove)
 * \brief function which pop out a given node from a chain list of node
 *
 * \param frontNode : the top node of a chain list of node
 * nodeToRemove : the node from the list that you want to pop out
 * \return void
 */
void rmvNode(node** frontNode, node* nodeToRemove)
{
	if(frontNode != NULL)
	{
		node* cursor = *frontNode;
		if(cursor == nodeToRemove)
		{
			*frontNode = cursor->linkedNode;
			nodeToRemove->linkedNode = NULL;
		}
		else
		{
			while(cursor->linkedNode != NULL)
			{
				if(cursor->linkedNode == nodeToRemove)
				{
					cursor->linkedNode = cursor->linkedNode->linkedNode;
					nodeToRemove->linkedNode = NULL;
					break;
				}
				cursor = cursor->linkedNode;
			}
		}
	}
}

/**
 * \fn void insertEndNode(node** frontNode, node* newNode)
 * \brief function which add a node at the bottom of a chain list of node
 *
 * \param frontNode : the top node of a chain list of node
 * newNode : the node that you want to add
 * \return void
 */
void insertEndNode(node** frontNode, node* newNode)
{
	//frontNode = newNode;
	if(*frontNode == NULL)
	{
		*frontNode = newNode;
	}
	else
	{
		node* temp = *frontNode;
		while(temp->linkedNode != NULL)
		{
			temp = temp->linkedNode;
		}
		temp->linkedNode = newNode;
	}
}

/**
 * \fn void viewNodes(node** frontNode, SDL_Renderer* renderer, SDL_Color col)
 * \brief function used to draw a chain list of nodes on the screen using SDL
 *
 * \param frontNode : the top node of a chain list of node
 * renderer : the renderer used with the window. The renderer must have been set using SDL functions
 * col : the color which will be used to display the nodes
 * \return void
 */
void viewNodes(node** frontNode, SDL_Renderer* renderer, SDL_Color col)
{
	node* temp = *frontNode;
	SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
	while(temp != NULL)
	{
		SDL_RenderFillRect(renderer, &((SDL_Rect) {temp->x*10 + 1, temp->y*10 + 1, 8, 8}));
		temp = temp->linkedNode;
	}
}

/**
 * \fn int nodeCmp(node* n1, node* n2)
 * \brief function used to compare the heuristic of two nodes
 * returns 1 if the fisrt one has better heuristic than the second one, 0 if their equals, -1 otherwise
 *
 * \param n1, n2 : the two nodes which will be compared
 * \return int
 */
int nodeCmp(node* n1, node* n2)
{
	if (n1->heuristic < n2->heuristic)
	{
		return 1;
	}
	else if (n1->heuristic == n2->heuristic)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

/**
 * \fn int distNodes(node* n1, node* n2)
 * \brief function which return the square of the distance between the two given nodes
 * 
 * \param n1, n2 : the two nodes between which we want to know the distance
 * \return int
 */
int distNodes(node* n1, node* n2)
{
	return pow((n1->x - n2->x), 2) + pow((n1->y - n2->y), 2);
}

/**
 * \fn void setHeuristic(node* nodeToSet, node* endNode)
 * \brief function which is used to set the heuristic of a node
 * 
 * \param nodeToSet : the node you want the heuristic to be set correctly
 * endNode : the end node of the A* algorithme
 * \return int
 */
void setHeuristic(node* nodeToSet, node* endNode)
{
	nodeToSet->heuristic = nodeToSet->cost + distNodes(endNode, nodeToSet);
}

/**
 * \fn node* getLowestNode(node** openSet)
 * \brief function which returns the node with the best heuristic (the lowest)
 * 
 * \param openSet : the chain list of nodes which represent the openSet of the A* algorithme
 * \return node*
 */
node* getLowestNode(node** openSet)
{
	node* lowestNode = *openSet;
	node* temp = *openSet;
	while(temp != NULL)
	{
		if(temp->heuristic < lowestNode->heuristic)
		{
			lowestNode = temp;
		}
		temp = temp->linkedNode;
	}
	return lowestNode;
}

/**
 * \fn bool isInSet(node** set, int x, int y)
 * \brief function which check if their is a node at the given coordinates in the given chain list of nodes
 * return true or false
 * 
 * \param frontNode : the top node of a chain list of node
 * x, y : the coordinate to check
 * \return bool
 */
bool isInSet(node** frontNode, int x, int y)
{
	node* temp = *frontNode;
	while(temp != NULL)
	{
		if(temp->x == x && temp->y == y)
		{
			return true;
		}
		temp = temp->linkedNode;
	}
	return false;
}

/**
 * \fn void addNeighbors(node** openSet, node** closedSet, node* currentNode, node* endNode, int mapHeight, int mapWidth)
 * \brief function which adds and create the neighboors of a given node in the openSet of the A* algorithme
 * It adds neighbors only if they are not in the closedSet or the openSet
 * 
 * \param openSet : the chain list of nodes which represent the openSet of the A* algorithme
 * closedSet : the chain list of nodes which represent the closeSet of the A* algorithme
 * currentNode : the reference node used to create the neighbors
 * endNode : the end node of the A* algorithme
 * mapHeight : the total number of rows of the map we use
 * mapWidth : the total number of columns of the map we use
 * \return bool
 */
void addNeighbors(node** openSet, node** closedSet, node* currentNode, node* endNode, int mapHeight, int mapWidth)
{
	int x = currentNode->x;
	int y = currentNode->y;
	node* temp;
	
	// Ajouter des collisions ici
	if (x-1 >= 0 && !isInSet(closedSet, x-1, y) && !isInSet(openSet, x-1, y))
	{
		temp = initNode(x-1, y, currentNode->cost + 1, 0);
		setHeuristic(temp, endNode);
		insertEndNode(openSet, temp);
	}
	if (x+1 < mapWidth && !isInSet(closedSet, x+1, y) && !isInSet(openSet, x+1, y))
	{
		temp = initNode(x+1, y, currentNode->cost + 1, 0);
		setHeuristic(temp, endNode);
		insertEndNode(openSet, temp);
	}
	if (y-1 >= 0 && !isInSet(closedSet, x, y-1) && !isInSet(openSet, x, y-1))
	{
		temp = initNode(x, y-1, currentNode->cost + 1, 0);
		setHeuristic(temp, endNode);
		insertEndNode(openSet, temp);
	}
	if (y+1 < mapHeight && !isInSet(closedSet, x, y+1) && !isInSet(openSet, x, y+1))
	{
		temp = initNode(x, y+1, currentNode->cost + 1, 0);
		setHeuristic(temp, endNode);
		insertEndNode(openSet, temp);
	}
}

/**
 * \fn int AStar(node** openSet, node** closedSet, node* startNode, node* endNode, int mapHeight, int mapWidth)
 * \brief function which do one step of the A* algorithme
 * returns 1 if a path has been find, -1 if their is no beginning node in the openSet, 0 otherwise
 * 
 * \param openSet : the chain list of nodes which represent the openSet of the A* algorithme
 * closedSet : the chain list of nodes which represent the closeSet of the A* algorithme
 * startNode : the starting node of the A* algorithme
 * endNode : the end node of the A* algorithme
 * mapHeight : the total number of rows of the map we use
 * mapWidth : the total number of columns of the map we use
 * \return int
 */
int AStar(node** openSet, node** closedSet, node* startNode, node* endNode, int mapHeight, int mapWidth)
{
	node* lowestNode = NULL;
	if (*openSet != NULL)
	{
		lowestNode = getLowestNode(openSet);

		if (lowestNode->x == endNode->x && lowestNode->y == endNode->y)
		{
			printf("Done !\n Start node = %p\n End node = %p\n", (void*) startNode, (void*) endNode);
			return 1;
		}
		addNeighbors(openSet, closedSet, lowestNode, endNode, mapHeight, mapWidth);
		rmvNode(openSet, lowestNode);
		insertEndNode(closedSet, lowestNode);
	}
	else{
		return -1;
	}
	return 0;
}