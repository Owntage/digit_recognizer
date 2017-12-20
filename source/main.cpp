#include <vector>
#include <iostream>
#include "declarations.h"
#include "pixel_field.h"
#include "minist_loader.h"
#include <random>

using namespace std;

typedef vector<double> vec_d;

void showVectorVals(string label, vector<double> &v)
{
	cout << label << " ";
	for(unsigned i = 0; i < v.size(); ++i)
	{
		cout << v[i] << " ";
	}
	cout << endl;
}

void trainXor() {
	TrainingData trainData("trainingData.txt");
	//e.g., {3, 2, 1 }
	vector<unsigned> topology;
	//topology.push_back(3);
	//topology.push_back(2);
	//topology.push_back(1);

	trainData.getTopology(topology);
	Net myNet(topology);

	vector<double> inputVals, targetVals, resultVals;
	int trainingPass = 0;
	while(!trainData.isEof())
	{
		++trainingPass;
		cout << endl << "Pass" << trainingPass;

		// Get new input data and feed it forward:
		if(trainData.getNextInputs(inputVals) != topology[0])
			break;
		showVectorVals(": Inputs :", inputVals);
		myNet.feedForward(inputVals);

		// Collect the net's actual results:
		myNet.getResults(resultVals);
		showVectorVals("Outputs:", resultVals);

		// Train the net what the outputs should have been:
		trainData.getTargetOutputs(targetVals);
		showVectorVals("Targets:", targetVals);
		assert(targetVals.size() == topology.back());

		myNet.backProp(targetVals);

		// Report how well the training is working, average over recnet
		cout << "Net recent average error: "
			 << myNet.getRecentAverageError() << endl;
	}

	cout << endl << "Done" << endl;
}

void trainDigits(Net& net, vector<vec_d>& images, vector<vec_d>& labels)
{
	vec_d results;

	for (int i = 0; i < images.size(); i++) {
		for (int j = 0; j < 3; j++)
		{
			net.feedForward(images[i]);
			net.getResults(results);
			net.backProp(labels[i]);
		}
	}
}


int main()
{
	sf::RenderWindow window(sf::VideoMode(IMAGE_WIDTH * TILE_SIZE, IMAGE_WIDTH * TILE_SIZE), "input");
	window.setFramerateLimit(60);

	sf::View view;
	view.setCenter(IMAGE_WIDTH / 2, IMAGE_WIDTH / 2);
	view.setSize(IMAGE_WIDTH, IMAGE_WIDTH);

	window.setView(view);

	Field field;
	bool mousePressed = false;

	cout << "extracting images & labels" << endl;
	auto images = extractImages("train-images.idx3-ubyte");
	auto labels = extractLabels("train-labels.idx1-ubyte");
	cout << "extraction finished" << endl;
	field.paint(images[0]);

	cout << "train started" << endl;
	Net myNet({784, 100, 1});

	trainDigits(myNet, images, labels);

	cout << "train finished" << endl;


	float counter = 0;
	float correctCounter = 0;
	for (int i = 0; i < images.size(); i++)
	{
		myNet.feedForward(images[i]);
		vec_d result;
		myNet.getResults(result);
		double actualValue = labels[i][0];
		double predictedValue = result[0];
		if (actualValue > 0 == predictedValue > 0)
		{
			correctCounter++;
		}
		counter++;
	}

	cout << "accuracy: " << correctCounter / counter << endl;


	while (window.isOpen())
	{

		// Event processing
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Request for closing the window
			switch (event.type)
			{
				case sf::Event::Closed:
				{
					window.close();
					break;
				}
				case sf::Event::MouseButtonPressed:
				{
					mousePressed = true;
					break;
				}
				case sf::Event::MouseButtonReleased:
				{
					mousePressed = false;
					break;
				}
				case sf::Event::MouseMoved:
				{
					if (!mousePressed) break;
					int x = event.mouseMove.x / TILE_SIZE;
					int y = event.mouseMove.y / TILE_SIZE;
					if (x >= 0 && y >= 0 && x < IMAGE_WIDTH && y < IMAGE_WIDTH)
					{
						field.paint(x, y);
					}
					break;
				}
				case sf::Event::KeyPressed:
				{
					switch (event.key.code)
					{
						case sf::Keyboard::Delete:
						{
							field.reset();
							break;
						}
						case sf::Keyboard::Space:
						{
							vec_d image = field.getVector();
							vec_d result;
							myNet.feedForward(image);
							myNet.getResults(result);
							cout << "predicted value: " << result[0] << endl;
							break;
						}
						case sf::Keyboard::D:
						{
							vec_d image = field.getVector();
							vec_d results;
							cout << "enter actual digit: " << endl;
							int digit;
							cin >> digit;
							cout << endl;
							vec_d label;
							label.resize(10, -1.0f);
							label[digit] = 1.0f;
							myNet.feedForward(image);
							myNet.getResults(results);
							showVectorVals("results: ", results);
							myNet.backProp(label);
							myNet.getRecentAverageError();
							break;
						}
					}
					break;
				}
			}
		}

		window.setActive();
		field.draw(window);
		window.display();
	}

}
