/**
 * \file neuralNetwork.h
 * \brief Neural network functions
 * \author Louis Schuck
 * \version 0.1
 * \date 18/02/2019
 *
 * Module that contain all the functions to do a neural network and its learning
 *
 */

//Header files containing the prototypes
#include "neuralNetwork.h"

/**
 * \fn NeuralNetwork* createNeuralNetwork(int nbLayer, int* neuronsPerLayers, float minWeight, float maxWeight)
 * \brief Initialize a NeuralNetwork with random weigths
 *
 * \param nbLayer : the number of layer in your neural network. Must be at least 2
 * \param neuronsPerLayers : a list of integer which indicate the number of neurones on each layers
 *          the first layer indicate the number of inputs the neural network takes
 * \param minWeight : the minimum value for each weight of each neurone
 * \param mawWeight : the maximum value for each weight of each neurone
 * 
 * \return
 *      NeuralNetwork*
 */
NeuralNetwork* createNeuralNetwork(int nbLayer, int* neuronsPerLayers, float minWeight, float maxWeight)
{
	// We allocate the memory to store the structure Layer
	NeuralNetwork* neuralNetwork = malloc(sizeof(NeuralNetwork));
	// We set the number of layers
	neuralNetwork->nbLayer = nbLayer;
	// We allocate all the memory to store all the structure Layer
	neuralNetwork->layers = malloc(sizeof(Layer*)*nbLayer);
	// We initialize the input layer
	neuralNetwork->layers[0] = createLayer(neuronsPerLayers[0], 0, minWeight, maxWeight);
	// For each layer that is not the input layer
	int indexLayer;
	for(indexLayer = 1; indexLayer < nbLayer; indexLayer++)
	{
		// We initialize each layers
		neuralNetwork->layers[indexLayer] = 
			createLayer(neuronsPerLayers[indexLayer], neuronsPerLayers[indexLayer-1], minWeight, maxWeight);
	}
	// then, we return the structure
	return neuralNetwork;
}

/**
 * \fn Layer* createLayer(int nbNeurone, int nbNeuroneOfPreviousLayer, float minWeight, float maxWeight)
 * \brief Initialize a Layer
 *
 * \param nbNeurone : the number of neurone on the layer
 * \param nbNeuroneOfPreviousLayer : the number of neurone on the previous layer
 * \param minWeight : the minimum value for each weight of each neurone
 * \param mawWeight : the maximum value for each weight of each neurone
 * \return
 *		Layer*
 */
Layer* createLayer(int nbNeurone, int nbNeuroneOfPreviousLayer, float minWeight, float maxWeight)
{
	// We allocate the memory to store the structure Layer
	Layer* layer = malloc(sizeof(Layer));
	// We set the number of neurons
	layer->nbNeurone = nbNeurone;
	// We allocate all the memory to store all the structure Neurone
	layer->neurons = malloc(sizeof(Neurone*)*nbNeurone);
	// For each neurons
	int indexNeurone;
	for(indexNeurone = 0; indexNeurone < nbNeurone; indexNeurone++)
	{
		// We initialize each neurons
		layer->neurons[indexNeurone] = createNeurone(nbNeuroneOfPreviousLayer, minWeight, maxWeight);
	}
	// Then, we return the structure
	return layer;
}

/**
 * \fn Neurone* createNeurone(int nbInput, float minWeight, float maxWeight)
 * \brief Initialize a Neurone
 *
 * \param nbInput : the number of input the neurone wil take (the number of neurone on the previous layer)
 * \param minWeight : the minimum value for each weight of each neurone
 * \param mawWeight : the maximum value for each weight of each neurone
 * \return
 *		Neurone*
 */
