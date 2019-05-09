using UnityEngine;
using UnityEngine.UI;

public class EventButton : MonoBehaviour
{
    //Make sure to attach these Buttons in the Inspector
    public Button quitButton;
    public Button genButton;

    public EnvironmentHandler environment;

    public Pathfinding path;

    public EntityHandler entity;

    void Start()
    {
        //Calls the TaskOnClick/TaskWithParameters/ButtonClicked method when you click the Button
        quitButton.onClick.AddListener(ApplicationQuit);
        genButton.onClick.AddListener(GenerationMap);
    }

    private void ApplicationQuit()
    {
        // save any game data here
        #if UNITY_EDITOR
            // Application.Quit() does not work in the editor so
            // UnityEditor.EditorApplication.isPlaying need to be set to false to end the game
            UnityEditor.EditorApplication.isPlaying = false;
        #else
            Application.Quit();
        #endif
    }

    private void GenerationMap()
    {
        environment.getEnvironmentMeshHandler().DestructMesh();
        environment.CreateMap();

        int[,] map = environment.getMap();
        int width = map.GetLength(0);
        int height = map.GetLength(1);

        path.cleanPath();
        path.findPathFromStartEnd(path.nearestNode(map,0,0), path.nearestNode(map,width-1,height-1), map);

        entity.entityInitialization();
    }
}