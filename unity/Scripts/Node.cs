using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Node
{
    private int x = -1;
    private int y = -1;
    private int cost;
    private double heuristic;

    public Node(int x, int y, int cost, double heuristic)
    {
	    this.x = x;
	    this.y = y;
	    this.cost = cost;
	    this.heuristic = heuristic;
    }

    public int getX()
    {
        return this.x;
    }

    public void setX(int xNode)
    {
        this.x = xNode;
    }

    public int getY()
    {
        return this.y;
    }

    public void setY(int yNode)
    {
        this.y = yNode;
    }

    public int getCost()
    {
        return this.cost;
    }

    public void setCost(int costNode)
    {
        this.cost = costNode;
    }

    public double getHeuristic()
    {
        return this.heuristic;
    }

    public void setHeuristic(double heuristicNode)
    {
        this.heuristic = heuristicNode;
    }
}