Neurone* createNeurone(int nbInput, float minWeight, float maxWeight)
{
	// We allocate the memory to store the structure Neurone
	Neurone* neurone = malloc(sizeof(Neurone));
	// We set the number of input
	neurone->nbInput = nbInput;
	// We set a random bias, between minWeight and maxWeight
	neurone->bias = nmap(rand()%1000, 0, 1000, minWeight, maxWeight);
	// We allocate the memory to store all the input weights
	neurone->inputWeights = malloc(sizeof(float)*nbInput);
	// For each weights
	int indexInputWeights;
	for(indexInputWeights = 0; indexInputWeights < nbInput; indexInputWeights++)
	{
		// We give it a random value between minWeight and maxWeight
		neurone->inputWeights[indexInputWeights] = nmap(rand()%1000, 0, 1000, minWeight, maxWeight);
	}
	// Then, we return the structure
	return neurone;
}

/**
 * \fn void destructNeuralNetwork(NeuralNetwork** neuralNetwork)
 * \brief free a NeuralNetwork from the memory
 *
 * \param
 *		neuralNetwork : the NeuralNetwork to be free
 * \return
 *      void
 */
void destructNeuralNetwork(NeuralNetwork** neuralNetwork)
{
	// If the argument is not null
	if (neuralNetwork != NULL)
	{
		// If what he point to is not null either
		if (*neuralNetwork != NULL)
		{
			// We destruct each Layer
			int indexLayer;
			for(indexLayer = 0; indexLayer < (*neuralNetwork)->nbLayer; indexLayer++)
			{
				destructLayer(&((*neuralNetwork)->layers[indexLayer]));
			}
			// We free the list of Layer from the memory
			free((*neuralNetwork)->layers);
			// We free the NeuralNetwork from the memory
			free(*neuralNetwork);
			// We change the pointer to NULL to be sure their is no problem outside of the function
			*neuralNetwork = NULL;
		}
	}
}

/**
 * \fn void destructLayer(Layer** layer)
 * \brief free a Layer from the memory
 *
 * \param
 *		layer : the Layer to be free
 * \return
 *      void
 */
void destructLayer(Layer** layer)
{
	// If the argument is not null
	if (layer != NULL)
	{
		// If what he point to is not null either
		if (*layer != NULL)
		{
			// We destruct each Neurone
			int indexNeurone;
			for(indexNeurone = 0; indexNeurone < (*layer)->nbNeurone; indexNeurone++)
			{
				destructNeurone(&((*layer)->neurons[indexNeurone]));
			}
			// We free the list of Neurone from the memory
			free((*layer)->neurons);
			// We free the Layer from the memory
			free(*layer);
			// We change the pointer to NULL to be sure their is no problem outside of the function
			*layer = NULL;
		}
	}
}

/**
 * \fn void destructNeurone(Neurone** neurone)
 * \brief free a Neurone from the memory
 *
 * \param
 *		neurone : the Neurone to be free
 * \return
 *      void
 */
void destructNeurone(Neurone** neurone)
{
	// If the argument is not null
	if (neurone != NULL)
	{
		// If what he point to is not null either
		if (*neurone != NULL)
		{
			// We free the list of weights
			free((*neurone)->inputWeights);
			// We free the Neurone from the memory
			free(*neurone);
			// We change the pointer to NULL to be sure their is no problem outside of the function
			*neurone = NULL;
		}
	}
}

/**
 * \fn void printNeuralNetwork(NeuralNetwork* neuralNetwork)
 * \brief visualize a neural network in the terminal
 *
 * \param
 *		neuralNetwork : the NeuralNetwork to be shown
 * \return
 *      void
 */
void printNeuralNetwork(NeuralNetwork* neuralNetwork)
{
	printf("The neural network has %d layer\n", neuralNetwork->nbLayer);
	int indexLayer, indexNeurone, indexWeight;
	for(indexLayer = 0; indexLayer < neuralNetwork->nbLayer; indexLayer++)
	{
		printf("\tThe layer %d has %d neurones\n", 
			indexLayer, 
			neuralNetwork->layers[indexLayer]->nbNeurone);
		for(indexNeurone = 0; indexNeurone < neuralNetwork->layers[indexLayer]->nbNeurone; indexNeurone++)
		{
			printf("\t\tWeights of the neurone %d\n", indexNeurone);
			for(indexWeight = 0; indexWeight < neuralNetwork->layers[indexLayer]->neurons[indexNeurone]->nbInput; indexWeight++)
			{
				printf("\t\t\tWeights of input %d : %f\n", 
					indexWeight, 
					neuralNetwork->layers[indexLayer]->neurons[indexNeurone]->inputWeights[indexWeight]);
			}
			printf("\t\t\tBias : %f\n", neuralNetwork->layers[indexLayer]->neurons[indexNeurone]->bias);
		}
	}
}

