/**
 * \file pathfinding.h
 * \brief Prototypes of the A* pathfinding algorithme
 * \author Louis Schuck
 * \version 0.1
 * \date 11/10/2018
 *
 * Module that contain all prototypes of function in pathfinding.c
 *
 */

#ifndef H_PATHFINDING
	#define H_PATHFINDING

//Header file
#include "core.h"

//The structure node used in the A* algorithme (used in core.c)
typedef struct node 
{
	int x;
	int y;
	int cost;
	int heuristic;
	struct node* linkedNode;
}node;

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
node* initNode(int x, int y, int cost, int heuristic);

/**
 * \fn node* nearestNode(Field oneField, int height, int width, int x, int y)
 * \brief function that return the nearest and safest node around the node which is used as a starting point for the pathfinding
 * But this node can be a WALL. A node is a structure used in the A* algorithme
 * A node can be used to create a chain list of node
 *
 * \param x, y : the coordinate of the node used as a starting point for the pathfinding
 * \param oneField : the field in which we search a safe node
 * \param height, width : the height and the width of this specific field
 * \return node*
 */
node* nearestNode(Field oneField, int height, int width, int x, int y);

/**
 * \fn void destructNodes(node** frontNode)
 * \brief function used to free all the nodes in a chain list of nodes
 *
 * \param frontNode : the top node of a chain list of node
 * \return void
 */
void destructNodes(node** frontNode);

/**
 * \fn void setHeuristic(node* nodeToSet, node* endNode)
 * \brief function which is used to set the heuristic of a node
 * 
 * \param nodeToSet : the node you want the heuristic to be set correctly
 * \param endNode : the end node of the A* algorithme
 * \return int
 */
void setHeuristic(node* nodeToSet, node* endNode);

/**
 * \fn node* popNode(node** frontNode)
 * \brief function which pop the top node out of a chain list of node and returns it
 *
 * \param frontNode : the top node of a chain list of node
 * \return node*
 */
node* popNode(node** frontNode);

/**
 * \fn void rmvNode(node** frontNode, node* nodeToRemove)
 * \brief function which pop out a given node from a chain list of node
 *
 * \param frontNode : the top node of a chain list of node
 * \param nodeToRemove : the node from the list that you want to pop out
 * \return void
 */
void rmvNode(node** frontNode, node* nodeToRemove);

/**
 * \fn node* cpyNode(node* nodeToCpy)
 * \brief function which initialize and return a copy of the given node
 *
 * \param nodeToCpy : the node wich will be copied
 * \return node*
 */
node* cpyNode(node* nodeToCpy);

/**
 * \fn void insertFrontNode(node** frontNode, node* newNode)
 * \brief function which add a node at the top of a chain list of node
 *
 * \param frontNode : the top node of a chain list of node
 * \param newNode : the node that you want to add
 * \return void
 */
void insertFrontNode(node** frontNode, node* newNode);

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
void viewNodes(node** frontNode, SDL_Renderer* renderer, SDL_Color color, int tileSize);

/**
 * \fn int nodeCmp(node* n1, node* n2)
 * \brief function used to compare the heuristic of two nodes
 * returns 1 if the fisrt one has better heuristic than the second one, 0 if their equals, -1 otherwise
 *
 * \param n1, n2 : the two nodes which will be compared
 * \return int
 */
int nodeCmp(node* n1, node* n2);

/**
 * \fn int distNodes(node* n1, node* n2)
 * \brief function which return the square of the distance between the two given nodes
 * 
 * \param n1, n2 : the two nodes between which we want to know the distance
 * \return int
 */
int distNodes(node* n1, node* n2);

/**
 * \fn bool isInSet(node** set, int x, int y)
 * \brief function which check if their is a node at the given coordinates in the given chain list of nodes
 * return true or false
 * 
 * \param frontNode : the top node of a chain list of node
 * x, y : the coordinate to check
 * \return bool
 */
bool isInSet(node** frontNode, int x, int y);

