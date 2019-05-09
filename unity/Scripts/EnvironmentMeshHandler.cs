using UnityEngine;
using System.Collections.Generic;

[RequireComponent(typeof (MeshFilter))]
[RequireComponent(typeof (MeshRenderer))]
public class EnvironmentMeshHandler : MonoBehaviour
{
	public List<Vector3> vertices;
    public List<int> triangles;

	private Mesh mesh;
	
	public void GenerateMeshWall(int[,] map, float cubeSize)
    {
		for (int x = 0; x < map.GetLength(0); x ++)
		{
			for (int y = 0; y < map.GetLength(1); y ++)
			{
				if(map[x, y] == (int)Definition.pointEnum.WALL)
				{
					CreateCube(x, y, cubeSize, map.GetLength(0), map.GetLength(1));
				}
			}
		}

		mesh = new Mesh();
        GetComponent<MeshFilter>().mesh = mesh;

        mesh.vertices = this.vertices.ToArray();
        mesh.triangles = this.triangles.ToArray();
        mesh.RecalculateNormals();
    }

	public void DestructMesh()
	{
		if(mesh != null)
			mesh.Clear();
		if(vertices.Count != 0)
			vertices.Clear();
		if(triangles.Count != 0)
			triangles.Clear();
	}

	private void CreateCube(int originX, int originY, float cubeSize, int width, int height)
    {
		Vector3 vector = new Vector3(originX, 0, originY);
        Node node0 = new Node ( vector );

		vector.Set(originX + cubeSize, 0,originY);
		Node node1 = new Node ( vector );

		vector.Set(originX + cubeSize, cubeSize, originY);
		Node node2 = new Node ( vector );

		vector.Set(originX, cubeSize, originY);
		Node node3 = new Node ( vector );

		vector.Set(originX, cubeSize, originY + cubeSize);
		Node node4 = new Node ( vector );

		vector.Set(originX + cubeSize, cubeSize, originY + cubeSize);
		Node node5 = new Node ( vector );

		vector.Set(originX + cubeSize, 0, originY + cubeSize);
		Node node6 = new Node ( vector );

		vector.Set(originX, 0, originY + cubeSize);
		Node node7 = new Node ( vector );

		AssignVertices(node0);
		AssignVertices(node1);
		AssignVertices(node2);
		AssignVertices(node3);
		AssignVertices(node4);
		AssignVertices(node5);
		AssignVertices(node6);
		AssignVertices(node7);

		CreateFace(node0, node2, node1, node0, node3, node2);
		CreateFace(node2, node3, node4, node2, node4, node5);
		CreateFace(node1, node2, node5, node1, node5, node6);
		CreateFace(node0, node7, node4, node0, node4, node3);
		CreateFace(node5, node4, node7, node5, node7, node6);
		CreateFace(node0, node6, node7, node0, node1, node6);
	}

	public class Node {
        public Vector3 position;
        public int vertexIndex = -1;

        public Node(Vector3 _pos) {
            position = _pos;
        }
    }

	private void CreateTriangle(Node a, Node b, Node c)
	{
        this.triangles.Add(a.vertexIndex);
        this.triangles.Add(b.vertexIndex);
        this.triangles.Add(c.vertexIndex);
    }

	private void CreateFace(Node a, Node b, Node c, Node d, Node e, Node f)
	{
		CreateTriangle(a, b, c);
		CreateTriangle(d, e, f);
	}

    private void AssignVertices(Node aNode)
	{
        if (aNode.vertexIndex == -1)
		{
            aNode.vertexIndex = vertices.Count;
            this.vertices.Add(aNode.position);
        }
    }
}
