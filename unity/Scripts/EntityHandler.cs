using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EntityHandler : MonoBehaviour
{
    public EnvironmentHandler environment;

    public FogHandler fogHandler;

    public Pathfinding path;

    public GeneticAlgorithm genetic;

    public int visionRange;

    private float waitSecondToMove;

    private int countFrame;

    private int diameter;

    private bool pause;

    private Node startNode;

    private Node endNode;

    private int xEntity;
    private int yEntity;

    private int height;
    private int width;

    private int[,] map;

    private int[,] mentalMap;

    private Point[,] fieldOfView;

    void Start()
    {
        initializationAttributs();
        //Set : map, diameter, width, height, mentalMap/fieldOfView vide, pathToFollow, start node, xEntity, yEntity, waitSecondToMove, countFrame

        //Actualize the position of the entity
        actualizePositionEntity();

        //Update the fieldOfView for the start
        updateFieldOfViewEntity();

        //Update the fieldOfView for the start
        updateMentalMapEntity();

        //Draw the fog using mesh and the mentalMap updated
        fogHandler.CreateFog(mentalMap);
    }

    //FixedUpdate is called every 0.02s
    void FixedUpdate()
    {
        this.countFrame++;

        if(Time.deltaTime * this.countFrame >= this.waitSecondToMove)
        {
            if(pause == false)
            {
                //If the entity reach the end of the path
                if(this.xEntity == endNode.getX() && this.yEntity == endNode.getY())
                //if(path.getPathStack().Count == 0)
                {
                    //Generate a new map, path and attributs
                    newGeneration();

                    //Actualize the position of the entity
                    actualizePositionEntity();

                    //Update the fieldOfView for the start
                    updateFieldOfViewEntity();

                    //Update the fieldOfView for the start
                    updateMentalMapEntity();

                    //Create the starting fog of the new map
                    fogHandler.CreateFog(mentalMap);
                }
                else
                {
                    if(path.getPathStack().Count == 0)
                        genetic.findNextPathGN(this.endNode);
                        //path.findPathFromStartEnd(this.startNode, this.endNode, this.map);

                    //Calcul the next position of the entity to follow the path
                    calculNextPosition();

                    //Actualize the position of the entity
                    actualizePositionEntity();

                    //Update the fieldOfView for the start
                    updateFieldOfViewEntity();

                    //Update the fieldOfView for the start
                    updateMentalMapEntity();
                    
                    //Destroy the mesh of the last frame
                    fogHandler.getFogMeshHandler().DestructMesh();
                    //Draw the actual frame's fog
                    fogHandler.CreateFog(mentalMap);
                }
            }
            this.countFrame = 0;
        }
    }

    public void actualizePositionEntity()
    {
        //Set the entity position in game scene
        this.transform.localPosition = new Vector3(this.xEntity + 0.5f, 0.5f, this.yEntity + 0.5f);
    }

    private void initializationAttributs()
    {
        int i, j;

        this.pause = false;

        this.countFrame = 0;

        this.map = environment.getMap();
        this.width = this.map.GetLength(0);
        this.height = this.map.GetLength(1);

        this.diameter = 2*this.visionRange + 1;

        this.fieldOfView = new Point[this.diameter,this.diameter];
        for(i = 0; i < this.diameter; i++)
        {
            for(j = 0; j < this.diameter; j++)
            {
                this.fieldOfView[i,j] = new Point();
            }
        }
        initialiseFieldOfViewEntity();

        this.mentalMap = new int[this.width,this.height];
        initializeMentalMapEntity();

        startNode = path.nearestNode(this.map, 0, 0);
        endNode = path.nearestNode(this.map,this.width-1,this.height-1);

        this.xEntity = startNode.getX();
        this.yEntity = startNode.getY();
    }

    private void calculNextPosition()
    {
        if(path.getPathStack().Count != 0)
        {
            Node nodeToGo = (Node)path.getPathStack().Pop();
            this.xEntity = nodeToGo.getX();
            this.yEntity = nodeToGo.getY();
        }
    }

    public void newGeneration()
    {
        //Destroy the mesh of the last map
        environment.getEnvironmentMeshHandler().DestructMesh();
        //Destroy the mesh of the last fog
        fogHandler.getFogMeshHandler().DestructMesh();
        environment.CreateMap();

        path.cleanPath();
        //path.findPathFromStartEnd(path.nearestNode(this.map,0,0), path.nearestNode(this.map,this.width-1,this.height-1), this.map);

        this.generateNewAttributs();
    }

    private void generateNewAttributs()
    {
        this.map = environment.getMap();
        this.width = this.map.GetLength(0);
        this.height = this.map.GetLength(1);

        initialiseFieldOfViewEntity();
        initializeMentalMapEntity();

        startNode = path.nearestNode(this.map, 0, 0);
        endNode = path.nearestNode(this.map,this.width-1,this.height-1);

        this.xEntity = startNode.getX();
        this.yEntity = startNode.getY();
    }

    private void initializeMentalMapEntity()
    {
        int widthIndex, heightIndex;

        for(widthIndex = 1; widthIndex < (this.width - 1); widthIndex++)
        {
            for(heightIndex = 1; heightIndex < (this.height - 1); heightIndex++)
            {
                this.mentalMap[widthIndex,heightIndex] = (int)Definition.pointEnum.FOG;
            }
        }

        for(heightIndex = 0; heightIndex < this.height; heightIndex++)
        {
            this.mentalMap[0,heightIndex] = (int)Definition.pointEnum.WALL;
            this.mentalMap[width-1,heightIndex] = (int)Definition.pointEnum.WALL;
        }

        for(widthIndex = 0; widthIndex < this.width; widthIndex++)
        {
            this.mentalMap[widthIndex,height-1] = (int)Definition.pointEnum.WALL;
            this.mentalMap[widthIndex,0] = (int)Definition.pointEnum.WALL;
        }
    }

    private void initialiseFieldOfViewEntity()
    {
        int iFieldOfView;
        int jFieldOfView;
        
        for(iFieldOfView = 0; iFieldOfView < this.diameter; iFieldOfView++)
        {
            for(jFieldOfView = 0; jFieldOfView < this.diameter; jFieldOfView++)
            {
                this.fieldOfView[jFieldOfView,iFieldOfView].setXPoint(jFieldOfView);
                this.fieldOfView[jFieldOfView,iFieldOfView].setYPoint(iFieldOfView);
                this.fieldOfView[jFieldOfView,iFieldOfView].setPointValue(Definition.pointEnum.FOG);
            }
        }
    }

    public void updateMentalMapEntity()
    {
        int widthIndex, heightIndex;

        for(widthIndex = 0; widthIndex < this.diameter; widthIndex++)
        {
            for(heightIndex = 0; heightIndex < this.diameter; heightIndex++)
            {
                int pointWidth = this.fieldOfView[widthIndex,heightIndex].getXPoint();
                int pointHeight = this.fieldOfView[widthIndex,heightIndex].getYPoint();
                Definition.pointEnum pointValue = this.fieldOfView[widthIndex,heightIndex].getPointValue();

                if(pointWidth >= 0 && pointWidth < this.width && pointHeight >= 0 && pointHeight < this.height)
                {
                    int mentalMapPoint = this.mentalMap[pointWidth,pointHeight];

                    //Check if the old value of the point was FOG before updating it
                    if(mentalMapPoint == (int)Definition.pointEnum.FOG)
                    {
                        this.mentalMap[pointWidth,pointHeight] = (int)pointValue; 
                    }
                    
                    this.mentalMap[this.xEntity,this.yEntity] = (int)Definition.pointEnum.VISITED;
                }
            }
        }
    }

    public void updateFieldOfViewEntity()
    {
        int i;
        int j;

        for(i = 0; i < this.diameter; i++)
        {
            for(j = 0; j < this.diameter; j++)
            {
                this.fieldOfView[j,i].setXPoint(j);
                this.fieldOfView[j,i].setYPoint(i);
                this.fieldOfView[j,i].setPointValue(Definition.pointEnum.FOG);
            }
        }

        // ================

        double angle;
        double stepAngle = 0.1; //tan(1 / entity->visionRange);
        double stepDistance = 0.4;
        double distance;
        bool aWall;

        for(angle = 0; angle < 2*Definition.M_PI; angle += stepAngle)
        {
            aWall = false;

            for(distance = 0; distance < visionRange; distance += stepDistance)
            {
                if(!aWall)
                {
                    double x = distance * Math.Cos(angle);
                    double y = distance * Math.Sin(angle);

                    if( ((this.xEntity + x) >= 0) && ((this.xEntity + x) < this.width) &&
                        ((this.yEntity + y) >= 0) && ((this.yEntity + y) < this.height) )
                    {
                        int widthFieldOfView = (int)(visionRange + x);
                        int heightFieldOfView = (int)(visionRange + y);

                        if( (widthFieldOfView >= 0) && (widthFieldOfView < 2*visionRange + 1) 
                            &&  (heightFieldOfView >= 0) && (heightFieldOfView < 2*visionRange + 1) )
                        {
                            Definition.pointEnum currentValue;

                            this.fieldOfView[widthFieldOfView,heightFieldOfView].setXPoint((int)(this.xEntity + x));
                            this.fieldOfView[widthFieldOfView,heightFieldOfView].setYPoint((int)(this.yEntity + y));

                            currentValue = (Definition.pointEnum)(this.map[(int)(this.xEntity + x),(int)(this.yEntity + y)]);
                            this.fieldOfView[widthFieldOfView,heightFieldOfView].setPointValue(currentValue);

                            if(currentValue == Definition.pointEnum.WALL)
                            {
                                aWall = true;
                            }
                        }
                    }
                }
            }
        }
    }

    public bool getPause()
    {
        return this.pause;
    }

    public void setPause(bool value)
    {
        this.pause = value;
    }

    public float getWaitSecondToMove()
    {
        return this.waitSecondToMove;
    }

    public void setWaitSecondToMove(float value)
    {
        this.waitSecondToMove = value;
    }

    public int[,] getMentalMap()
    {
        return this.mentalMap;
    }

    public int getPointFromMentalMap(int x, int y)
    {
        return this.mentalMap[x,y];
    }

    public int getXEntity()
    {
        return this.xEntity;
    }

    public int getYEntity()
    {
        return this.yEntity;
    }

}
