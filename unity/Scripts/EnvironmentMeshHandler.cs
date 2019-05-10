using UnityEngine;
using System.Collections.Generic;

[RequireComponent(typeof (MeshFilter))]
[RequireComponent(typeof (MeshRenderer))]
public class EnvironmentMeshHandler : MonoBehaviour
{
	private List<GameObject> listCube;

	void Start()
	{
		this.listCube = new List<GameObject>();
	}

	public void GenerateMeshWall(int[,] map, float cubeSize)
    {
		for (int x = 0; x < map.GetLength(0); x ++)
		{
			for (int y = 0; y < map.GetLength(1); y ++)
			{
				if(map[x, y] == (int)Definition.pointEnum.WALL)
				{
					CreateCube(x, y, cubeSize);
				}
			}
		}
    }

	public void DestructMesh()
	{
		foreach(GameObject element in listCube)
		{
			element.GetComponent<BuildMesh>().getMesh().Clear();
			Destroy(element);
		}
		this.listCube.Clear();
	}

	private void CreateCube(int originX, int originY, float cubeSize)
    {
		GameObject cube = new GameObject("Wall");
		cube.AddComponent(typeof(MeshFilter));
		cube.AddComponent(typeof(MeshRenderer));
		cube.AddComponent(typeof(BuildMesh));

		cube.GetComponent<BuildMesh>().Build(originX, originY, cubeSize);

		this.listCube.Add(cube);
	}
}