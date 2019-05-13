using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using UnityEditor;

public class GeneticAlgorithm : MonoBehaviour
{
    public EntityHandler entity;

    public Pathfinding path;

    private double[,] interestField;

    private int widthInterestField;

    private int heightInterestField;

    private double[] labelingWeights;

    void Start()
    {
        initializeGeneticAlgorithmAttributs();
        loadLabelingWeights();
        printLabelingWeights();
    }

    private void initializeGeneticAlgorithmAttributs()
    {
        this.widthInterestField = entity.getMentalMap().GetLength(0);
        this.heightInterestField = entity.getMentalMap().GetLength(1);
        this.interestField = new double[widthInterestField, heightInterestField];
        initialiseInterestField();

        this.labelingWeights = new double[10];
        initialiseLabelingWeights();
    }

    private void initialiseInterestField()
    {
        int widthIndex;
        int heightIndex;

        for(widthIndex = 0; widthIndex < this.widthInterestField; widthIndex++)
        {
            for(heightIndex = 0; heightIndex < this.heightInterestField; heightIndex++)
            {
                this.interestField[widthIndex,heightIndex] = 0;
            }
        }
    }

    private void initialiseLabelingWeights()
    {
        int i;
        System.Random pseudoRandom = new System.Random(Time.time.ToString().GetHashCode());

        for(i = 0; i < this.labelingWeights.GetLength(0); i++)
        {
            this.labelingWeights[i] = ((double)(pseudoRandom.NextDouble()%1000))/1000-0.5;
        }
    }

    private void initialiseCurrentFieldOfView(int[,] aField)
    {
        int i;
        int j;

        for(i = 0; i < aField.GetLength(0); i++)
        {
            for(j = 0; j < aField.GetLength(1); j++)
            {
                aField[i,j] = (int)(Definition.pointEnum.UNDEFINED);
            }
        }
    }

    private void loadLabelingWeights()
    {
        string path = EditorUtility.OpenFilePanel("Select Genetic Network", "", "gn");

        if (path.Length != 0)
        {
            int i;
            var fileContent = File.ReadAllBytes(path);

            for(i = 0; i < 10; i++)
            {
                this.labelingWeights[i] = BitConverter.ToDouble(fileContent,i*8);

            }
        }
    }

    private void printLabelingWeights()
    {   
        int i;

        for(i = 0; i < 10; i++)
        {
            print(this.labelingWeights[i]);
        }
    }
/*
    private Stack findNextPathGN(Node endNode)
    {
        Node startNode = new Node(entity.getXEntity(), entity.getYEntity(), 0, 0);

        //We update each values of the interest field with what our neural network think
        updateInterestField2(interestField, endNode->x, endNode->y, entity, labelingWeights);
        
        //We set a default wanted node
        Node wantedPosition = endNode;
        
        //Use to store the path found by the pathfinding
        Stack path = new Stack();
        //We try to find a path
        while(path.Count == 0)
        {
            //We try to find a path
            path = findPathFromStartEnd(startNode, wantedPosition, entity->mentalMap, &(data->endEvent));
            //If we haven't find a path
            if ((path == startNode || path == NULL))
            {
                //We change our wanted node to the best position found by the neural network
                updateBestWantedPosition(wantedPosition, interestField);
            }
        }
        destructInterestField(&interestField);
        destructNodes(&wantedPosition);

        return path;
    }

    void updateInterestField2(InterestField* interestField, int xEnd, int yEnd, Entity* entity, LabelingWeights* labelingWeights)
    {
        if (interestField != NULL && entity != NULL && labelingWeights != NULL)
        {
            int width, height;
            for(width = 0; width < interestField->width; width++)
            {
                for(height = 0; height < interestField->height; height++)
                {
                    Field* fieldOfView = getFieldOfViewFromMap(entity->mentalMap, width, height, entity->visionRange);
                    
                    interestField->data[width][height] = labeling3(fieldOfView, width, height, xEnd, yEnd, entity, labelingWeights);
                    
                    destructField(&fieldOfView);
                }
            }
        }
    }

    void updateBestWantedPosition(node* wantedPosition, InterestField* interestField)
    {
        if (wantedPosition != NULL && interestField != NULL)
        {
            float bestPoint = -INFINITY;
            wantedPosition->x = 0;
            wantedPosition->y = 0;
            int width, height;
            for(width = 0; width < interestField->width; width++)
            {
                for(height = 0; height < interestField->height; height++)
                {
                    
                    if (interestField->data[width][height] > bestPoint)
                    {
                        bestPoint = interestField->data[width][height];
                        wantedPosition->x = width;
                        wantedPosition->y = height;
                    }
                }
            }
            interestField->data[wantedPosition->x][wantedPosition->y] = -INFINITY;
        }
    }
*/
    private double labeling3(int[,] aField, int xPosition, int yPosition, int xFinalPosition, int yFinalPosition)
    {
        double emptyPoint = 0;
        double wallPoint = 0;
        double fogPoint = 0;
        double visitedPoint = 0;
        double dist = 0;
        double avgDistEmpty = 0;
        double avgDistWall = 0;
        double avgDistFog = 0;
        double avgDistVisited = 0;
        double distFromEntity = 0;
        double value = 0;
        double centerPointx = (aField.GetLength(0)-1)/2;
        double centerPointy = (aField.GetLength(1)-1)/2;

        if (aField[(int)centerPointx,(int)centerPointy] != (int)Definition.pointEnum.EMPTY) 
        {
            return -Definition.INFINITY;
        }
        
        for(int width = 0; width < aField.GetLength(0); width++)
        {
            for(int height = 0; height < aField.GetLength(1); height++)
            {
                switch (aField[width,height])
                {
                    case (int)Definition.pointEnum.EMPTY:
                        avgDistEmpty += Math.Sqrt(Math.Pow(width-centerPointx,2) + Math.Pow(height-centerPointy,2));
                        emptyPoint++;
                        break;
                    case (int)Definition.pointEnum.WALL:
                        avgDistWall += Math.Sqrt(Math.Pow(width-centerPointx,2) + Math.Pow(height-centerPointy,2));
                        wallPoint++;
                        break;
                    case (int)Definition.pointEnum.FOG:
                        if(isVisibleFrom(aField, (aField.GetLength(0)-1)/2, (aField.GetLength(1)-1)/2, width, height))
                        {
                            avgDistFog += Math.Sqrt(Math.Pow(width-centerPointx,2) + Math.Pow(height-centerPointy,2));
                            fogPoint++;
                        }
                        break;
                    case (int)Definition.pointEnum.VISITED:
                        avgDistVisited += Math.Sqrt(Math.Pow(width-centerPointx,2) + Math.Pow(height-centerPointy,2));
                        visitedPoint++;
                        break;
                }
            }
        }

        dist = Math.Sqrt(Math.Pow(xFinalPosition-xPosition,2) + Math.Pow(yFinalPosition-yPosition,2));
        distFromEntity = Math.Sqrt(Math.Pow(entity.getXEntity()-xPosition,2) + Math.Pow(entity.getYEntity()-yPosition,2));

        avgDistEmpty /= emptyPoint;
        avgDistWall /= wallPoint;
        avgDistFog /= fogPoint;
        avgDistVisited /= visitedPoint;

        value = dist*labelingWeights[(int)Definition.weightsEnum.DIST]+
                emptyPoint*labelingWeights[(int)Definition.weightsEnum.NB_EMPTY]+
                wallPoint*labelingWeights[(int)Definition.weightsEnum.NB_WALL]+
                fogPoint*labelingWeights[(int)Definition.weightsEnum.NB_FOG]+
                visitedPoint*labelingWeights[(int)Definition.weightsEnum.NB_VISITED]+
                avgDistEmpty*labelingWeights[(int)Definition.weightsEnum.AVG_DIST_EMPTY]+
                avgDistWall*labelingWeights[(int)Definition.weightsEnum.AVG_DIST_WALL]+
                avgDistFog*labelingWeights[(int)Definition.weightsEnum.AVG_DIST_FOG]+
                avgDistVisited*labelingWeights[(int)Definition.weightsEnum.AVG_DIST_VISITED]+
                distFromEntity*labelingWeights[(int)Definition.weightsEnum.DIST_FROM_ENTITY];

        return value;
    }

