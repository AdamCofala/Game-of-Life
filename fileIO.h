#pragma once
#include <fstream>
#include <vector>
#include "Cell.h"
#include <string>
#include <iostream>
using namespace std;


class fileIO {


	string output = "";
	string input = "";

public:

	void dialogOutput() {

		system("cls");

		cout << "For what name it should be save? : ";
		string filename;
		cin >> filename;

		output = filename + ".txt";
	}

	void dialogInput() {

		system("cls");

		cout << "Give me a name? : ";
		string filename;
		cin >> filename;

		input = filename + ".txt";
	}

	void fileInput(vector<vector<Cell>> &Plane) {

		ifstream in(input);

		if (!in) {
			throw ("Error opening file!");
			return;
		}

		Plane.clear();

		string line;

		while (getline(in, line)) {
			vector<Cell> row;
			for (char c : line) {
				Cell cell;
				cell.isAlive = (c == '1');
				row.push_back(cell);
			}
			Plane.push_back(row);
		}
	}



	void fileOutput(const vector<vector<Cell>>&Plane) {

		ofstream out(output);

		if (!out) {
			throw ("Error opening file!");
			return;
		}

		for (const auto x : Plane) {
			for (const auto y : x) {
				if (y.isAlive) out << '1';
				else out << '0';
			}
			out << "\n";
		}

	}

};