#include "Net.h"

using namespace std;

int main()
{
	Net myNet = Net();
	int c = 1;
	int corr1 = 0, corr2 = 0;
	int n;
	cout << "Fill randomly weights - 0, load weights - 1 : ";
	cin >> n;
	switch (n)
	{
	case 0:
	{
		myNet.fillRandomly();
	}break;
	default:
	{
		myNet.loadWeights();
	}break;
	}
	// the end of loading weights
	do {
		cout << "Learn - 0 \nTest - 1 \nMy_Picture - 2 \nLearn_fully - 3 \nLearn 5 times and test - 4 \n" << endl;
		cin >> n;
		cout << endl;
		try
		{
			switch (n)
			{
			case 0:		// learn
			{
				myNet.learn();
			}break;
			case 1:		// test
			{
				cout << endl << "Cost of the net: ";
				cout << myNet.test(corr1, corr2) << endl;
				cout << "Results:" << endl;
				int numberOfImages = 10000;
				cout << "Number of good answers from " << myNumberOfImages << " pictures: " << corr1 + corr2 << endl;
				cout << "Number of good before " << myNumberOfImages / 2 << ": " << corr1 << endl;
				cout << "Number of good after " << myNumberOfImages / 2 << ": " << corr2 << endl;
			}break;
			case 2:		// recognize given photo
			{
				cout << myNet.recognizePicture();
			}break;
			case 3:		// learning as long as cost is too big
			{
				float cost;
				float prev_cost = 6;
				int learnIterations = 0;
				while ((cost = myNet.test(corr1, corr2)) > 0.1 && (prev_cost - cost) > 0.01 && learnIterations < 15)
				{
					cout << " Current cost: " << cost;
					myNet.learn();
					cout << " iteration: " << learnIterations + 1 << endl;
					learnIterations++;
					prev_cost = cost;
				}
				cout << " Current cost: " << cost << endl;
				cout << "Number of learning iterations: " << learnIterations << endl;
			}break;
			case 4:		// learn some iterations
			{
				for (int i = 0; i < 10; i++)
				{
					myNet.learn();
					cout << i << " ";
				}
				cout << endl << "Cost of the net: ";
				cout << myNet.test(corr1, corr2) << endl;
				cout << "Results:" << endl;
				int numberOfImages = 10000;
				cout << "Number of good answers from " << myNumberOfImages << " pictures: " << corr1 + corr2 << endl;
				cout << "Number of good before " << myNumberOfImages / 2 << ": " << corr1 << endl;
				cout << "Number of good after " << myNumberOfImages / 2 << ": " << corr2 << endl;
			}break;
			}

		}
		catch (exception &e)
		{
			cout << e.what();
		}
		cout << endl << endl << "Press 1 to repead: ";
		cin >> c;
	} while (c);
	myNet.saveWeights();
	cout << endl << "The end" << endl;
	Sleep(2000);
	getchar(); getchar();
	return 0;
}
