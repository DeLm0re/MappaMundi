using UnityEngine;
using System.Collections;
using System;
using System.IO;

using Crosstales.FB;
using B83.Image.BMP;

public class EnvironmentHandler : MonoBehaviour 
{
    public Pathfinding path;

    private EnvironmentMeshHandler envMeshHandler;

    public int width;
    public int height;

    public string seed;
    public bool useRandomSeed;

    private int[,] map;

    private string filePath;

    void Start()
    {
        this.map = new int[width, height];
        this.filePath = "none";

        CreateMap();
    }
    public void CreateMap()
    {
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
            this.map[0, heightIndex] = (int)Definition.pointEnum.WALL;
            this.map[width-1, heightIndex] = (int)Definition.pointEnum.WALL;
        }

        for(int widthIndex = 0; widthIndex < width; widthIndex++)
        {
            this.map[widthIndex, height-1] = (int)Definition.pointEnum.WALL;
            this.map[widthIndex, 0] = (int)Definition.pointEnum.WALL;
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
                this.map[x,y] = (pseudoRandom.Next(1,100) < 50)? (int)Definition.pointEnum.WALL : (int)Definition.pointEnum.EMPTY;
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
                sumNeighbours = this.map[w-1, h-1] + this.map[w-1, h] + this.map[w, h-1] + this.map[w-1, h+1]
                                + this.map[w+1, h-1] + this.map[w+1, h] + this.map[w, h+1] + this.map[w+1, h+1];

                //Clean some obstacles
                if(sumNeighbours < 2)
                {
                    this.map[w, h] = (int)Definition.pointEnum.EMPTY;
                }
                //Prevent stuck
                else if(sumNeighbours >= 4)
                {
                    this.map[w, h] = (int)Definition.pointEnum.EMPTY;
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
                sumNeighbours = this.map[w, h-1] + this.map[w-1, h] + this.map[w, h+1] + this.map[w+1, h];

                if(sumNeighbours >= 3)
                {
                    this.map[w, h] = (int)Definition.pointEnum.WALL;
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

    public void CreateCustomMap()
    {
        int i, j;
        
        if(this.filePath == "none")
        {
            this.filePath = FileBrowser.OpenSingleFile("bmp");
        }
        Texture2D texture = null;
        byte[] fileData = File.ReadAllBytes(this.filePath);

        BMPLoader bmpLoader = new BMPLoader();

        //Load the BMP data
        BMPImage bmpImg = bmpLoader.LoadBMP(fileData);

        //Convert the Color32 array into a Texture2D
        texture = bmpImg.ToTexture2D();

        for(i = 0; i < texture.width; i++)
        {
            for(j = 0; j < texture.height; j++)
            {
                if(texture.GetPixel(i, j).r == 0)
                {
                    this.map[i, j] = (int)Definition.pointEnum.WALL;
                }
                else
                {
                    this.map[i, j] = (int)Definition.pointEnum.EMPTY;
                }
            }
        }
        
        //Draw the map using mesh
        envMeshHandler = GetComponent<EnvironmentMeshHandler>();
        envMeshHandler.GenerateMeshWall(map, 1);
    }

    public void setFilePath(string value)
    {
        this.filePath = value;
    }

    public string getFilePath()
    {
        return this.filePath;
    }
}