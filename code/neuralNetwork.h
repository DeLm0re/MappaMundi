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

/* @function
 *		Initialize a NeuralNetwork with random weigths
 *
 * @param
 *		nbLayer : 			the number of layer in your neural network. Must be at least 2
 *		neuronsPerLayers :	a list of integer which indicate the number of neurones on each layers
 *							the first layer indicate the number of inputs the neural network takes
 *		minWeight : 		the minimum value for each weight of each neurone
 *		mawWeight : 		the maximum value for each weight of each neurone
 *
 * @return
 *		a NeuralNetwork pointer
 */
NeuralNetwork* createNeuralNetwork(int nbLayer, int* neuronsPerLayers, float minWeight, float maxWeight);

/* @function
 *		Initialize a Layer
 *
 * @param
 *		nbNeurone : 				the number of neurone on the layer
 *		nbNeuroneOfPreviousLayer :	the number of neurone on the previous layer
 *		minWeight : 				the minimum value for each weight of each neurone
 *		mawWeight : 				the maximum value for each weight of each neurone
 *
 * @return
 *		a Layer pointer
 */
Layer* createLayer(int nbNeurone, int nbNeuroneOfPreviousLayer, float minWeight, float maxWeight);

/* @function
 *		Initialize a Neurone
 *
 * @param
 *		nbInput : 	the number of input the neurone wil take (the number of neurone on the previous layer)
 *		minWeight : the minimum value for each weight of each neurone
 *		mawWeight : the maximum value for each weight of each neurone
 *
 * @return
 *		a Neurone pointer
 */
Neurone* createNeurone(int nbInput, float minWeight, float maxWeight);

/* @function
 *		free a NeuralNetwork from the memory
 *
 * @param
 *		neuralNetwork : 	the NeuralNetwork to be free
 */
void destructNeuralNetwork(NeuralNetwork** neuralNetwork);

/* @function
 *		free a Layer from the memory
 *
 * @param
 *		layer : 	the Layer to be free
 */
void destructLayer(Layer** layer);

/* @function
 *		free a Neurone from the memory
 *
 * @param
 *		neurone : 	the Neurone to be free
 */
void destructNeurone(Neurone** neurone);

/* @function
 *		visualize a neural network in the terminal
 *
 * @param
 *		neuralNetwork : 	the NeuralNetwork to be shown
 */
void printNeuralNetwork(NeuralNetwork* neuralNetwork);

/* @function
 *		get the output of a neural network
 *
 * @param
 *		neuralNetwork : 	the neural network that will compute the inputs into outputs
 *		inputs :			a list containing the inputs we will put into the neural network
 *							their is as many input as the number of neurons in the first layer
 *
 * @return
 *		a list of outputs. Their is as much output as neurons on the last layer of the neural network
 */
float* getOutputOfNeuralNetwork(NeuralNetwork* neuralNetwork, float* inputs);

/* @function
 *		get the output of a Layer
 *
 * @param
 *		layer : 	the layer that will compute the inputs into outputs
 *		inputs :	a list containing the inputs we will put into each neurons of the layer
 *
 * @return
 *		the output of the neurone. Their is as much output as neurons on the layer
 */
float* getOutputOfLayer(Layer* layer, float* inputs);

/* @function
 *		get the output of neurone
 *
 * @param
 *		neurone : 	the neurone that will compute the inputs into a single output
 *		inputs :	a list containing the inputs we will put into the neurone
 *					their is as many input as indicated in the neurone (nbInput)
 *
 * @return
 *		the output of the neurone
 */
float getOutputOfNeurone(Neurone* neurone, float* inputs);

/* @function
 *		use after each neurones to transform the output. It is the transfer function of each neurons
 *
 * @param
 *		x : 	the input given to the function
 *
 * @return
 *		the output of the function
 */
float neuroneTransferFunction(float x);

/* @function
 *		work the same as neuroneTransferFunction but it is the derivative of the transfer function
 *
 * @param
 *		x : 	the input given to the function. it must be an ouput of the transfer function
 *
 * @return
 *		the output of the function
 */
float neuroneDerivateTransferFunction(float x);


/* @function
 *		check if the neural network return the reference output if you give it specific inputs.
 *		if it doesn't, correct the weights of each neurons with the retropropagation technique
 *
 * @param
 *		neuralNetwork : 	the neural network we will check and correct
 *		inputs : 			the inputs we will give to the neural network
 *		referenceOutputs :	the output the neural network is suppose to give from the given inputs
 *		learningSpeed : 	the amount of wich we will modify each weight when we correct them.
 *							the higher it is, the faster the neural network learn but the less it is precise
 *		errorMargin : 		the error the neural network is allowd to do.
 *							it is basicly the maximum difference between the outputs of the neural network and
 *							the referenceOutputs
 *
 * @return
 *		true if the outputs of the neural network from the given inputs is close enought to the referenceOutputs
 *		false if not, and if correction has been done
 */
bool superviseLearningNeuralNetwork(NeuralNetwork* neuralNetwork, float* inputs, float* referenceOutputs, float learningSpeed, float errorMargin);

/* @function
 *		save a neural network in a file
 *
 * @param
 *		neuralNetwork : 	the neural network we will save
 *		path : 				the path to the file where we'll save the neural network
 *
 * @return
 *		return true if the file has correctly been saved, false otherwise
 */
bool saveNeuralNetwork(NeuralNetwork* neuralNetwork, const char* path);

/* @function
 *		load a neural network from a file
 *
 * @param
 *		path : 				the path to the file where we'll load the neural network
 
 * @return
 *		the loaded neural network. If the function can't read it, return NULL
 */
NeuralNetwork* loadNeuralNetwork(const char* path);

