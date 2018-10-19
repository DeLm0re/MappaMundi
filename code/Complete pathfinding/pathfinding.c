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
 * \param heuristic : the cost + the distance between the ending node and the node initialized
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

node* nearestNode(Field oneField, int height, int width, int x, int y)
{
	int xNode = x;
	int yNode = y;
	int i;
	int j;
	int neighboursOrder = 1;
	bool findNode = false;

	if((x >= 0) && (x < width) && (y >= 0) && (y <  height))
	{
		if(oneField[x][y] == EMPTY)
		{
			return initNode(x, y, 0, 0);
		}
	}

	while(findNode == false)
	{
		for(i = (x-neighboursOrder); i <= (x+neighboursOrder); i++)
		{
			for(j = (y-neighboursOrder); j <= (y+neighboursOrder); j++)
			{
				if((i >= 0) && (i < width) && (j >= 0) && (j <  height))
				{
					if(oneField[i][j] == EMPTY)
					{
						xNode = i;
						yNode = j;
						findNode = true;
					}
				}
			}
		}

		neighboursOrder++;
	}	

	return initNode(xNode, yNode, 0, 0);
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
 * \fn void setHeuristic(node* nodeToSet, node* endNode)
 * \brief function which is used to set the heuristic of a node
 * 
 * \param nodeToSet : the node you want the heuristic to be set correctly
 * \param endNode : the end node of the A* algorithme
 * \return int
 */
void setHeuristic(node* nodeToSet, node* endNode)
{
	nodeToSet->heuristic = nodeToSet->cost + distNodes(endNode, nodeToSet);
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
 * \param nodeToRemove : the node from the list that you want to pop out
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
 * \fn node* cpyNode(node* nodeToCpy)
 * \brief function which initialize and return a copy of the given node
 *
 * \param nodeToCpy : the node wich will be copied
 * \return node*
 */
node* cpyNode(node* nodeToCpy)
{
	node* newNode = initNode(nodeToCpy->x, nodeToCpy->y, nodeToCpy->cost, nodeToCpy->heuristic);
	return newNode;
}

/**
 * \fn void insertFrontNode(node** frontNode, node* newNode)
 * \brief function which add a node at the top of a chain list of node
 *
 * \param frontNode : the top node of a chain list of node
 * \param newNode : the node that you want to add
 * \return void
 */
void insertFrontNode(node** frontNode, node* newNode)
{
	if(newNode != NULL)
	{
		newNode->linkedNode = *frontNode;
		*frontNode = newNode;
	}
}

/**
 * \fn void viewNodes(node** frontNode, SDL_Renderer* renderer, SDL_Color col)
 * \brief function used to draw a chain list of nodes on the screen using SDL
 *
 * \param frontNode : the top node of a chain list of node
 * \param renderer : the renderer used with the window. The renderer must have been set using SDL functions
 * \param col : the color which will be used to display the nodes
 * \return void
 */
void viewNodes(node** frontNode, SDL_Renderer* renderer, SDL_Color color, int tileSize)
{
	if (tileSize > 2)
	{
		node* cursor = *frontNode;
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		while(cursor != NULL)
		{
			SDL_RenderFillRect(renderer, &((SDL_Rect) {
				cursor->x*tileSize + 1, 
				cursor->y*tileSize + 1, 
				tileSize-2, 
				tileSize-2}));
			cursor = cursor->linkedNode;
		}
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
 * \fn bool isNextTo(node* nodeToCheck, int x, int y)
 * \brief function which check if the given node is next to the given coordinate
 * 
 * \param nodeToCheck : the node which will be checked
 * \param x, y : the reference coordinate
 * \return bool
 */
bool isNextTo(node* nodeToCheck, int x, int y)
{
	int xdist = abs(nodeToCheck->x - x);
	int ydist = abs(nodeToCheck->y - y);
	if (xdist + ydist == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
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
 * \fn node* getPath(node** closedSet)
 * \brief function which create a chain list of nodes which represent the obtimised path
 * using the closedSet of a A* algorithme
 * 
 * \param closedSet : the chain list of nodes which represent the closeSet of the A* algorithme
 * \param endNode : the end node of the A* algorithme
 * \return node*
 */
node* getPath(node** closedSet, node* endNode)
{
	node* path = NULL;
	node* cursor = *closedSet;
	node* temp = endNode;
	int currentCost = 0;
	while(cursor != NULL)
	{
		if(cursor->x == temp->x && cursor->y == temp->y)
		{
			temp = cursor;
			currentCost = temp->cost;
			break;
		}
		cursor = cursor->linkedNode;
	}
	insertFrontNode(&path, cpyNode(temp));

	while(currentCost > 0)
	{
		cursor = *closedSet;
		while(cursor != NULL)
		{
			if(cursor->cost == currentCost - 1 && isNextTo(cursor, temp->x, temp->y))
			{
				temp = cursor;
				currentCost--;
				break;
			}
			cursor = cursor->linkedNode;
		}
		insertFrontNode(&path, cpyNode(temp));
	}
	return path;
}

/**
 * \fn void addNeighbors(node** openSet, node** closedSet, node* currentNode, node* endNode, int fieldHeight, int fieldWidth)
 * \brief function which adds and create the neighboors of a given node in the openSet of the A* algorithme
 * It adds neighbors only if they are not in the closedSet or the openSet
 * 
 * \param openSet : the chain list of nodes which represent the openSet of the A* algorithme
 * \param closedSet : the chain list of nodes which represent the closeSet of the A* algorithme
 * \param currentNode : the reference node used to create the neighbors
 * \param endNode : the end node of the A* algorithme
 * \param fieldHeight : the total number of rows of the map we use
 * \param fieldWidth : the total number of columns of the map we use
 * \return bool
 */
void addNeighbors(node** openSet, node** closedSet, node* currentNode, node* endNode, Field theField, int fieldHeight, int fieldWidth)
{
	int x = currentNode->x;
	int y = currentNode->y;
	node* temp;
	
	// Ajouter des collisions ici
	if (x-1 >= 0 && !isInSet(closedSet, x-1, y) && !isInSet(openSet, x-1, y) && theField[x-1][y] == EMPTY)
	{
		temp = initNode(x-1, y, currentNode->cost + 1, 0);
		setHeuristic(temp, endNode);
		insertFrontNode(openSet, temp);
	}
	if (x+1 < fieldWidth && !isInSet(closedSet, x+1, y) && !isInSet(openSet, x+1, y) && theField[x+1][y] == EMPTY)
	{
		temp = initNode(x+1, y, currentNode->cost + 1, 0);
		setHeuristic(temp, endNode);
		insertFrontNode(openSet, temp);
	}
	if (y-1 >= 0 && !isInSet(closedSet, x, y-1) && !isInSet(openSet, x, y-1) && theField[x][y-1] == EMPTY)
	{
		temp = initNode(x, y-1, currentNode->cost + 1, 0);
		setHeuristic(temp, endNode);
		insertFrontNode(openSet, temp);
	}
	if (y+1 < fieldHeight && !isInSet(closedSet, x, y+1) && !isInSet(openSet, x, y+1) && theField[x][y+1] == EMPTY)
	{
		temp = initNode(x, y+1, currentNode->cost + 1, 0);
		setHeuristic(temp, endNode);
		insertFrontNode(openSet, temp);
	}
}

/**
 * \fn int AStar(node** openSet, node** closedSet, node* startNode, node* endNode, int fieldHeight, int fieldWidth)
 * \brief function which do one step of the A* algorithme
 * once the path has been found, returns the complete path. OtherWise return NULL
 * 
 * \param openSet : the chain list of nodes which represent the openSet of the A* algorithme
 * \param closedSet : the chain list of nodes which represent the closeSet of the A* algorithme
 * \param startNode : the starting node of the A* algorithme
 * \param endNode : the end node of the A* algorithme
 * \param fieldHeight : the total number of rows of the map we use
 * \param fieldWidth : the total number of columns of the map we use
 * \return int
 */
node* AStar(node** openSet, node** closedSet, node* startNode, node* endNode, Field theField, int fieldHeight, int fieldWidth)
{
	if(*openSet == NULL)
	{
		return startNode;
	}

	node* lowestNode = getLowestNode(openSet);
	rmvNode(openSet, lowestNode);
	insertFrontNode(closedSet, lowestNode);
	if (lowestNode->x == endNode->x && lowestNode->y == endNode->y)
	{
		return getPath(closedSet, endNode);
	}
	else
	{
		addNeighbors(openSet, closedSet, lowestNode, endNode, theField, fieldHeight, fieldWidth);
		return NULL;
	}
}
