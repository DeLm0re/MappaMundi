/**
 * \file neuralNetwork.h
 * \brief Prototypes of the neural network functions
 * \author Louis Schuck
 * \version 0.1
 * \date 18/02/2019
 *
 * Module that contain all the prototypes of functions in neuralNetwork.c
 *
 */

#ifndef H_NEURALNETWORK
	#define H_NEURALNETWORK

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "rioFunction.h"

// Structure Neurone
typedef struct Neurone
{
	float* inputWeights; 	// The weights of the neurone for each input
	float bias; 			// The bias of the neurone
	int nbInput; 			// The number of input the neurone wil receive
}Neurone;

// Structure Layer
typedef struct Layer
{
	Neurone** neurons;		// The neurons in the layer
	int nbNeurone;			// The number of neurons in the Layer
}Layer;

// Structure NeuralNetwork
typedef struct NeuralNetwork
{
	Layer** layers; 		// The layers in the NeuralNetworks
	int nbLayer;			// The number of layers in the NeuralNetwork
}NeuralNetwork;

/**
 * \fn NeuralNetwork* createNeuralNetwork(int nbLayer, int* neuronsPerLayers, float minWeight, float maxWeight)
 * \brief Initialize a NeuralNetwork with random weigths
 *
 * \param nbLayer : the number of layer in your neural network. Must be at least 2
 * \param neuronsPerLayers : a list of integer which indicate the number of neurones on each layers
 * the first layer indicate the number of inputs the neural network takes
 * \param minWeight : the minimum value for each weight of each neurone
 * \param mawWeight : the maximum value for each weight of each neurone
 * 
 * \return NeuralNetwork* : The initialized NeuralNetwork
 */
NeuralNetwork* createNeuralNetwork(int nbLayer, int* neuronsPerLayers, float minWeight, float maxWeight);

/**
 * \fn Layer* createLayer(int nbNeurone, int nbNeuroneOfPreviousLayer, float minWeight, float maxWeight)
 * \brief Initializes a Layer of a neural network
 *
 * \param nbNeurone : the number of neurone on the layer
 * \param nbNeuroneOfPreviousLayer : the number of neurone on the previous layer
 * \param minWeight : the minimum value for each weight of each neurone
 * \param mawWeight : the maximum value for each weight of each neurone
 * 
 * \return Layer* : The created layer
 */
Layer* createLayer(int nbNeurone, int nbNeuroneOfPreviousLayer, float minWeight, float maxWeight);

/**
 * \fn Neurone* createNeurone(int nbInput, float minWeight, float maxWeight)
 * \brief Initialize a Neurone
 *
 * \param nbInput : the number of input the neurone wil take (the number of neurone on the previous layer)
 * \param minWeight : the minimum value for each weight of each neurone
 * \param mawWeight : the maximum value for each weight of each neurone
 * 
 * \return Neurone* : The created neurone
 */
Neurone* createNeurone(int nbInput, float minWeight, float maxWeight);

/**
 * \fn void destructNeuralNetwork(NeuralNetwork** neuralNetwork)
 * \brief Frees a NeuralNetwork from the memory
 *
 * \param neuralNetwork : the NeuralNetwork to be free
 * 
 * \return void
 */
void destructNeuralNetwork(NeuralNetwork** neuralNetwork);

/**
 * \fn void destructLayer(Layer** layer)
 * \brief Frees a Layer from the memory
 *
 * \param layer : the Layer to be free
 * 
 * \return void
 */
void destructLayer(Layer** layer);

/**
 * \fn void destructNeurone(Neurone** neurone)
 * \brief Frees a Neurone from the memory
 *
 * \param neurone : address of the Neurone to free
 * 
 * \return void
 */
void destructNeurone(Neurone** neurone);

/**
 * \fn void printNeuralNetwork(NeuralNetwork* neuralNetwork)
 * \brief Display a neural network in the terminal
 *
 * \param neuralNetwork : the NeuralNetwork to be shown
 * 
 * \return void
 */
