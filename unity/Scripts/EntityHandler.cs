using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EntityHandler : MonoBehaviour
{
    public EnvironmentHandler environment;

    public FogHandler fogHandler;

    public Pathfinding path;

    public int visionRange;

    private int diameter;

    private Node startNode;

    private Stack pathToFollow;

    private int xEntity;
    private int yEntity;

    private int height;
    private int width;

    private int[,] map;

    private int[,] mentalMap;

    private Point[,] fieldOfView;

    void Start()
    {
        entityInitialization();

        updateFieldOfViewEntity();
        updateMentalMapEntity();

        //Draw the fog using mesh 
        fogHandler.CreateFog(mentalMap);
    }

    void FixedUpdate()
    {
        if(pathToFollow != null)
        {
            if(pathToFollow.Count == 0)
            {
                newPathToFollow();

                updateFieldOfViewEntity();
                updateMentalMapEntity();

                //Draw the fog using mesh 
                fogHandler.getFogMeshHandler().DestructMesh();
                fogHandler.CreateFog(mentalMap);
            }
            else
            {
                followPathOneStep();

                updateFieldOfViewEntity();
                updateMentalMapEntity();

                fogHandler.getFogMeshHandler().DestructMesh();
                fogHandler.CreateFog(mentalMap);
            }
        }
    }

    public void entityInitialization()
    {
        this.map = environment.getMap();
        this.width = this.map.GetLength(0);
        this.height = this.map.GetLength(1);

        this.diameter = 2*this.visionRange + 1;

        initialiseFieldOfViewEntity();
        initializeMentalMapEntity();

        pathToFollow = path.getPathStack();

        startNode = path.nearestNode(this.map, 0, 0);

        this.xEntity = startNode.getX();
        this.yEntity = startNode.getY();

        this.transform.localPosition = new Vector3(this.xEntity + 0.5f, 0.5f, this.yEntity + 0.5f);
    }

    private void followPathOneStep()
    {
        Node nodeToGo;

        nodeToGo = (Node)pathToFollow.Pop();

        this.xEntity = nodeToGo.getX();
        this.yEntity = nodeToGo.getY();

        this.transform.localPosition = new Vector3(this.xEntity + 0.5f, 0.5f, this.yEntity + 0.5f);
    }

    private void newPathToFollow()
    {
        environment.getEnvironmentMeshHandler().DestructMesh();
        Array.Clear(this.fieldOfView, 0, this.fieldOfView.Length);
        Array.Clear(this.mentalMap, 0, this.mentalMap.Length);
        environment.CreateMap();

        path.cleanPath();
        path.findPathFromStartEnd(path.nearestNode(this.map,0,0), path.nearestNode(this.map,this.width-1,this.height-1), this.map);

        this.entityInitialization();
    }

    private void initializeMentalMapEntity()
    {
        int widthIndex, heightIndex;

        this.mentalMap = new int[this.width,this.height];

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

        this.fieldOfView = new Point[this.diameter,this.diameter];
        
        for(iFieldOfView = 0; iFieldOfView < this.diameter; iFieldOfView++)
        {
            for(jFieldOfView = 0; jFieldOfView < this.diameter; jFieldOfView++)
            {
                this.fieldOfView[jFieldOfView,iFieldOfView] = new Point();
                this.fieldOfView[jFieldOfView,iFieldOfView].setXPoint(jFieldOfView);
                this.fieldOfView[jFieldOfView,iFieldOfView].setYPoint(iFieldOfView);
                this.fieldOfView[jFieldOfView,iFieldOfView].setPointValue(Definition.pointEnum.FOG);
            }
        }
    }

    private void updateMentalMapEntity()
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

    private void updateFieldOfViewEntity()
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

}
