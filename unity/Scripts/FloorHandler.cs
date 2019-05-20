using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FloorHandler : MonoBehaviour
{
    public EnvironmentHandler environment;

    void Start()
    {
        floorInitialization();
    }

    private void floorInitialization()
    {
        int[,] map = environment.getMap();
        
        float width = map.GetLength(0);
        float height = map.GetLength(1);

        GameObject aFloor = GameObject.Find("Floor");

        aFloor.transform.localScale = new Vector3(width, 0.1f, height);
        aFloor.transform.localPosition = new Vector3(width/2, -0.1f, height/2);
    }
}
