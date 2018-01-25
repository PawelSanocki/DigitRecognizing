# DigitRecognizing

C++ ANN

Implementation of neural network in order to recognize digits. ANN learns using data from MNIST.

The net is created using three classes: Net, Layer, Neutron.
*************************************************************************************************************************
Neuron:

contains information about activation value, input value, pointers to each neuron of previous layer and weights of the connections, bias and error.
Methods:

void setValue(float v);
setting value using sigmoidal function

void getValue();
getting values from previous layer multiplying by corresponding veights adding bias and calling setValue

float cost(float v);
calculating cost, needed only in last layer

void fillRandomly();
settting bias and weights randomly

void get_error(int number_of_the_asking_neuron);
collecting the error from following layer's neurons

void correct_weights();
correcting weights using error and learning factor

void correct_bias();
correcting bias using error and learning factor

float derivative();
derivative of sigmoidal function

*************************************************************************************************************************
Layer:

contains vector of neurons, makes implemantation more intuitional
*************************************************************************************************************************
Net:

contains vector of layers
Methods:

Net(int nNeuronFirst = 28 * 28, int nNeuronHidden = 16, int nNeuronLast = 10)
create net with given number of neurons in each layer

void connectLayers(int nNeuronFirst, int nNeuronHidden, int nNeuronLast);
creating pointers for Neutrons and connectiong the layers

float* recognize(float *results);
pushes signal through the net and returns table with results (values of neurons of last layer)

void fillRandomly();
setting weights and biases randomly

void loadWeights();
loading weights and biases from file

void saveWeights();
saving weights and biases to file

void learn();
reading the label of correct answers and pushing values to first layer and calling recognize and improve

void improve(float* results, int label);
correcting the weights and biases basing on table of outputs and wanted result

float test(int& corr, int &corr2);
testing and retorning the number of good in first half and the second, simmilar to learn, returns average cost of the net of testing samples

int recognizePicture();
recognizing digit given in .bmp format and returning result

float netError(int number);
calculating the nets error





