using UnityEngine;
using System.Collections;
 
public class BuildMesh : MonoBehaviour 
{  
    private Mesh aMesh;

    private MeshFilter aMeshFilter;

    // Use this for initialization
    public void Build(int originX, int originY, float cubeSize) {
 
        this.aMeshFilter = GetComponent<MeshFilter>();
        this.aMesh = aMeshFilter.mesh;

		Vector3 vertLeftTopFront = new Vector3();
		Vector3 vertRightTopFront = new Vector3();
		Vector3 vertRightTopBack = new Vector3();
		Vector3 vertLeftTopBack = new Vector3();
		Vector3 vertLeftBottomFront = new Vector3();
		Vector3 vertRightBottomFront = new Vector3();
		Vector3 vertLeftBottomBack = new Vector3();
		Vector3 vertRightBottomBack = new Vector3();

		vertLeftTopFront.Set(originX + cubeSize, cubeSize, originY + cubeSize);
		vertRightTopFront.Set(originX + cubeSize, cubeSize, originY);
		vertRightTopBack.Set(originX , cubeSize, originY);
		vertLeftTopBack.Set(originX, cubeSize, originY + cubeSize);
		vertLeftBottomFront.Set(originX + cubeSize, 0, originY + cubeSize);
		vertRightBottomFront.Set(originX + cubeSize, 0, originY);
		vertLeftBottomBack.Set(originX, 0, originY + cubeSize);
		vertRightBottomBack.Set(originX, 0, originY);
 
        //Vertices//
        Vector3[] vertices = new Vector3[]
        {
            //front face//
            vertLeftTopFront,//left top front, 0
            vertRightTopFront,//right top front, 1
            vertLeftBottomFront,//left bottom front, 2
            vertRightBottomFront,//right bottom front, 3
 
            //back face//
            vertRightTopBack,//right top back, 4
            vertLeftTopBack,//left top back, 5
            vertRightBottomBack,//right bottom back, 6
            vertLeftBottomBack,//left bottom back, 7
 
            //left face//
            vertLeftTopBack,//left top back, 8
            vertLeftTopFront,//left top front, 9
            vertLeftBottomBack,//left bottom back, 10
            vertLeftBottomFront,//left bottom front, 11
 
            //right face//
            vertRightTopFront,//right top front, 12
            vertRightTopBack,//right top back, 13
            vertRightBottomFront,//right bottom front, 14
            vertRightBottomBack,//right bottom back, 15
 
            //top face//
            vertLeftTopBack,//left top back, 16
            vertRightTopBack,//right top back, 17
            vertLeftTopFront,//left top front, 18
            vertRightTopFront,//right top front, 19
 
            //bottom face//
            vertLeftBottomFront,//left bottom front, 20
            vertRightBottomFront,//right bottom front, 21
            vertLeftBottomBack,//left bottom back, 22
            vertRightBottomBack//right bottom back, 23
 
        };
 
        //Triangles// 3 points, clockwise determines which side is visible
        int[] triangles = new int[]
        {
            //front face//
            0,2,3,//first triangle
            3,1,0,//second triangle
 
            //back face//
            4,6,7,//first triangle
            7,5,4,//second triangle
 
            //left face//
            8,10,11,//first triangle
            11,9,8,//second triangle
 
            //right face//
            12,14,15,//first triangle
            15,13,12,//second triangle
 
            //top face//
            16,18,19,//first triangle
            19,17,16,//second triangle
 
            //bottom face//
            20,22,23,//first triangle
            23,21,20//second triangle
        };
 
        //UVs//
        Vector2[] uvs = new Vector2[]
        {
            //front face// 0,0 is bottom left, 1,1 is top right//
            new Vector2(0,1),
            new Vector2(0,0),
            new Vector2(1,1),
            new Vector2(1,0),
 
            new Vector2(0,1),
            new Vector2(0,0),
            new Vector2(1,1),
            new Vector2(1,0),
 
            new Vector2(0,1),
            new Vector2(0,0),
            new Vector2(1,1),
            new Vector2(1,0),
 
            new Vector2(0,1),
            new Vector2(0,0),
            new Vector2(1,1),
            new Vector2(1,0),
 
            new Vector2(0,1),
            new Vector2(0,0),
            new Vector2(1,1),
            new Vector2(1,0),
 
            new Vector2(0,1),
            new Vector2(0,0),
            new Vector2(1,1),
            new Vector2(1,0)
        };

        Renderer renderer = GetComponent<Renderer>();
		this.aMesh.Clear();
        this.aMesh.vertices = vertices;
        this.aMesh.triangles = triangles;
        this.aMesh.uv = uvs;
		this.aMesh.RecalculateNormals();
        Material mat = Resources.Load("Wall") as Material;
        renderer.material = mat;
	}

    public Mesh getMesh()
    {
        return this.aMesh;
    }
}