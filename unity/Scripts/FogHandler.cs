using UnityEngine;
using System.Collections;
using System;

public class FogHandler : MonoBehaviour 
{
    private FogMeshHandler fogMeshHandler;

    void Start()
    {

    }

    public void CreateFog(int[,] mentalMap)
    {
        //Draw the map using mesh
        fogMeshHandler = GetComponent<FogMeshHandler>();
        fogMeshHandler.GenerateMeshFog(mentalMap, 1);
    }

    public FogMeshHandler getFogMeshHandler()
    {
        return this.fogMeshHandler;
    }
}