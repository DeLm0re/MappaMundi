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

//Header file
#include "core.h"

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
node* initNode(int x, int y, int cost, int heuristic);
node* nearestNode(Field oneField, int height, int width, int x, int y);
void destructNodes(node** frontNode);
void setHeuristic(node* nodeToSet, node* endNode);

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
node* popNode(node** frontNode);
void rmvNode(node** frontNode, node* nodeToRemove);
node* cpyNode(node* nodeToCpy);
void insertFrontNode(node** frontNode, node* newNode);

/**
 * \fn void destructNodes(node** frontNode)
 * \brief function used to free all the nodes in a chain list of nodes
 *
 * \param frontNode : the top node of a chain list of node
 * \return void
 */
void viewNodes(node** frontNode, SDL_Renderer* renderer, SDL_Color color, int tileSize);

/**
 * \fn void setHeuristic(node* nodeToSet, node* endNode)
 * \brief function which is used to set the heuristic of a node
 * 
 * \param nodeToSet : the node you want the heuristic to be set correctly
 * \param endNode : the end node of the A* algorithme
 * \return int
 */
int nodeCmp(node* n1, node* n2);
int distNodes(node* n1, node* n2);
bool isInSet(node** frontNode, int x, int y);
bool isNextTo(node* nodeToCheck, int x, int y);

/**
 * \fn node* popNode(node** frontNode)
 * \brief function which pop the top node out of a chain list of node and returns it
 *
 * \param frontNode : the top node of a chain list of node
 * \return node*
 */
node* getLowestNode(node** openSet);
node* getPath(node** closedSet, node* endNode);
void addNeighbors(node** openSet, node** closedSet, node* currentNode, node* endNode, Field theField, int fieldHeight, int fieldWidth);
node* AStar(node** openSet, node** closedSet, node* startNode, node* endNode, Field theField, int fieldHeight, int fieldWidth);