/**
 * \fn bool isNextTo(node* nodeToCheck, int x, int y)
 * \brief function which check if the given node is next to the given coordinate
 * 
 * \param nodeToCheck : the node which will be checked
 * \param x, y : the reference coordinate
 * \return bool
 */
bool isNextTo(node* nodeToCheck, int x, int y);

/**
 * \fn node* getLowestNode(node** openSet)
 * \brief function which returns the node with the best heuristic (the lowest)
 * 
 * \param openSet : the chain list of nodes which represent the openSet of the A* algorithme
 * \return node*
 */
node* getLowestNode(node** openSet);

/**
 * \fn node* getPath(node** closedSet)
 * \brief function which create a chain list of nodes which represent the obtimised path
 * using the closedSet of a A* algorithme
 * 
 * \param path : the chain list of nodes which represent the path of the A* algorithme
 * \param index : the node position of the node you want to return from the path
 * \return node*
 */
node* getPath(node** closedSet, node* endNode);

/**
 * \fn node* getPath(node** closedSet)
 * \brief function which create a chain list of nodes which represent the obtimised path
 * using the closedSet of a A* algorithme
 * 
 * \param closedSet : the chain list of nodes which represent the closeSet of the A* algorithme
 * \param endNode : the end node of the A* algorithme
 * \return node*
 */
node* getNode(node** path, int index);

/**
 * \fn void addNeighbors(node** openSet, node** closedSet, node* currentNode, node* endNode, int fieldHeight, int fieldWidth)
 * \brief function which adds and create the neighboors of a given node in the openSet of the A* algorithme
 * It adds neighbors only if they are not in the closedSet or the openSet
 * 
 * \param openSet : the chain list of nodes which represent the openSet of the A* algorithme
 * \param closedSet : the chain list of nodes which represent the closeSet of the A* algorithme
 * \param currentNode : the reference node used to create the neighbors
 * \param endNode : the end node of the A* algorithme
 * \param theField : the field used to see if it is a valid neighbor or not
 * \param fieldHeight : the total number of rows of the map we use
 * \param fieldWidth : the total number of columns of the map we use
 * \return void
 */
void addNeighbors(node** openSet, node** closedSet, node* currentNode, node* endNode, Field theField, int fieldHeight, int fieldWidth);

/**
 * \fn node* AStar(node** openSet, node** closedSet, node* startNode, node* endNode, int fieldHeight, int fieldWidth)
 * \brief function which do one step of the A* algorithme
 * once the path has been found, returns the complete path. OtherWise return NULL. If no path possible, returns startNode
 * 
 * \param openSet : the chain list of nodes which represent the openSet of the A* algorithme
 * \param closedSet : the chain list of nodes which represent the closeSet of the A* algorithme
 * \param startNode : the starting node of the A* algorithme
 * \param endNode : the end node of the A* algorithme
 * \param theField : the field used to see where the path can go
 * \param fieldHeight : the total number of rows of the map we use
 * \param fieldWidth : the total number of columns of the map we use
 * \return node*
 */
node* AStar(node** openSet, node** closedSet, node* startNode, node* endNode, Field theField, int fieldHeight, int fieldWidth);

/**
 * \fn node* findPathFrom_To_(node* startNode, node* endNode, Field theField, int fieldHeight, int fieldWidth, bool* endEvent)
 * \brief function that finds the path between two points (start, end)
 * returns the complete path. OtherWise return NULL. If no path possible, returns startNode
 * 
 * \param startNode : the starting node of the A* algorithme
 * \param endNode : the end node of the A* algorithme
 * \param theField : the field used to see where the path can go
 * \param fieldHeight : the total number of rows of the map we use
 * \param fieldWidth : the total number of columns of the map we use
 * \param endEvent : pointer to boolean that will trigger the end of the function. Put NULL if there is none
 * \return node*
 */
node* findPathFrom_To_(node* startNode, node* endNode, Field theField, int fieldHeight, int fieldWidth, bool* endEvent);

#endif
