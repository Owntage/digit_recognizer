//
// Created by Owntage on 12/13/2017.
//

#ifndef NETWORK_SHOOTER_PROJECT_PIXEL_FIELD_H
#define NETWORK_SHOOTER_PROJECT_PIXEL_FIELD_H

#include <SFML/Graphics.hpp>

#define TILE_SIZE 10
#define IMAGE_WIDTH 28

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

	void paint(const vector<double>& imageVec) {
		for (int i = 0; i < IMAGE_WIDTH; i++)
		{
			for (int j = 0; j < IMAGE_WIDTH; j++)
			{
				int intensity = (int) (imageVec[i + j * IMAGE_WIDTH] * 255.0);
				intensity = 255 - intensity;
				pixels[i][j].setFillColor(sf::Color(intensity, intensity, intensity));
			}
		}
	}
};

#endif //NETWORK_SHOOTER_PROJECT_PIXEL_FIELD_H
