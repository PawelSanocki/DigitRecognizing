#include "Net.h"

Net::Net(int nNeuronFirst, int nNeuronHidden, int nNeuronLast)
{
	srand(time(NULL));
	net.push_back(Layer(nNeuronFirst));
	for (int i = 0; i<N_HIDDEN; i++)
	{
		net.push_back(Layer(nNeuronHidden));
	}
	net.push_back(Layer(nNeuronLast));

	connectLayers(nNeuronFirst, nNeuronHidden, nNeuronLast);
}

Net::~Net()
{
	//dtor
}

void Net::connectLayers(int nNeuronFirst, int nNeuronHidden, int nNeuronLast)
{
	for (int j = 0; j < net.size() - 1; j++)
	{
		for (unsigned int i = 0; i < net[j + 1].layer.size(); i++)
		{
			net[j].next.push_back(&net[j + 1].layer[i]);
		}
		for (unsigned int i = 0; i<net[j].layer.size(); i++)
		{
			net[j].layer[i].next = net[j].next;
		}
	}
	for (int j = 1; j < net.size(); j++)
	{
		for (unsigned int i = 0; i < net[j - 1].layer.size(); i++)
		{
			net[j].prev.push_back(&net[j - 1].layer[i]);
		}
		for (unsigned int i = 0; i<net[j].layer.size(); i++)
		{
			net[j].layer[i].prev = net[j].prev;
		}
	}
}

float* Net::recognize(float *results)
{
	// checking if pointers are correct
	for (int i = 1; i < net.size(); i++)
	{
		if (net[i].prev.size() != net[i - 1].layer.size())
		{
			throw runtime_error("Connecting failed!");
		}
	}
	for (int i = 0; i < net.size() - 1; i++)
	{
		if (net[i].next.size() != net[i + 1].layer.size())
		{
			throw runtime_error("Connecting failed!");
		}
	}
	// passing signals throught the net
	for (unsigned int i = 1; i < this->net.size(); i++)
	{
		for (unsigned int j = 0; j < net[i].layer.size(); j++)
		{
			net[i].layer[j].getValue();
		}
	}
	// giving and saving results
	for (unsigned int i = 0; i < net[net.size() - 1].layer.size(); i++)
	{
		results[i] = net[net.size() - 1].layer[i].value;
	}
	return results;
}

void Net::fillRandomly()
{
	for (int i = 1; i < net.size(); i++)
	{
		for (unsigned int j = 0; j < this->net[i].layer.size(); j++)
		{
			this->net[i].layer[j].fillRandomly();
		}
	}
}

void Net::loadWeights()
{
	ifstream file("file.txt");
	float floatNumber;
	for (int i = 1; i < net.size(); i++)
	{
		for (unsigned int j = 0; j < this->net[i].layer.size(); j++)
		{
			for (unsigned int k = 0; k < net[i - 1].layer.size(); k++)
			{
				file >> floatNumber;
				net[i].layer[j].weight.push_back((float)floatNumber);
			}
			file >> floatNumber;
			net[i].layer[j].bias = (float)floatNumber;
		}
	}
	file.close();
}

void Net::saveWeights()
{
	ofstream file("file.txt", ios::trunc);
	for (unsigned int i = 1; i < net.size(); i++)
	{
		for (unsigned int j = 0; j < this->net[i].layer.size(); j++)
		{
			for (unsigned int k = 0; k < net[i].layer[j].weight.size(); k++)
			{
				file << this->net[i].layer[j].weight[k] << endl;
			}
			file << this->net[i].layer[j].bias << endl;
		}
	}
	file.close();
}

void Net::improve(float* results, int label)
{
	// calculating the error of the last layer neurons
	for (unsigned int j = 0; j < net[net.size() - 1].layer.size(); j++)
	{
		if (j == label)
		{
			net[net.size() - 1].layer[j].error = net[net.size() - 1].layer[j].derivative() * (1 - results[j]);
		}
		else
		{
			net[net.size() - 1].layer[j].error = net[net.size() - 1].layer[j].derivative() * (0 - results[j]);
		}
	}
	// backpropagating the error
	for (unsigned int i = this->net.size() - 2; i > 0; i--)
	{
		for (unsigned int j = 0; j < net[i].layer.size(); j++)
		{
			net[i].layer[j].get_error(j);
		}
	}
	// correcting the weights and biases
	for (unsigned int i = 1; i < net.size(); i++)
	{
		for (unsigned int j = 0; j < net[i].layer.size(); j++)
		{
			net[i].layer[j].correct_weights();
			//net[i].layer[j].correct_bias();
			net[i].layer[j].error = 0;
		}
	}
}