/**
 * \fn float* getOutputOfNeuralNetwork(NeuralNetwork* neuralNetwork, float* inputs)
 * \brief get the output of a neural network. Their is as much output as neurons on the last layer of the neural network
 *
 * \param neuralNetwork : the neural network that will compute the inputs into outputs
 * \param inputs : a list containing the inputs we will put into the neural network
 *		    their is as many input as the number of neurons in the first layer
 * \return
 *		float* 
 */
float* getOutputOfNeuralNetwork(NeuralNetwork* neuralNetwork, float* inputs)
{
	float* tempOutputs = NULL; // Use to store temporarly the outputs of a layer
	// The first layer is just the inputs layer, so his outputs are the inputs
	// We basicly just skip it
	float* outputs = cpyList(inputs, neuralNetwork->layers[0]->nbNeurone);
	// For each other layers in the neural network
	int indexLayer;
	for(indexLayer = 1; indexLayer < neuralNetwork->nbLayer; indexLayer++)
	{
		// We calculate his outputs from the outputs of the previous layer
		tempOutputs = getOutputOfLayer(
			neuralNetwork->layers[indexLayer], 
			outputs);
		// We free the outputs of the previous layer from the memory
		free(outputs);
		// We set the outputs of the layer we just calculated as the new outputs
		outputs = tempOutputs;
	}
	// Then, we return the outputs of the last layer
	return outputs;
}

/**
 * \fn float* getOutputOfLayer(Layer* layer, float* inputs)
 * \brief get the output of a Layer. Their is as much output as neurons on the layer
 *
 * \param layer : the layer that will compute the inputs into outputs
 * \param inputs : a list containing the inputs we will put into each neurons of the layer
 * \return
 *		float*
 */
float* getOutputOfLayer(Layer* layer, float* inputs)
{
	// We initialize the list that will contain all the outputs
	float* outputs = malloc(sizeof(float)*layer->nbNeurone);
	// For each neurons in the layer
	int indexNeurone;
	for(indexNeurone = 0; indexNeurone < layer->nbNeurone; indexNeurone++)
	{
		// We calculate his output
		outputs[indexNeurone] = getOutputOfNeurone(layer->neurons[indexNeurone], inputs);
	}
	// Then, we return the outputs
	return outputs;
}

/**
 * \fn float getOutputOfNeurone(Neurone* neurone, float* inputs)
 * \brief get the output of neurone
 *
 * \param neurone : the neurone that will compute the inputs into a single output
 * \param inputs : a list containing the inputs we will put into the neurone
 *					their is as many input as indicated in the neurone (nbInput)
 * \return
 *		float
 */
float getOutputOfNeurone(Neurone* neurone, float* inputs)
{
	float output = 0; // Will be use to store the output we are calculating
	// For each input of the neurone
	int indexInput;
	for(indexInput = 0; indexInput < neurone->nbInput; indexInput++)
	{
		// We calculate the weighted input and we sum it to the output
		output += inputs[indexInput] * neurone->inputWeights[indexInput];
	}
	// Then we add the bias to the output
	output += neurone->bias;
	// Then, we return the output of the transfer function
	return neuroneTransferFunction(output);
}

/**
 * \fn float neuroneTransferFunction(float x)
 * \brief use after each neurones to transform the output. It is the transfer function of each neurons
 *
 * \param
 *		x : the input given to the function
 * \return
 *		float
 */
float neuroneTransferFunction(float x)
{
	// we return the output of the sigmoid function
	return 1/(1 + exp(-x));
}

/**
 * \fn float neuroneDerivateTransferFunction(float x)
 * \brief work the same as neuroneTransferFunction but it is the derivative of the transfer function
 *
 * \param
 *		x : the input given to the function. it must be an ouput of the transfer function
 * \return
 *		float
 */
