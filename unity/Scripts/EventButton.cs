using System;
using UnityEngine;
using UnityEngine.UI;
using System.IO;

using Crosstales.FB;

public class EventButton : MonoBehaviour
{
    //Make sure to attach these Buttons in the Inspector
    public Button quitButton;
    public Button pauseButton;
    public Button genomeButton;
    public Button aleatoireButton;
    public Button customButton;
    
    public Slider sliderFps;

    public Text fpsValue;

    public EnvironmentHandler environment;

    public Pathfinding path;

    public EntityHandler entity;

    public GeneticAlgorithm genetic;

    void Start()
    {
        updateFps();
        //Calls the TaskOnClick/TaskWithParameters/ButtonClicked method when you click the Button
        quitButton.onClick.AddListener(ApplicationQuit);
        pauseButton.onClick.AddListener(ChangePauseMode);
        genomeButton.onClick.AddListener(SwapGenome);
        sliderFps.onValueChanged.AddListener(delegate {updateFps(); });
        aleatoireButton.onClick.AddListener(AleatoireMap);
        customButton.onClick.AddListener(CustomMap);
    }

    private void AleatoireMap()
    {
        environment.setFilePath("none");
        entity.setFlagMap("random");
        this.GenerationMap();
    }

    private void CustomMap()
    {
        environment.setFilePath("none");
        entity.setFlagMap("custom");
        this.GenerationMap();
    }
    
    private void updateFps()
    {
        entity.setWaitSecondToMove(1/sliderFps.value);
        displayNewFps();
    }

    private void displayNewFps()
    {
        fpsValue.text = sliderFps.value.ToString();
    }

    private void ChangePauseMode()
    {
        entity.setPause(!entity.getPause());
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
        entity.newGeneration();

        //Actualize the position of the entity
        entity.actualizePositionEntity();

        //Update the fieldOfView for the start
        entity.updateFieldOfViewEntity();

        //Update the fieldOfView for the start
        entity.updateMentalMapEntity();

        //Destroy the mesh of the last frame
        entity.fogHandler.getFogMeshHandler().DestructMesh();
        //Create the starting fog of the new map
        entity.fogHandler.CreateFog(entity.getMentalMap());
    }

    private void SwapGenome()
    {
        string filePath = Definition.OpenSingleFile();

        if(System.IO.File.Exists(filePath))
        {
            int i;
            var fileContent = File.ReadAllBytes(filePath);

            for(i = 0; i < 10; i++)
            {
                genetic.setLabelingWeights(i, BitConverter.ToDouble(fileContent,i*8));
            }
        }
        else
        {
            Debug.Log("Pas de fichier");
        }

        this.GenerationMap();
    }

}