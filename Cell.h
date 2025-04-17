#pragma once
#include <vector>
using namespace std;

class Cell {
public:

	int posX;
	int poxY;

	bool isAlive = 0;

	int neigh = 0;

	void countNeigh(int x, int y, const vector<vector<Cell>>& Plane) {
		int result = 0;

		for (int i = x - 1; i <= x + 1; i++) {
			for (int j = y - 1; j <= y + 1; j++) {  // Fixed loop condition
				// Skip the cell itself
				if (i == x && j == y) continue;

				// Check boundaries correctly
				if (i >= 0 && i < Plane.size() &&       // Check row boundaries
					j >= 0 && j < Plane[i].size()) {    // Check column boundaries
					if (Plane[i][j].isAlive) result++;
				}
			}
		}

		neigh = result;
	}

};