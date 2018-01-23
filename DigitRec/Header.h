#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <exception>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <queue>
#include <exception>
#include <ctime>
#include <fstream>
#include <queue>
#include <windows.h>

#define LEARNING_FACTOR 0.4																			// how fast should the net learn (how big impact on the weights and biases should have one sample)
#define M_E 2.71828182845904523536																	// needed for sigmoid function
#define N_HIDDEN 2																					// amount of hidden layers
#define myNumberOfImages numberOfImages																// needed to easily set number of images for testing the program
// numberOfImages
using namespace std;

unsigned char** read_mnist_images(string full_path, int& number_of_images, int& image_size);		// reading mnist images, saving the amount of them to number_of_images, and size to image_size
unsigned char* read_mnist_labels(string full_path, int& number_of_labels);							// reading labels
int biggest(float* tab);																			// returns index of the biggest number in the table of ten elements, needed to bing the final answer
float project(float number);																		//projecting 0 - 255 to -2 - 2, finnaly not used


#endif // HEADER_H_INCLUDED