float neuroneDerivateTransferFunction(float x)
{
	// We return the derivative of the sigmoid function with x the output of a sigmoid function
	return x * (1 - x);
}

/**
 * \fn bool superviseLearningNeuralNetwork(NeuralNetwork* neuralNetwork, float* inputs, float* referenceOutputs, float learningSpeed, float errorMargin)
 * \brief check if the neural network return the reference output if you give it specific inputs.
 *		if it doesn't, correct the weights of each neurons with the retropropagation technique
 *      true if the outputs of the neural network from the given inputs is close enought to the referenceOutputs
 *		false if not, and if correction has been done
 *
 * \param neuralNetwork : the neural network we will check and correct
 * \param inputs : the inputs we will give to the neural network
 * \param referenceOutputs : the output the neural network is suppose to give from the given inputs
 * \param learningSpeed : the amount of wich we will modify each weight when we correct them.
 *			the higher it is, the faster the neural network learn but the less it is precise
 * \param errorMargin : the error the neural network is allowd to do.
 *		    it is basicly the maximum difference between the outputs of the neural network and
 *		    the referenceOutputs
 * \return
 *		bool
 */
bool superviseLearningNeuralNetwork(NeuralNetwork* neuralNetwork, float* inputs, float* referenceOutputs, float learningSpeed, float errorMargin)
{
	//////////////////////////////////////////////////
	//--- outputs of each neurons of each layers ---//
	//////////////////////////////////////////////////
	
	// Temporary variable wich will be use to know how many outputs their is to the neural network
	int nbOutput = neuralNetwork->layers[neuralNetwork->nbLayer-1]->nbNeurone;
	// We allocate the memory to store all the outputs of each layers
	float** outputsOfEachLayer = malloc(sizeof(float*)*neuralNetwork->nbLayer);
	// We store a copy of the inputs as the first layer outputs
	outputsOfEachLayer[0] = cpyList(inputs, neuralNetwork->layers[0]->nbNeurone);
	// For each other layers in the neural network
	int indexLayer;
	for(indexLayer = 1; indexLayer < neuralNetwork->nbLayer; indexLayer++)
	{
		// We calculate his outputs from the outputs of the previous layer and we store it
		outputsOfEachLayer[indexLayer] = getOutputOfLayer(
			neuralNetwork->layers[indexLayer], 
			outputsOfEachLayer[indexLayer-1]);
	}

	////////////////////////
	//--- global error ---//
	////////////////////////
	
	float globalError[nbOutput]; // Use to store the global error
	// We calculate the global error for each output
	int indexOutput;
	for(indexOutput = 0; indexOutput < nbOutput; indexOutput++)
	{
		// The global error of any input is
		globalError[indexOutput] = 
			referenceOutputs[indexOutput] - 							// The reference output, minus
			outputsOfEachLayer[neuralNetwork->nbLayer-1][indexOutput];	// The output of the last layer
	}
	
	bool isTrainingNeeded = false; // Use to know if a training is needed or not
	// For each global error
	for(indexOutput = 0; indexOutput < nbOutput; indexOutput++)
	{
		// If it is inferior to the error margin
		if (globalError[indexOutput] < -errorMargin ||
			globalError[indexOutput] > errorMargin)
		{
			// Then, we don't need to train
			isTrainingNeeded = true;
		}
	}
	
	// If the neural network needs to be trained
	if (isTrainingNeeded)
	{
		///////////////////////
		//--- local error ---//
		///////////////////////
		//--- layer before the output layer
		
		// We allocate the memory to store all the local errors of each layers
		float** localErrors = malloc(sizeof(float*)*neuralNetwork->nbLayer);
		// We calculate the local error of the output layer
		// First, we get how many output their is on this layer
		int currentNbOutput = neuralNetwork->layers[neuralNetwork->nbLayer-1]->nbNeurone;
		// Second, we allocate the memory to store all the local errors of this layer
		float* currentLocalError = malloc(sizeof(float)*currentNbOutput);
		// Third, for each output of this layer
		for(indexOutput = 0; indexOutput < currentNbOutput; indexOutput++)
		{
			// We calculate his local error
			currentLocalError[indexOutput] = 
				neuroneDerivateTransferFunction(outputsOfEachLayer[neuralNetwork->nbLayer-1][indexOutput]) * 
				globalError[indexOutput];
		}
		// Finaly, we store it in the matrice that contain all local errors
		localErrors[neuralNetwork->nbLayer-1] = currentLocalError;
		
		//--- other layers
		
		// Then, we calculate the local error of each remaining layers (all but the two last one)
		float currentErrorSum; // use to store the weighted sum of local errors of the previously calculated layer
		int indexNeurone; // use to get the weights of a neurone to calculate the weighted sum of local errors
		for(indexLayer = neuralNetwork->nbLayer-2; indexLayer >= 0; indexLayer--)
		{
			// We reset the weighted sum of local errors
			currentErrorSum = 0;
			// First, we get how many output their is on this layer
			currentNbOutput = neuralNetwork->layers[indexLayer]->nbNeurone;
			// Second, we allocate the memory to store all the local errors of this layer
			currentLocalError = malloc(sizeof(float)*currentNbOutput);
			// Third, for each output of this layer
			for(indexOutput = 0; indexOutput < currentNbOutput; indexOutput++)
			{
				// And for each neurons of the previously calculated layer (for each link, so for each weights)
				for(indexNeurone = 0; indexNeurone < neuralNetwork->layers[indexLayer+1]->nbNeurone; indexNeurone++)
				{
					// We update the weighted sum
					currentErrorSum += 
						// The local error of the previously calculated layer for each neurons
						localErrors[indexLayer+1][indexNeurone] * 
						// The weight that link the neurone which we are currently calculating the local error to the previously calculated layer for each neurons
						neuralNetwork->layers[indexLayer+1]->neurons[indexNeurone]->inputWeights[indexOutput];
				}
				// We calculate his local error
				currentLocalError[indexOutput] = 
					neuroneDerivateTransferFunction(outputsOfEachLayer[indexLayer][indexOutput]) * 
					currentErrorSum;
			}
			// Finaly, we store it in the matrice that contain all local errors
			localErrors[indexLayer] = currentLocalError;
		}
		
		//////////////////////////////////
		//--- correcting the weights ---//
		//////////////////////////////////
		
		// For each layers which are not the output layer
		for(indexLayer = 1; indexLayer < neuralNetwork->nbLayer; indexLayer++)
		{
			// For each neurons on this layer
			for(indexNeurone = 0; indexNeurone < neuralNetwork->layers[indexLayer]->nbNeurone; indexNeurone++)
			{
				// We get how many output their is on the previous layer
				currentNbOutput = neuralNetwork->layers[indexLayer-1]->nbNeurone;
				// For each output of the previous layer
				for(indexOutput = 0; indexOutput < currentNbOutput; indexOutput++)
				{
					// We change it according to the local error
					neuralNetwork->layers[indexLayer]->neurons[indexNeurone]->inputWeights[indexOutput] += 
						learningSpeed * 
						localErrors[indexLayer][indexNeurone] * 
						outputsOfEachLayer[indexLayer-1][indexOutput];
				}
				// Then, we change the bias according to the local error
				neuralNetwork->layers[indexLayer]->neurons[indexNeurone]->bias += 
					learningSpeed * 
					localErrors[indexLayer][indexNeurone];
			}
		}
		
		// We free the outputs and the local errors we previously saved in matrices
		destructMatrice2D((void**) outputsOfEachLayer, neuralNetwork->nbLayer); 
		destructMatrice2D((void**) localErrors, neuralNetwork->nbLayer);
	}
	// We return false if we did a training, true otherwise
	return !isTrainingNeeded;
}

