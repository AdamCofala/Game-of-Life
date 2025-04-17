#include <iostream>
#include <vector>
#include <Windows.h>
#include <conio.h>
#include <string>

#include "Cell.h"

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

bool shouldRun = true;
int generation = 0;
int population = 0;


void genStep(vector<vector<Cell>>& Plane) {
	population = 0;
	generation++;
	// First, count neighbors for all cells
	for (int i = 0; i < Plane.size(); i++) {
		for (int j = 0; j < Plane[i].size(); j++) {
			Plane[i][j].countNeigh(i, j, Plane);
		}
	}

	// Then apply the rules to all cells
	for (int i = 0; i < Plane.size(); i++) {
		for (int j = 0; j < Plane[i].size(); j++) {
			// Apply Game of Life rules
			if (Plane[i][j].isAlive) {
				
				population++;

				if (Plane[i][j].neigh < 2 || Plane[i][j].neigh > 3) {
					Plane[i][j].isAlive = false;
					population--;
				}
				// Live cell with 2 or 3 neighbors lives on (no change needed)
			}
			else {
				// Dead cell with exactly 3 neighbors becomes alive
				if (Plane[i][j].neigh == 3) {
					Plane[i][j].isAlive = true;
					population++;
				}
			}
		}
	}
}

//void Wypisz(const vector<vector<Cell>>& Plane, HANDLE& hConsole) {
//	
//
//	string result="";
//	for (int y = 0; y < Plane.size(); y++) {
//		for (int x = 0; x < Plane[y].size(); x++) {
//
//			if (Plane[y][x].isAlive) {
//				SetConsoleTextAttribute(hConsole, 10);
//				printf("■"); 
//			}
//			else {
//				SetConsoleTextAttribute(hConsole, 8);
//				printf("□");
//			}
//
//			cout << " ";
//			//Plane[y][x].isAlive ? result+="■" : result+="□";
//			//result += " ";
//		}
//		//result += "\n";
//		printf("\n");
//	}
//	cout << result;
//}

void Wypisz(const vector<vector<Cell>>& Plane, HANDLE& hConsole) {
	if (Plane.empty() || Plane[0].empty()) return;

	const int rows = Plane.size();
	const int cols = Plane[0].size();
	const int info_lines = 2; // Dodatkowe linie na informacje
	const int bufferWidth = 2 * cols;
	const int total_buffer_rows = info_lines + rows;

	vector<CHAR_INFO> buffer(total_buffer_rows * bufferWidth);


	// Wypełnianie siatki
	for (int y = 0; y < rows; ++y) {
		for (int x = 0; x < cols; ++x) {
			const Cell& cell = Plane[y][x];
			WORD color = cell.isAlive ? 0x0B : 0x08;

			int bufferPos = (info_lines + y) * bufferWidth + 2 * x;

			// Komórka
			buffer[bufferPos].Char.UnicodeChar = cell.isAlive ? L'■' : L'□';
			buffer[bufferPos].Attributes = color;

			// Spacjaz
			buffer[bufferPos + 1].Char.UnicodeChar = L' ';
			buffer[bufferPos + 1].Attributes = color;
		}
	}

	// Wypełnianie linii informacyjnych
	std::wstring gen_text = L"Generacja: " + std::to_wstring(generation) + L"  ";
	std::wstring pop_text = L"Populacja: " + std::to_wstring(population) + L"  ";

	std::wstring margin = L"";

	gen_text.resize(bufferWidth, L'□');
	pop_text.resize(bufferWidth, L'□');
	margin.resize(bufferWidth, L'□');

	for (int x = 0; x < bufferWidth; ++x) {
		// Generacja

		buffer[ 0*bufferWidth + x].Char.UnicodeChar = margin[x];
		buffer[ 0*bufferWidth + x].Attributes = 0x0B;
		buffer[   bufferWidth + x].Char.UnicodeChar = margin[x];
		buffer[   bufferWidth + x].Attributes = 0x0B;

		buffer[(rows) * bufferWidth + x].Char.UnicodeChar = gen_text[x];
		buffer[(rows) * bufferWidth + x].Attributes = 0x0B;
				
		buffer[(rows+1) * bufferWidth + x].Char.UnicodeChar = pop_text[x];
		buffer[(rows+1) * bufferWidth + x].Attributes = 0x0B;
	}

	// Konfiguracja konsoli

	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(hConsole, &cursorInfo);

	SetConsoleTitleW(L"Game of Life");

	SetConsoleOutputCP(CP_UTF8);

	CONSOLE_FONT_INFOEX fontInfo = { 0 };
	fontInfo.cbSize = sizeof(fontInfo);
	fontInfo.nFont = 0;
	fontInfo.dwFontSize.X = 5;  // Font width
	fontInfo.dwFontSize.Y = 10;  // Font height
	fontInfo.FontFamily = FF_DONTCARE;
	fontInfo.FontWeight = FW_NORMAL;

	// Choose a TrueType font that supports Unicode
	wcscpy_s(fontInfo.FaceName, L"Consolas");

	SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);

	COORD bufferSize = { static_cast<SHORT>(bufferWidth), static_cast<SHORT>(total_buffer_rows) };
	SetConsoleScreenBufferSize(hConsole, bufferSize);

	SMALL_RECT windowSize = {0, 0, static_cast<SHORT>(bufferWidth-1), static_cast<SHORT>(total_buffer_rows-1 )};
	SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

	COORD bufferCoord = { 0, 0 };
	SMALL_RECT writeArea = {0, 0, static_cast<SHORT>(bufferWidth - 1), static_cast<SHORT>(total_buffer_rows - 1)};

	WriteConsoleOutputW(hConsole, buffer.data(), bufferSize, bufferCoord, &writeArea);
}

// Function to initialize a Gosper Glider Gun
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

	Plane[y][x + 1].isAlive = true;
	Plane[y + 1][x + 2].isAlive = true;
	Plane[y + 2][x].isAlive = true;
	Plane[y + 2][x + 1].isAlive = true;
	Plane[y + 2][x + 2].isAlive = true;
}

int main() {

	vector<vector<Cell>> Plane(60, vector<Cell>(60));

	initGosperGliderGun(Plane);
	srand(time(NULL));

	
	
	while (true) {
	
		if (shouldRun) { 		
			genStep(Plane);
			initGlider(rand() % 50, rand() % 50, Plane);
		};
		
		if (_kbhit()) {
			char key = _getch();  
			if (key == 'a' || key == 'A') {
				shouldRun = !shouldRun;
			}
		}
		
		Wypisz(Plane,hConsole);
		Sleep(50);
	}

	return 0;
}