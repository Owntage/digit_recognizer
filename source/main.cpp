#include <vector>
#include <iostream>
#include "declarations.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

#define TILE_SIZE 10
#define IMAGE_WIDTH 28

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

struct Field
{
	sf::RectangleShape pixels[IMAGE_WIDTH][IMAGE_WIDTH];

	Field()
	{
		for (int i = 0; i < IMAGE_WIDTH; i++)
		{
			for (int j = 0; j < IMAGE_WIDTH; j++)
			{
				pixels[i][j].setPosition(i, j);
				pixels[i][j].setFillColor(sf::Color::White);
				pixels[i][j].setSize(sf::Vector2f(1, 1));
				pixels[i][j].setOutlineThickness(0.1);
				pixels[i][j].setOutlineColor(sf::Color(128, 128, 128));
			}
		}
	}

	void draw(sf::RenderTarget& renderTarget)
	{
		for (int i = 0; i < IMAGE_WIDTH; i++)
		{
			for (int j = 0; j < IMAGE_WIDTH; j++)
			{
				renderTarget.draw(pixels[i][j]);
			}
		}
	}

	void reset()
	{
		for (int i = 0; i < IMAGE_WIDTH; i++)
		{
			for (int j = 0; j < IMAGE_WIDTH; j++)
			{
				pixels[i][j].setFillColor(sf::Color::White);
			}
		}
	}

	void paint(int i, int j)
	{
		pixels[i][j].setFillColor(sf::Color::Black);
	}
};

int main()
{
	//trainXor();

	sf::RenderWindow window(sf::VideoMode(IMAGE_WIDTH * TILE_SIZE, IMAGE_WIDTH * TILE_SIZE), "input");
	window.setFramerateLimit(60);

	sf::View view;
	view.setCenter(IMAGE_WIDTH / 2, IMAGE_WIDTH / 2);
	view.setSize(IMAGE_WIDTH, IMAGE_WIDTH);

	window.setView(view);

	Field field;
	bool mousePressed = false;

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
