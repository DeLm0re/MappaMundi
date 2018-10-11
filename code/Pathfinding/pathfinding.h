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

//General header files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//More specific headers files
#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>

//The structure node used in the A* algorithme
typedef struct node 
{
	int x;
	int y;
	int cost;
	int heuristic;
	struct node* linkedNode;
}node;

node* initNode(int x, int y, int cost, int heuristic);
void destructNodes(node** frontNode);
node* popNode(node** frontNode);
void rmvNode(node** frontNode, node* nodeToRemove);
void insertEndNode(node** frontNode, node* newNode);
void viewNodes(node** frontNode, SDL_Renderer* renderer, SDL_Color col);
int nodeCmp(node* n1, node* n2);
int distNodes(node* n1, node* n2);
void setHeuristic(node* nodeToSet, node* endNode);
node* getLowestNode(node** openSet);
bool isInSet(node** frontNode, int x, int y);

void addNeighbors(node** openSet, node** closedSet, node* currentNode, node* endNode, int mapHeight, int mapWidth);

int AStar(node** openSet, node** closedSet, node* startNode, node* endNode, int mapHeight, int mapWidth);