/**
 * \fn bool saveNeuralNetwork(NeuralNetwork* neuralNetwork, const char* path)
 * \brief save a neural network in a file. return true if the file has correctly been saved, false otherwise
 *
 * \param neuralNetwork : the neural network we will save
 * \param path : the path to the file where we'll save the neural network
 * \return
 *		bool
 */
bool saveNeuralNetwork(NeuralNetwork* neuralNetwork, const char* path)
{
	FILE* file; // use to store the file where the game will be save
	// If we managed to open the file
	if((file = fopen(path, "wb+")))
	{
		//////////////////////////////////////
		//--- size of the neural network ---//
		//////////////////////////////////////
		
		// We write down the number of layer
		fwrite(&(neuralNetwork->nbLayer), sizeof(int), 1, file);
		// Then, for each layer
		int indexLayer;
		for(indexLayer = 0; indexLayer < neuralNetwork->nbLayer; indexLayer++)
		{
			// We write down the number of neurone per layer
			fwrite(&(neuralNetwork->layers[indexLayer]->nbNeurone), sizeof(int), 1, file);
		}
		
		/////////////////////////////////////////
		//--- weights of the neural network ---//
		/////////////////////////////////////////
		
		// For each layer
		int indexNeurone;
		for(indexLayer = 0; indexLayer < neuralNetwork->nbLayer; indexLayer++)
		{
			// Then, for each neurone
			for(indexNeurone = 0; indexNeurone < neuralNetwork->layers[indexLayer]->nbNeurone; indexNeurone++)
			{
				// We write down the bias
				fwrite(&(neuralNetwork->layers[indexLayer]->neurons[indexNeurone]->bias), 
					sizeof(float), 
					1, 
					file);
				// We write down the weights
				fwrite(neuralNetwork->layers[indexLayer]->neurons[indexNeurone]->inputWeights, 
					sizeof(float), 
					neuralNetwork->layers[indexLayer]->neurons[indexNeurone]->nbInput, 
					file);
			}
		}
		
		// We close the file
		fclose(file);
		// We return true to indicate that everything worked correctly
		return true;
	}
	else
	{
		// We return false because the file has not been read correctly
		return false;
	}
}