    private bool isVisibleFrom(int[,] fieldOfView, int xOrigin, int yOrigin, int xPosition, int yPosition)
    {
        double angle = Math.Atan2(yPosition - yOrigin, xPosition - xOrigin);

        double dist = Math.Sqrt(Math.Pow(xPosition-xOrigin, 2) + Math.Pow(yPosition-yOrigin, 2));

        double[] vect = new double[2];
        vect[0] = Math.Cos(angle);
        vect[1] = Math.Sin(angle);

        bool isVisible = true;

        for(int i = 0; i < dist; i++)
        {
            double x = xOrigin + i * vect[0];
            double y = yOrigin + i * vect[1];

            if ( ((int) x) != xPosition && ((int) y) != yPosition)
            {
                if (fieldOfView[(int) x,(int) y] == (int)Definition.pointEnum.WALL)
                {
                    isVisible = false;
                }
            }
        }
        return isVisible;
    }

    public int[,] getFieldOfViewFromMap(int x, int y)
    {
        int[,] aField = new int[entity.visionRange*2 + 1, entity.visionRange*2 + 1];
        // If the mental map exist and if the vision range is greater than 0
        if(entity.getMentalMap() != null && entity.visionRange > 0)
        {
            // We initialize the field of view
            initialiseCurrentFieldOfView(aField);
            int radiusSquare = entity.visionRange * entity.visionRange; // use to know the maximum distance a tile can have to be in the vision range
            double distanceSquare; // use to know the distance of a tile from the x and y coordinate
            //For each tile that could be in the vision range (a square around the x and y coordinate)
            int width, height;
            for(width = x - entity.visionRange; width <= x + entity.visionRange; width++)
            {
                for(height = y - entity.visionRange; height <= y + entity.visionRange; height++)
                {
                    //We calculate his distance to the x and y coordinate
                    distanceSquare = Math.Pow(width - x, 2) + Math.Pow(height - y, 2);
                    //If it is in the vision range
                    if(distanceSquare < radiusSquare)
                    {
                        // If it doesn't go out of bounds
                        if (0 < width && width < entity.getMentalMap().GetLength(0) && 0 < height && height < entity.getMentalMap().GetLength(1))
                        {
                            // We add it to the mental map
                            aField[width - x + entity.visionRange,height - y + entity.visionRange] = entity.getPointFromMentalMap(width,height);
                        }
                    }
                }
            }
        }
        return aField;
    }
}