float Net::test(int & corr, int & corr2)
{
	float results[10];
	int numberOfImages;
	int imageSize;
	float error = 0;		// error of the whole net for testing samples
	corr = 0; corr2 = 0;
	unsigned char* label = read_mnist_labels("t10k-labels.idx1-ubyte", numberOfImages);
	unsigned char** image = read_mnist_images("t10k-images.idx3-ubyte", numberOfImages, imageSize);
	int rows = 28, columns = 28;
	// printing some of the written digits ;)
	/*
	for (unsigned int i = 0; i < 10; i++)
	{
	for (int j = 0; j < rows; j++)
	{
	for (int k = 0; k < columns; k++)
	{
	if (image[i][j * 28 + k] > 100)
	cout << "0";
	else cout << " ";
	}
	cout << endl;
	}
	cout << endl << endl;
	}*/
	//
	// for each image recognizing
	for (int i = 0; i < myNumberOfImages; i++)	// numberOfImages
	{
		// setting values in the first layer
		for (int j = 0; j < rows; j++)
		{
			for (int k = 0; k < columns; k++)
			{
				net[0].layer[j * 28 + k].setValue((project(image[i][j * 28 + k])));
			}
		}
		error += netError(label[i]);
		// comparing with wanted result
		if (biggest(this->recognize(results)) == label[i])
		{
			if (i < myNumberOfImages / 2) corr++;
			else corr2++;
		}
	}
	return error / myNumberOfImages;
}

void Net::learn()
{
	int numberOfImages;
	int imageSize;
	float results[10];
	queue <float> newLabel;
	try
	{
		unsigned char *label = read_mnist_labels("train-labels.idx1-ubyte", numberOfImages);
		unsigned char **image = read_mnist_images("train-images.idx3-ubyte", numberOfImages, imageSize);
		int rows = 28, columns = 28;
		// printing some of the written digits ;)
		/*for (unsigned int i = 0; i < 10; i++)
		{
		for (int j = 0; j < rows; j++)
		{
		for (int k = 0; k < columns; k++)
		{
		if (image[i][j * 28 + k] > 100)
		cout << "0";
		else cout << " ";
		}
		cout << endl;
		}
		cout << endl << endl;
		}*/
		//
		// pushing results on the queue
		for (int i = 0; i < numberOfImages; i++)
		{
			newLabel.push(label[i]);
		}
		// for each image loading the first layer value and knowing the answer correcting the weights and biases
		for (unsigned int i = 0; i < myNumberOfImages; i++)
		{
			for (int j = 0; j < rows; j++)
			{
				for (int k = 0; k < columns; k++)
				{
					net[0].layer[j * 28 + k].setValue(project(image[i][j * 28 + k]));
				}
			}
			this->improve(this->recognize(results), newLabel.front());
			newLabel.pop();
		}
	}
	catch (exception &e)
	{
		cout << e.what() << " Learning error" << endl;
	}
	this->saveWeights();
}

int Net::recognizePicture()
{
	float results[10];
	ifstream f("picture.bmp", std::ifstream::binary);
	if (f.is_open())
	{
		// get length of file:
		f.seekg(0, f.end);
		int length = f.tellg();
		f.seekg(0, f.beg);
		// allocate memory:
		unsigned char * buffer = new unsigned char[length];
		// read data as a block:
		f.read((char*)buffer, length);
		f.close();
		// print content:
		//cout.write(buffer, length);

		// printing the number ;)
		/*
		for (int i = 1; i < 29; i++)
		{
		for (int j = 1; j < 29; j++)
		{
		if (buffer[(length - 1) - 3 * (i * 28 - j)] < 250) cout << 0;
		else cout << " ";
		}
		cout << endl;
		}
		*/
		// setting the values of the first 
		for (int i = 1; i < 29; i++)
		{
			for (int j = 1; j < 29; j++)
			{
				if (buffer[(length - 1) - 3 * (i * 28 - j)] < 250) net[0].layer[(i - 1) * 28 + (j - 1)].setValue(255);
				else net[0].layer[(i - 1) * 28 + (j - 1)].setValue(0);
			}
		}
		delete buffer;
	}
	// what is the number?
	int answer = biggest(this->recognize(results));
	// printing all results
	for (int i = 0; i < 10; i++)
	{
		cout << i << " : " << results[i] << endl;
	}
	//returning final answer
	cout << endl << "It is: " << endl;
	return answer;
}

float Net::netError(int number)
{
	float error = 0;

	for (int i = 0; i < 10; i++)
	{
		if (i == number)
		{
			error += pow(this->net[net.size() - 1].layer[i].value - 1, 2) / 2;
		}
		else
		{
			error += pow(this->net[net.size() - 1].layer[i].value - 0, 2) / 2;
		}

	}
	return error;
}
