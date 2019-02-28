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
 * \param x, y : the coordinate of the node
 * \param cost : the number of step since the starting node
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

/**
 * \fn node* nearestNode(Field oneField, int x, int y)
 * \brief function that return the nearest and safest node around the node which is used as a starting point for the pathfinding
 * But this node can be a WALL. A node is a structure used in the A* algorithme
 * A node can be used to create a chain list of node
 *
 * \param x, y : the coordinate of the node used as a starting point for the pathfinding
 * \param oneField* : poiter to the field in which we search a safe node
 * \return node*
 */
node* nearestNode(Field *oneField, int x, int y)
{
	int xNode = x;
	int yNode = y;
	int i;
	int j;
	int neighboursOrder = 1;
	bool findNode = false;

	if((x >= 0) && (x < oneField->width) && (y >= 0) && (y <  oneField->height))
	{
		if(oneField->data[x][y] == EMPTY)
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
				if((i >= 0) && (i < oneField->width) && (j >= 0) && (j <  oneField->height))
				{
					if(oneField->data[i][j] == EMPTY)
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
	//While their is still node in the chakn list
	while((*frontNode)->linkedNode != NULL)
	{
		//Extract the top node from the chain list and free it
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
	//The formula to calculate the heuristic of a node is : his cost + distance to the end
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
	//The node that will be extract is the top node of the chain list
	node* popedNode = *frontNode;
	//If the list isn't empty
	if(frontNode != NULL)
	{
		//We change the frontNode by the next node
		*frontNode = (*frontNode)->linkedNode;
	}
	//Then we delink the poped node before returning it
	popedNode->linkedNode = NULL;
	return popedNode;
	/*If at any moment their is a NULL pointer their is no problem. The poped node could be NULL (no node is poped)
	And the chain list could become NULL after the node was extract*/ 
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
	//If the chain list is not NULL (understand : if a node could be remove)
	if(frontNode != NULL)
	{
		node* cursor = *frontNode;
		//We check independently if the fisrt node is the node to remove because we will not check it later
		if(cursor == nodeToRemove)
		{
			//If the first node is the node to remove we basicly pop it
			*frontNode = cursor->linkedNode;
			/*We delink it from the chain list. We can do it on nodeToRemove because we are checking the pointer
			So the cursor is the nodeToRemove. We could have used the cursor insted*/
			nodeToRemove->linkedNode = NULL;
		}
		else
		{
			/*If the front node is not the node to remove, we check all the chain list and so all the linked node
			that is why we needed to checj the first node independently, because it is linked to zero node*/
			while(cursor->linkedNode != NULL)
			{
				//If the node pointed by the cursor is the node to remove
				if(cursor->linkedNode == nodeToRemove)
				{
					//We change the link (basicly we jump to the next linked node)
					cursor->linkedNode = cursor->linkedNode->linkedNode;
					//We delink it from the chain list
					nodeToRemove->linkedNode = NULL;
					break;
				}
				cursor = cursor->linkedNode;
			}
		}
	}
	/*We do not return the removed node because we theoricaly already have its pointer via "nodeToRemove"
	In our case it is not usefull because the removed not is move to an other chain list, so it will be correctly free
	However it could be something usefull to add for other uses*/
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
	//We simply initialise a node with the same parametre of the given node and return it
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
	//If we have a node to insert
	if(newNode != NULL)
	{
		//We link the top node of the chain list to the new node we want to insert on top
		newNode->linkedNode = *frontNode;
		//We change the front node of the chain list by our new one
		*frontNode = newNode;
	}
}

/**
 * \fn void viewNodes(node** frontNode, SDL_Renderer* renderer, SDL_Color col)
 * \brief function used to draw a chain list of nodes on the screen using SDL
 *
 * \param frontNode : the top node of a chain list of node
 * \param renderer : the renderer used with the window. The renderer must have been set using SDL functions
 * \param color : the color which will be used to display the nodes
 * \param tileSize : the size of one tile in the window
 * \return void
 */
void viewNodes(node** frontNode, SDL_Renderer* renderer, SDL_Color color, int tileSize)
{
	//If the size of the tile is large enought to draw a rectangle
	if (tileSize > 2)
	{
		node* cursor = *frontNode;
		//We set the drawing color
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		//For each node in the chain list
		while(cursor != NULL)
		{
			//We draw a rectangle at the correct coordiante
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
	//Simple comparison between the heuristics of the two nodes
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
	//Using Pythagor's theorem we calculate the square of the distance between the two nodes
	return pow((n1->x - n2->x), 2) + pow((n1->y - n2->y), 2);
}

/**
 * \fn bool isInSet(node** set, int x, int y)
 * \brief function which check if their is a node at the given coordinates in the given chain list of nodes
 * return true or false
 * 
 * \param frontNode : the top node of a chain list of node
 * \param x, y : the coordinate to check
 * \return bool
 */
bool isInSet(node** frontNode, int x, int y)
{
	node* cursor = *frontNode;
	//For each node in the chain list
	while(cursor != NULL)
	{
		//We check if their is a node with the given coordinates
		if(cursor->x == x && cursor->y == y)
		{
			//We return true
			return true;
		}
		cursor = cursor->linkedNode;
	}
	//If we find nothing, we return false;
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
	//We walculate the distance between the given node and the given coordinate in the x and y axis
	int xdist = abs(nodeToCheck->x - x);
	int ydist = abs(nodeToCheck->y - y);
	//The node is next to this coordinate if one of the distances is equal to 1 (up, down, right and left)
	//If both are equals to 1, that means we're next to it diagonaly, not usefull in our case but it could be
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
	//This pointer is use to keep in memory the node with the lowest heuristic, by default it is the first node of the set
	node* lowestNode = *openSet;
	node* cursor = *openSet;
	//For each node in the set
	while(cursor != NULL)
	{
		//We check if its heuristic is lower than the current lowest node
		if(cursor->heuristic < lowestNode->heuristic)
		{
			//If it is, we update it
			lowestNode = cursor;
		}
		cursor = cursor->linkedNode;
	}
	//Once everything is checked, we return the lowest ndoe found
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
	//First we create a chain list wich will contain the path
	node* path = NULL;
	node* cursor = *closedSet;
	node* temp = endNode;
	//This variable is used to keep track of the current cost we are searching
	int currentCost = 0;

	//We begin by searching in the closedSet the node which is at the end poitn coordinate
	while(cursor != NULL)
	{
		if(cursor->x == temp->x && cursor->y == temp->y)
		{
			//Once find, we save this node for later
			temp = cursor;
			//And we update the current cost
			currentCost = temp->cost;
			break;
		}
		cursor = cursor->linkedNode;
	}
	//We insert a copy of this node to our path
	insertFrontNode(&path, cpyNode(temp));

	//Then, while the cost as still not reach 0 (understand : we didn't backtrack to the starting point yet)
	while(currentCost > 0)
	{
		//We check in all the chain list for a node with a cost directly inferior to the current cost
		//And next to the last node added to the path
		cursor = *closedSet;
		while(cursor != NULL)
		{
			if(cursor->cost == currentCost - 1 && isNextTo(cursor, temp->x, temp->y))
			{
				//Once find we save the node for later
				temp = cursor;
				//And we update the cost
				currentCost--;
				/*And most importantly, we stop the loop
				Their is usually different nodes who correspond to this description and we want only one of them
				All these nodes are equivalent if we want the most optimize path*/
				break;
			}
			cursor = cursor->linkedNode;
		}
		//Then we insert the node into the path
		insertFrontNode(&path, cpyNode(temp));
	}
	//Finaly we return the path
	return path;
	/*The logic behind the reconstruction of the path is simple : 
	-we start with the last node of the path
	-we find a neighbour with a cost directly inferior of the last added node
	-we add it
	-repeat until the cost reaches 0
	*/
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
node* getNode(node** path, int index)
{
	//The node use as a cursor to navigate
	node* cursor = *path;
	//The curent index
	int i = 0;
	//For each node in the set
	while(cursor != NULL && i != index)
	{
		i++;
		cursor = cursor->linkedNode;
	}
	//Once everything is checked, we return the lowest ndoe found
	return cursor;
}

/**
 * \fn void addNeighbors(node** openSet, node** closedSet, node* currentNode, node* endNode)
 * \brief function which adds and create the neighboors of a given node in the openSet of the A* algorithme
 * It adds neighbors only if they are not in the closedSet or the openSet
 * 
 * \param openSet : the chain list of nodes which represent the openSet of the A* algorithme
 * \param closedSet : the chain list of nodes which represent the closeSet of the A* algorithme
 * \param currentNode : the reference node used to create the neighbors
 * \param endNode : the end node of the A* algorithme
 * \param theField* : poiter to the field used to see if it is a valid neighbor or not
 * \return void
 */
void addNeighbors(node** openSet, node** closedSet, node* currentNode, node* endNode, Field *theField)
{
	int x = currentNode->x;
	int y = currentNode->y;
	node* temp;
	
	/*For each neighbors we check if :
	-It is inside of the field
	-It is not in the closed or open set
	-It is in an empty tile
	*/
	if (x-1 >= 0 && !isInSet(closedSet, x-1, y) && !isInSet(openSet, x-1, y) && theField->data[x-1][y] == EMPTY)
	{
		//If everything is verify we create it
		temp = initNode(x-1, y, currentNode->cost + 1, 0);
		//We set his heuristic
		setHeuristic(temp, endNode);
		//We insert it in the openSet
		insertFrontNode(openSet, temp);
	}
	// etc.
	if (x+1 < theField->width && !isInSet(closedSet, x+1, y) && !isInSet(openSet, x+1, y) && theField->data[x+1][y] == EMPTY)
	{
		temp = initNode(x+1, y, currentNode->cost + 1, 0);
		setHeuristic(temp, endNode);
		insertFrontNode(openSet, temp);
	}
	if (y-1 >= 0 && !isInSet(closedSet, x, y-1) && !isInSet(openSet, x, y-1) && theField->data[x][y-1] == EMPTY)
	{
		temp = initNode(x, y-1, currentNode->cost + 1, 0);
		setHeuristic(temp, endNode);
		insertFrontNode(openSet, temp);
	}
	if (y+1 < theField->height && !isInSet(closedSet, x, y+1) && !isInSet(openSet, x, y+1) && theField->data[x][y+1] == EMPTY)
	{
		temp = initNode(x, y+1, currentNode->cost + 1, 0);
		setHeuristic(temp, endNode);
		insertFrontNode(openSet, temp);
	}
}

/**
 * \fn node* AStar(node** openSet, node** closedSet, node* startNode, node* endNode)
 * \brief function which do one step of the A* algorithme
 * once the path has been found, returns the complete path. OtherWise return NULL. If no path possible, returns startNode
 * 
 * \param openSet : the chain list of nodes which represent the openSet of the A* algorithme
 * \param closedSet : the chain list of nodes which represent the closeSet of the A* algorithme
 * \param startNode : the starting node of the A* algorithme
 * \param endNode : the end node of the A* algorithme
 * \param theField* : pointer the field used to see where the path can go
 * \return node*
 */
node* AStar(node** openSet, node** closedSet, node* startNode, node* endNode, Field *theField)
{
	//If their is no nodes left in the openSet
	if(*openSet == NULL)
	{
		//We return just the starting node, which means their is no path
		return startNode;
	}

	//We get the lowest heuristic node in the open set
	node* lowestNode = getLowestNode(openSet);
	//We remove it from the open set
	rmvNode(openSet, lowestNode);
	//We insert it in the closed set
	insertFrontNode(closedSet, lowestNode);
	//If it's in the end node coordinate
	if (lowestNode->x == endNode->x && lowestNode->y == endNode->y)
	{
		//We return the reconstructed path
		return getPath(closedSet, endNode);
	}
	else
	{
		//Otherwise, we add the neighbors into the open set
		addNeighbors(openSet, closedSet, lowestNode, endNode, theField);
		return NULL;
	}
}

/**
 * \fn node* findPathFrom_To_(node* startNode, node* endNode, Field theField, bool* endEvent)
 * \brief function that finds the path between two points (start, end)
 * returns the complete path. OtherWise return NULL. If no path possible, returns startNode
 * 
 * \param startNode : the starting node of the A* algorithme
 * \param endNode : the end node of the A* algorithme
 * \param theField* : pointer to the field used to see where the path can go
 * \param endEvent : pointer to boolean that will trigger the end of the function. Put NULL if there is none
 * \return node*
 */
node* findPathFrom_To_(node* startNode, node* endNode, Field *theField, bool* endEvent)
{
	node* path = NULL; //Used to store a path
	node* openSet = NULL; //Used to store the openSet for the A* algorithm
	node* closedSet = NULL; //Used to store the closedSet for the A* algorithm

	bool* endPathfinding = NULL; // Used to know if we need to interrupt the loop

	//If an endEvent was given as an argument
	if (endEvent != NULL) 
	{
		//We use it as a reference to end the loop
		endPathfinding = endEvent;
	}
	//Otherwise
	else
	{
		//We use a dummy boolean set to false
		bool valueEndPathfinding = false;
		endPathfinding = &valueEndPathfinding;
	}
	
	//We give to the openSet a starting point
	insertFrontNode(&openSet, cpyNode(startNode));
	while (path == NULL && *endPathfinding == false)
	{
		//We do one step of A* algorithme
		path = AStar(&openSet, &closedSet, startNode, endNode, theField);
	}

	//We free the openSet and closedSet from the memory
	destructNodes(&openSet);
	destructNodes(&closedSet);

	//We return the path we found
	return path;
}

