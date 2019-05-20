using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

using Crosstales.FB;

public class Definition : MonoBehaviour
{
    public enum pointEnum {UNDEFINED = -1, EMPTY = 0, WALL = 1, FOG = 2, VISITED = 3};

    public enum menuEnum {TRAIN_NN = 1, LOAD_NN = 2, TRAIN_GN = 3, LOAD_GN = 4};

    public enum weightsEnum {DIST = 0, NB_EMPTY = 1, NB_WALL = 2, NB_FOG = 3, NB_VISITED = 4, AVG_DIST_EMPTY = 5, AVG_DIST_WALL = 6,
                            AVG_DIST_FOG = 7, AVG_DIST_VISITED = 8, DIST_FROM_ENTITY = 9};

	public const double M_PI = 3.14159265358979323846;

    public const double INFINITY = double.PositiveInfinity;

    public static string OpenSingleFile()
    {
        string path = FileBrowser.OpenSingleFile("gn");
        return path;
    }
}
