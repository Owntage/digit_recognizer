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
	for (unsigned i = 0; i < v.size(); ++i)
	{
		cout << v[i] << " ";
	}
	cout << endl;
}


void trainDigits(Net &net, vector<vec_d> &images, vector<vec_d> &labels)
{
	vec_d results;

	for (int i = 0; i < images.size(); i++)
	{
		for (int j = 0; j < 1; j++)
		{
			net.feedForward(images[i]);
			net.getResults(results);
			net.backProp(labels[i]);
		}
	}
}

int getDigit(vec_d vec)
{
	int result = 0;
	for (int i = 1; i < 10; i++)
	{
		if (vec[i] > vec[result])
		{
			result = i;
		}
	}
	return result;
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

	cout << "extracting images" << endl;
	auto images = extractImages("train-images.idx3-ubyte");


	cout << "input example: " << endl;
	showVectorVals("input", images[0]);


	cout << "extraction finished" << endl;

	vector<Net *> myNets;
	vector<double> accuracyVector;


	myNets.push_back(new Net({784, 100, 32, 32, 10}));
	auto labels = extractLabels("train-labels.idx1-ubyte", 0);

	cout << "input label: " << getDigit(labels[0]) << endl;


	for (int i = 0; i < 3; i++)
	{
		cout << "epoch: " << i << endl;
		trainDigits(*myNets.back(), images, labels);
	}


//validation
	float counter = 0;
	float correctCounter = 0;
	for (int i = 0; i < images.size(); i++)
	{
		myNets.back()->feedForward(images[i]);
		vec_d result;
		myNets.back()->getResults(result);
		int actualValue = getDigit(labels[i]);
		int predictedValue = getDigit(result);
		if (actualValue == predictedValue)
		{
			correctCounter++;
		}
		counter++;
	}
	accuracyVector.push_back(correctCounter / counter);

	cout << "train finished" << endl;
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
						for (int i = 0; i < 2; i++)
						{
							for (int j = 0; j < 2; j++)
							{
								field.paint(x - 1 + i, y - 1 + j);
							}
						}
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
							cout << "recognition: ";
							myNets.back()->feedForward(image);
							vec_d result;
							myNets.back()->getResults(result);
							for (char digit = 0; digit < 10; digit++)
							{
								cout << result[digit] << " ";
							}
							cout << endl;
							cout << "recognized digit: " << getDigit(result) << endl;
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
							//myNet.feedForward(image);
							//myNet.getResults(results);
							showVectorVals("results: ", results);
							//myNet.backProp(label);
							//myNet.getRecentAverageError();
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