void printNeuralNetwork(NeuralNetwork* neuralNetwork);

/**
 * \fn float* getOutputOfNeuralNetwork(NeuralNetwork* neuralNetwork, float* inputs)
 * \brief Get the output of a neural network. Their is as much outputs as neurons of the last layer of the neural network
 *
 * \param neuralNetwork : the neural network that will compute the inputs into outputs
 * \param inputs : a list containing the inputs we will put into the neural network
 * their is as many input as the number of neurons in the first layer
 * 
 * \return float* : Output of the neural network
 */
float* getOutputOfNeuralNetwork(NeuralNetwork* neuralNetwork, float* inputs);

/**
 * \fn float* getOutputOfLayer(Layer* layer, float* inputs)
 * \brief Get the output of a Layer. Their is as much output as neurons on the layer
 *
 * \param layer : the layer that will compute the inputs into outputs
 * \param inputs : a list containing the inputs we will put into each neurons of the layer
 * 
 * \return float* : Output of the layer
 */
float* getOutputOfLayer(Layer* layer, float* inputs);

/**
 * \fn float getOutputOfNeurone(Neurone* neurone, float* inputs)
 * \brief Get the output of a neurone
 *
 * \param neurone : the neurone that will compute the inputs into a single output
 * \param inputs : a list containing the inputs we will put into the neurone
 * there is as many input as indicated in the neurone (nbInput)
 * 
 * \return float : Output of the neurone
 */
float getOutputOfNeurone(Neurone* neurone, float* inputs);

/**
 * \fn float neuroneTransferFunction(float x)
 * \brief Use after each neurones to transform the output. It is the transfer function of each neurons
 *
 * \param x : the input given to the function
 * 
 * \return float : Output of the transfer function
 */
float neuroneTransferFunction(float x);

/**
 * \fn float neuroneDerivateTransferFunction(float x)
 * \brief Work the same as neuroneTransferFunction but it is the derivative of the transfer function
 *
 * \param x : the input given to the function. it must be an ouput of the transfer function
 * 
 * \return float : Output of the derivate tranfer function
 */
float neuroneDerivateTransferFunction(float x);

/**
 * \fn bool superviseLearningNeuralNetwork(NeuralNetwork* neuralNetwork, float* inputs, float* referenceOutputs, float learningSpeed, float errorMargin)
 * \brief Check if the neural network return the reference output if you give it specific inputs.
 * if it doesn't, correct the weights of each neurons with the retropropagation technique
 *
 * \param neuralNetwork : the neural network we will check and correct
 * \param inputs : the inputs we will give to the neural network
 * \param referenceOutputs : the output the neural network is suppose to give from the given inputs
 * \param learningSpeed : the amount of wich we will modify each weight when we correct them.
 * the higher it is, the faster the neural network learn but the less it is precise
 * \param errorMargin : the error the neural network is allowd to do.
 * it is basicly the maximum difference between the outputs of the neural network and
 * the referenceOutputs

 * \return bool : True if the outputs of the neural network from the given inputs is close enought to the referenceOutputs
 * false if not, and if correction has been done
 */
bool superviseLearningNeuralNetwork(NeuralNetwork* neuralNetwork, float* inputs, float* referenceOutputs, float learningSpeed, float errorMargin);

/**
 * \fn bool saveNeuralNetwork(NeuralNetwork* neuralNetwork, const char* path)
 * \brief Save a neural network in a file
 *
 * \param neuralNetwork : the neural network we will save
 * \param path : the path to the file where we'll save the neural network
 * 
 * \return bool : Returns true if the file has correctly been saved, false otherwise
 */
bool saveNeuralNetwork(NeuralNetwork* neuralNetwork, const char* path);

/**
 * \fn NeuralNetwork* loadNeuralNetwork(const char* path)
 * \brief Load a neural network from a file. If the function can't read it, return NULL
 *
 * \param path : the path to the file where we'll load the neural network
 * 
 * \return NeuralNetwork* : The loaded network
 */
NeuralNetwork* loadNeuralNetwork(const char* path);

#endif