/**
 * \fn NeuralNetwork* loadNeuralNetwork(const char* path)
 * \brief load a neural network from a file. If the function can't read it, return NULL
 *
 * \param
 *		path : the path to the file where we'll load the neural network
 * \return
 *		NeuralNetwork*
 */
NeuralNetwork* loadNeuralNetwork(const char* path)
{
	FILE* file;
	if((file = fopen(path, "rb")))
	{
		int statut = 0;
	
		//////////////////////////////////////
		//--- size of the neural network ---//
		//////////////////////////////////////
		
		// We read the number of layer
		int nbLayer;
		statut = fread(&nbLayer, sizeof(int), 1, file);
		if (statut == 0) 
		{
			return NULL;
		}
		
		// We initialize a list to store the information about the number of neurone per layer
		int neuronsPerLayers[nbLayer];
		// Then, for each layer
		int indexLayer;
		for(indexLayer = 0; indexLayer < nbLayer; indexLayer++)
		{
			// We read the number of neurone per layer
			statut = fread(&(neuronsPerLayers[indexLayer]), sizeof(int), 1, file);
			if (statut == 0) 
			{
				return NULL;
			}
		}
		// We create a neural network with the correct number of layer and neurone
		NeuralNetwork* neuralNetwork = createNeuralNetwork(nbLayer, neuronsPerLayers, -0.5, 0.5);
		
		/////////////////////////////////////////
		//--- weights of the neural network ---//
		/////////////////////////////////////////
		
		// For each layer
		int indexNeurone;
		for(indexLayer = 0; indexLayer < nbLayer; indexLayer++)
		{
			// Then, for each neurone
			for(indexNeurone = 0; indexNeurone < neuronsPerLayers[indexLayer]; indexNeurone++)
			{
				// We read the bias
				statut = fread(&(neuralNetwork->layers[indexLayer]->neurons[indexNeurone]->bias), 
					sizeof(float), 
					1, 
					file);

				// We read the weights
				statut = fread(neuralNetwork->layers[indexLayer]->neurons[indexNeurone]->inputWeights, 
					sizeof(float), 
					neuralNetwork->layers[indexLayer]->neurons[indexNeurone]->nbInput, 
					file);

			}
		}
		
		// We close the file
		fclose(file);
		// We return the neural network
		return neuralNetwork;
	}
	else
	{
		return NULL;
	}
}
