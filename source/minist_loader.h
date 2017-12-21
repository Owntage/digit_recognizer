#ifndef NETWORK_SHOOTER_PROJECT_MINIST_LOADER_H
#define NETWORK_SHOOTER_PROJECT_MINIST_LOADER_H

#include <vector>
#include <fstream>
#include <string>
#include <stdint.h>

using namespace std;

#define ROWS 28
#define COLUMNS 28
#define SIZE 60000

vector<vector<double> > extractImages(std::string filename)
{
	ifstream input;
	input.open(filename, ios::in | ios::binary);
	int unused[2];
	input.read((char*) unused, sizeof(int) * 4);

	vector<vector<double> > result;
	for (int i = 0; i < SIZE; i++)
	{
		vector<double> image;
		for (int j = 0; j < ROWS * COLUMNS; j++)
		{
			unsigned char pixel;
			input.read((char*)&pixel, 1);
			image.push_back(((double) pixel) / 255.0);
		}
		result.push_back(image);
	}

	return result;
}

vector<vector<double> > extractLabels(std::string filename, char targetDigit)
{
	ifstream input;
	input.open(filename, ios::in | ios::binary);
	int unused[2];
	input.read((char*) unused, sizeof(int) * 2);

	vector<vector<double> > result;
	for (int i = 0; i < SIZE; i++)
	{
		vector<double> label;
		unsigned char digit;
		label.resize(10, 0);
		input.read((char*)&digit, 1);
		label[digit] = 1.0;
		result.push_back(label);
	}

	return result;
}

#endif //NETWORK_SHOOTER_PROJECT_MINIST_LOADER_H
