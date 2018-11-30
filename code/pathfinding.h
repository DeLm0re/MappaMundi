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

//Initialisation and destruction function
node* initNode(int x, int y, int cost, int heuristic);
node* nearestNode(Field oneField, int height, int width, int x, int y);
void destructNodes(node** frontNode);
void setHeuristic(node* nodeToSet, node* endNode);

//Manipulation function
node* popNode(node** frontNode);
void rmvNode(node** frontNode, node* nodeToRemove);
node* cpyNode(node* nodeToCpy);
void insertFrontNode(node** frontNode, node* newNode);

//Display function
void viewNodes(node** frontNode, SDL_Renderer* renderer, SDL_Color color, int tileSize);

//Information function
int nodeCmp(node* n1, node* n2);
int distNodes(node* n1, node* n2);
bool isInSet(node** frontNode, int x, int y);
bool isNextTo(node* nodeToCheck, int x, int y);

//A* algorithme function
node* getLowestNode(node** openSet);
node* getPath(node** closedSet, node* endNode);
void addNeighbors(node** openSet, node** closedSet, node* currentNode, node* endNode, Field theField, int fieldHeight, int fieldWidth);
node* AStar(node** openSet, node** closedSet, node* startNode, node* endNode, Field theField, int fieldHeight, int fieldWidth);
