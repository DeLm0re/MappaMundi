using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Pathfinding : MonoBehaviour
{
    private List<Node> openList;
    private List<Node> closeList;

    private Stack pathStack;

    public Pathfinding()
    {
        this.openList = new List<Node>();
        this.closeList = new List<Node>();
        this.pathStack = new Stack();
    }

    public Node nearestNode(int[,] map, int x, int y)
    {
        int xNode = x;
        int yNode = y;

        int i;
        int j;

        int neighboursOrder = 1;
        bool findNode = false;

        int widthField = map.GetLength(0);
        int heightField = map.GetLength(1);

        while(findNode == false)
        {
            for(i = (x-neighboursOrder); i <= (x+neighboursOrder); i++)
            {
                for(j = (y-neighboursOrder); j <= (y+neighboursOrder); j++)
                {
                    if((i >= 0) && (i < widthField) && (j >= 0) && (j <  heightField))
                    {
                        if(map[i,j] == (int)Definition.pointEnum.EMPTY)
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

        Node returnNode = new Node(xNode, yNode, 0, 0);
        return returnNode;
    }

    private void setHeuristic(Node nodeToSet, Node endNode)
    {
	    //The formula to calculate the heuristic of a node is : his cost + distance to the end
	    nodeToSet.setHeuristic(nodeToSet.getCost() + distNodes(endNode, nodeToSet));
    }

    private Node pop(List<Node> aList)
    {
        if (aList.Count > 0)
        {
            Node tempNode = aList[aList.Count - 1];
            aList.RemoveAt(aList.Count - 1);
            return tempNode;
        }
        else
            return default(Node);
    }

    public void removeByObject(List<Node> aList, Node aNode)
    {
        int nodePosition = aList.IndexOf(aNode);
        aList.RemoveAt(nodePosition);
    }

    private double distNodes(Node node1, Node node2)
    {
        //Using Pythagor's theorem we calculate the square of the distance between the two nodes
        return Math.Pow((node1.getX() - node2.getX()), 2) + Math.Pow((node1.getY() - node2.getY()), 2);
    }

    private bool isInSet(List<Node> aList, int x, int y)
    {
        //For each node in the chain list
        foreach(Node element in aList)
        {
            //We check if their is a node with the given coordinates
            if(element.getX() == x && element.getY() == y)
            {
                //We return true
                return true;
            }
        }
        //If we find nothing, we return false;
        return false;
    }

    private bool isNextTo(Node nodeToCheck, int x, int y)
    {
        //We walculate the distance between the given node and the given coordinate in the x and y axis
        int xdist = Math.Abs(nodeToCheck.getX() - x);
        int ydist = Math.Abs(nodeToCheck.getY() - y);
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

    private Node getLowestNode()
    {
        Node theNode = this.openList[0];

        //Return the value of the node with the min heuristic
        foreach(Node element in this.openList)
        {
            if(element.getHeuristic() < theNode.getHeuristic())
            {
                theNode = element;
            }
        }
        return theNode;
    }

    private void getPath(Node endNode)
    {
        //First we create a chain list wich will contain the path
        Node temp = endNode;

        //This variable is used to keep track of the current cost we are searching
        int currentCost = 0;

        //We begin by searching in the closedSet the node which is at the end poitn coordinate
        foreach(Node element in this.closeList)
        {
            if(element.getX() == temp.getX() && element.getY() == temp.getY())
            {
                //Once find, we save this node for later
                temp = element;
                //And we update the current cost
                currentCost = temp.getCost();
                break;
            }
        }
        //We insert a copy of this node to our path
        this.pathStack.Push(temp);

        //Then, while the cost as still not reach 0 (understand : we didn't backtrack to the starting point yet)
        while(currentCost > 0)
        {
            //We check in all the chain list for a node with a cost directly inferior to the current cost
            //And next to the last node added to the path
            foreach(Node element in this.closeList)
            {
                if(element.getCost() == currentCost - 1 && isNextTo(element, temp.getX(), temp.getY()))
                {
                    //Once find we save the node for later
                    temp = element;
                    //And we update the cost
                    currentCost--;
                    /*And most importantly, we stop the loop
                    Their is usually different nodes who correspond to this description and we want only one of them
                    All these nodes are equivalent if we want the most optimize path*/
                    break;
                }
            }
            //Then we insert the node into the path
            this.pathStack.Push(temp);
        }
        /*The logic behind the reconstruction of the path is simple : 
        -we start with the last node of the path
        -we find a neighbour with a cost directly inferior of the last added node
        -we add it
        -repeat until the cost reaches 0
        */
    }

    private void addNeighbors(Node currentNode, Node endNode, int[,] map)
    {
        int x = currentNode.getX();
        int y = currentNode.getY();
        Node temp = null;

        int widthField = map.GetLength(0);
        int heightField = map.GetLength(1);
        
        /*For each neighbors we check if :
        -It is inside of the field
        -It is not in the closed or open set
        -It is in an empty tile
        */
        if (x-1 >= 0 && !isInSet(this.closeList, x-1, y) && !isInSet(this.openList, x-1, y) && 
            (map[x-1,y] == (int)Definition.pointEnum.EMPTY || map[x-1,y] == (int)Definition.pointEnum.VISITED))
        {
            //If everything is verify we create it
            temp = new Node(x-1, y, currentNode.getCost() + 1, 0);
            //We set his heuristic
            setHeuristic(temp, endNode);
            //We insert it in the openSet
            this.openList.Add(temp);
        }
        // etc.
        if (x+1 < widthField && !isInSet(this.closeList, x+1, y) && !isInSet(this.openList, x+1, y) && 
            (map[x+1,y] == (int)Definition.pointEnum.EMPTY || map[x+1,y] == (int)Definition.pointEnum.VISITED))
        {
            temp = new Node(x+1, y, currentNode.getCost() + 1, 0);
            setHeuristic(temp, endNode);
            this.openList.Add(temp);
        }
        if (y-1 >= 0 && !isInSet(this.closeList, x, y-1) && !isInSet(this.openList, x, y-1) && 
            (map[x,y-1] == (int)Definition.pointEnum.EMPTY || map[x,y-1] == (int)Definition.pointEnum.VISITED))
        {
            temp = new Node(x, y-1, currentNode.getCost() + 1, 0);
            setHeuristic(temp, endNode);
            this.openList.Add(temp);
        }
        if (y+1 < heightField && !isInSet(this.closeList, x, y+1) && !isInSet(this.openList, x, y+1) && 
            (map[x,y+1] == (int)Definition.pointEnum.EMPTY || map[x,y+1] == (int)Definition.pointEnum.VISITED))
        {
            temp = new Node(x, y+1, currentNode.getCost() + 1, 0);
            setHeuristic(temp, endNode);
            this.openList.Add(temp);
        }
    }

    private void AStar(Node startNode, Node endNode, int[,] map)
    {
        //If their is no nodes left in the openSet
        if(this.openList.Count == 0)
        {
            //We return just the starting node, which means their is no path
            this.pathStack.Push(startNode);
        }

        //We get the lowest heuristic node in the open set
        Node lowestNode = getLowestNode();
        //We remove it from the open set
        removeByObject(this.openList, lowestNode);
        //We insert it in the closed set
        this.closeList.Add(lowestNode);
        //If it's in the end node coordinate
        if (lowestNode.getX() == endNode.getX() && lowestNode.getY() == endNode.getY())
        {
            //Update reconstructed path
            getPath(endNode);
        }
        else
        {
            //Otherwise, we add the neighbors into the open set
            addNeighbors(lowestNode, endNode, map);
        }
    }

    public void findPathFromStartEnd(Node startNode, Node endNode, int[,] map)
    {       
        //We give to the openSet a starting point
        this.openList.Add(startNode);

        while(this.pathStack.Count == 0)
        {
            //We do one step of A* algorithme
            AStar(startNode, endNode, map);
        }
    }

    public void printPath()
    {
        Array myNodeArray = Array.CreateInstance(typeof(Node), this.pathStack.Count);
        this.pathStack.CopyTo(myNodeArray, 0);

        foreach(Node element in myNodeArray)
        {
            Debug.Log("X: " + element.getX());
            Debug.Log("Y: " + element.getY());
            Debug.Log("======================");
        }
    }

    public void cleanPath()
    {
        this.openList.Clear();
        this.closeList.Clear();
        this.pathStack.Clear();
    }

    public Stack getPathStack()
    {
        return this.pathStack;
    }
}
