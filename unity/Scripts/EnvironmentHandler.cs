using UnityEngine;
using System.Collections;
using System;

public class EnvironmentHandler : MonoBehaviour 
{
    public Pathfinding path;

    private EnvironmentMeshHandler envMeshHandler;

    public int width;
    public int height;

    public string seed;
    public bool useRandomSeed;

    private int[,] map;

    void Start()
    {
        CreateMap();

        path.findPathFromStartEnd(path.nearestNode(map,0,0), path.nearestNode(map,width-1,height-1), map);
        //pathFinder.printPath();
    }

    /*
    void Update() {
        if (Input.GetKey(KeyCode.R)) {
            meshGen.DestructMesh();
            CreateMap();
        }
    }*/

    public void CreateMap()
    {
        map = new int[width,height];

        //Make the borders of the map walls
        GenerateMapBorders();

        //Generate a random Bernouilli map
        GenerateMapEnv();

        //Smooth the map environment
        SmoothMapEnv();

        //Make strong walls (fill blank in walls)
        StrongMapWalls();

        //Draw the map using mesh
        envMeshHandler = GetComponent<EnvironmentMeshHandler>();
        envMeshHandler.GenerateMeshWall(map, 1);
    }

    private void GenerateMapBorders()
    {
        for(int heightIndex = 0; heightIndex < height; heightIndex++)
        {
            map[0, heightIndex] = (int)Definition.pointEnum.WALL;
            map[width-1, heightIndex] = (int)Definition.pointEnum.WALL;
        }

        for(int widthIndex = 0; widthIndex < width; widthIndex++)
        {
            map[widthIndex, height-1] = (int)Definition.pointEnum.WALL;
            map[widthIndex, 0] = (int)Definition.pointEnum.WALL;
        }
    }


    private void GenerateMapEnv()
    {
        if (useRandomSeed)
        {
            seed = Time.time.ToString();
        }

        System.Random pseudoRandom = new System.Random(seed.GetHashCode());

        for (int x = 1; x < width-1; x ++)
        {
            for (int y = 1; y < height-1; y ++)
            {
                map[x,y] = (pseudoRandom.Next(1,100) < 50)? 1 : 0;
            }
        }
    }

    private void SmoothMapEnv()
    {
        int sumNeighbours = 0;

        for(int w = 1; w < width-1; w++)
        {
            for(int h = 1; h < height-1; h++)
            {
                sumNeighbours = map[w-1, h-1] + map[w-1, h] + map[w, h-1] + map[w-1, h+1]
                                + map[w+1, h-1] + map[w+1, h] + map[w, h+1] + map[w+1, h+1];

                //Clean some obstacles
                if(sumNeighbours < 2)
                {
                    map[w, h] = (int)Definition.pointEnum.EMPTY;
                }
                //Prevent stuck
                else if(sumNeighbours >= 4)
                {
                    map[w, h] = (int)Definition.pointEnum.EMPTY;
                }
            }
        }
    }

    private void StrongMapWalls()
    {
        int sumNeighbours;

        for(int w = 1; w < width-1; w++)
        {
            for(int h = 1; h < height-1; h++)
            {
                sumNeighbours = map[w, h-1] + map[w-1, h] + map[w, h+1] + map[w+1, h];

                if(sumNeighbours >= 3)
                {
                    map[w, h] = (int)Definition.pointEnum.WALL;
                }
            }
        }
    }

    public EnvironmentMeshHandler getEnvironmentMeshHandler()
    {
        return this.envMeshHandler;
    }

    public int[,] getMap()
    {
        return this.map;
    }
}