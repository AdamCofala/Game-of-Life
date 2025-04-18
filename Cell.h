#pragma once
#include <vector>
#include <omp.h> 
using namespace std;

class Cell {
public:

	int posX;
	int poxY;

	bool isAlive = 0;
	bool isDying = 0;

	int neigh = 0;

	void countNeigh(int x, int y, const vector<vector<Cell>>& Plane, int size = 1) {
		int result = 0;

		for (int i = x - size; i <= x + size; i++) {
			for (int j = y - size; j <= y + size; j++) {  // Fixed loop condition
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


void initGosperGliderGun(vector<vector<Cell>>& Plane) {

	// Left block
	Plane[5][1].isAlive = true;
	Plane[5][2].isAlive = true;
	Plane[6][1].isAlive = true;
	Plane[6][2].isAlive = true;

	// Left ship
	Plane[3][13].isAlive = true;
	Plane[3][14].isAlive = true;
	Plane[4][12].isAlive = true;
	Plane[4][16].isAlive = true;
	Plane[5][11].isAlive = true;
	Plane[5][17].isAlive = true;
	Plane[6][11].isAlive = true;
	Plane[6][15].isAlive = true;
	Plane[6][17].isAlive = true;
	Plane[6][18].isAlive = true;
	Plane[7][11].isAlive = true;
	Plane[7][17].isAlive = true;
	Plane[8][12].isAlive = true;
	Plane[8][16].isAlive = true;
	Plane[9][13].isAlive = true;
	Plane[9][14].isAlive = true;

	// Right ship
	Plane[1][25].isAlive = true;
	Plane[2][23].isAlive = true;
	Plane[2][25].isAlive = true;
	Plane[3][21].isAlive = true;
	Plane[3][22].isAlive = true;
	Plane[4][21].isAlive = true;
	Plane[4][22].isAlive = true;
	Plane[5][21].isAlive = true;
	Plane[5][22].isAlive = true;
	Plane[6][23].isAlive = true;
	Plane[6][25].isAlive = true;
	Plane[7][25].isAlive = true;

	// Right block
	Plane[3][35].isAlive = true;
	Plane[3][36].isAlive = true;
	Plane[4][35].isAlive = true;
	Plane[4][36].isAlive = true;
}

void initGlider(int x, int y, vector<vector<Cell>>& Plane) {

	if (y + 2 < Plane.size() && x + 2 < Plane[x].size()) {
		Plane[y][x + 1].isAlive = true;
		Plane[y + 1][x + 2].isAlive = true;
		Plane[y + 2][x].isAlive = true;
		Plane[y + 2][x + 1].isAlive = true;
		Plane[y + 2][x + 2].isAlive = true;
	}
}

void initRing(int x, int y, vector<vector<Cell>>& Plane) {
	int sizeX = Plane.size();
	int sizeY = Plane[0].size();

	int ringCenterX = x;
	int ringCenterY = y;
	int ringRadius = 8;
	for (int i = ringCenterX - ringRadius; i <= ringCenterX + ringRadius; i++) {
		for (int j = ringCenterY - ringRadius; j <= ringCenterY + ringRadius; j++) {
			int dx = abs(i - ringCenterX);
			int dy = abs(j - ringCenterY);
			if (dx + dy >= 6 && dx + dy <= 8) { // Create a diamond-shaped ring
				if (i >= 0 && i < sizeX && j >= 0 && j < sizeY) {
					Plane[i][j].isAlive = true;
				}
			}
		}
	}
}

void initSmothGlider(int x, int y, vector<vector<Cell>>& Plane, int direction = 0) {
	// Check if coordinates are valid
	if (x < 0 || y < 0 || x + 10 >= Plane.size() || y + 10 >= Plane[0].size()) {
		return; // Out of bounds
	}

	// Clear the area
	for (int i = x - 2; i <= x + 12; i++) {
		for (int j = y - 2; j <= y + 12; j++) {
			if (i >= 0 && j >= 0 && i < Plane.size() && j < Plane[0].size()) {
				Plane[i][j].isAlive = false;
			}
		}
	}

	// Pattern designed to create approximately 34-45 neighbors for birth
	// and maintain cells with 34-57 neighbors for survival

	// Create a densely populated square
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			Plane[x + i][y + j].isAlive = true;
		}
	}

	// Add asymmetric elements that might lead to movement
	Plane[x + 6][y].isAlive = true;
	Plane[x + 7][y].isAlive = true;
	Plane[x + 8][y + 1].isAlive = true;
	Plane[x + 6][y + 6].isAlive = true;
	Plane[x][y + 6].isAlive = true;
	Plane[x][y + 7].isAlive = true;
}
