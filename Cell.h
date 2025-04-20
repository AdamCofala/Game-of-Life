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

void clearPlane(vector<vector<Cell>>& Plane) {
	for (int i = 0; i < Plane.size(); i++) {
		for (int j = 0; j < Plane[i].size(); j++) {
			Plane[i][j].isAlive = false;
		}
	}
}

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

void initLargeGlider(int x, int y, std::vector<std::vector<Cell>>& Plane, int direction = 0) {

    // --- Define the base pattern relative coordinates (dx, dy) ---
    // Based on the 11x10 grid provided previously.
    const std::vector<std::pair<int, int>> basePattern = {
        // Row 0
        {4, 0}, {5, 0}, {6, 0},
        // Row 1
        {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}, {7, 1},
        // Row 2
        {0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2}, {8, 2},
        // Row 3
        {0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {9, 3},
        // Row 4
        {0, 4}, {1, 4}, {2, 4}, {3, 4}, {9, 4},
        // Row 5
        {0, 5}, {1, 5}, {2, 5}, {3, 5}, {8, 5}, {9, 5},
        // Row 6
        {0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {6, 6}, {7, 6}, {8, 6}, {9, 6},
        // Row 7
        {1, 7}, {2, 7}, {3, 7}, {4, 7}, {5, 7}, {6, 7}, {7, 7}, {8, 7},
        // Row 8
        {2, 8}, {3, 8}, {4, 8}, {5, 8}, {6, 8}, {7, 8},
        // Row 9
        {3, 9}, {4, 9}, {5, 9}, {6, 9},
        // Row 10
        {4, 10}, {5, 10}, {6, 10}
    };

    const int baseWidth = 10;  // Width of the bounding box for the base pattern
    const int baseHeight = 11; // Height of the bounding box for the base pattern

    // --- Normalize direction and calculate rotated dimensions ---
    // Ensures direction is 0, 1, 2, or 3. Handles negative inputs.
    int normalizedDirection = ((direction % 4) + 4) % 4;

    int rotatedWidth, rotatedHeight;
    if (normalizedDirection == 0 || normalizedDirection == 2) { // 0 or 180 deg
        rotatedWidth = baseWidth;
        rotatedHeight = baseHeight;
    }
    else { // 90 or 270 deg
        rotatedWidth = baseHeight; // Dimensions swap
        rotatedHeight = baseWidth;
    }

    // --- Check if rotated pattern fits within bounds ---
    if (x < 0 || y < 0 || x + rotatedWidth > Plane.size() || y + rotatedHeight > Plane[0].size()) {
        // It might be useful to print the calculated rotated dimensions here for debugging
        std::cerr << "Error: Rotated pattern (width=" << rotatedWidth << ", height=" << rotatedHeight
            << " at " << x << "," << y << ") placement out of bounds." << std::endl;
        return;
    }

    // --- Clear the area for the rotated pattern ---
    // Use rotated dimensions for clearing area calculation
    int clearStartX = max(0, x - 2);
    int clearStartY = max(0, y - 2);
    // Use min with plane dimensions to avoid going out of bounds
    int clearEndX = min((int)Plane.size(), x + rotatedWidth + 2);
    int clearEndY = min((int)Plane[0].size(), y + rotatedHeight + 2);

    for (int i = clearStartX; i < clearEndX; ++i) {
        // Check row validity
        if (i >= 0 && i < Plane.size()) {
            for (int j = clearStartY; j < clearEndY; ++j) {
                // Check column validity for this specific row
                if (j >= 0 && j < Plane[i].size()) {
                    Plane[i][j].isAlive = false;
                }
            }
        }
    }

    // --- Apply the pattern with rotation ---
    for (const auto& p : basePattern) {
        int dx_base = p.first;
        int dy_base = p.second;
        int dx_rotated, dy_rotated;

        // Calculate rotated offsets based on the normalized direction
        switch (normalizedDirection) {
        case 0: // 0 degrees (no rotation)
            dx_rotated = dx_base;
            dy_rotated = dy_base;
            break;
        case 1: // 90 degrees clockwise: (dx, dy) -> (dy, W-1-dx)
            dx_rotated = dy_base;
            dy_rotated = (baseWidth - 1) - dx_base;
            break;
        case 2: // 180 degrees: (dx, dy) -> (W-1-dx, H-1-dy)
            dx_rotated = (baseWidth - 1) - dx_base;
            dy_rotated = (baseHeight - 1) - dy_base;
            break;
        case 3: // 270 degrees clockwise: (dx, dy) -> (H-1-dy, dx)
            dx_rotated = (baseHeight - 1) - dy_base;
            dy_rotated = dx_base;
            break;
        default: // Should not happen due to normalization
            dx_rotated = dx_base;
            dy_rotated = dy_base;
            break;
        }

        // Calculate final absolute coordinates on the Plane
        int finalX = x + dx_rotated;
        int finalY = y + dy_rotated;

        // Final check before writing (mostly redundant if initial check is correct, but safe)
        if (finalX >= 0 && finalX < Plane.size() && finalY >= 0 && finalY < Plane[0].size()) {
            // Check specific row size for safety with potentially jagged vectors
            if (finalY < Plane[finalX].size()) {
                Plane[finalX][finalY].isAlive = true;
            }
        }
        else {
            // This case should ideally not be reached if the initial boundary check is correct.
            // Could indicate an issue with rotation logic or plane dimensions.
            std::cerr << "Warning: Calculated rotated coordinate (" << finalX << "," << finalY << ") is out of bounds unexpectedly." << std::endl;
        }
    }
}