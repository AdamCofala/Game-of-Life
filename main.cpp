#include <iostream>
#include <vector>
#include <windows.h>
#include <conio.h>
#include <string>
#include <cstdlib>
#include <omp.h> 
#include <fstream>
#include <cassert>
#include <random>

#include "Cell.h"
#include "fileIO.h"

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

fileIO fIO;

bool shouldRun = true;
bool shouldClose = false;
bool shouldSave = false;
int generation = 0;
int population = 0;

int height = 300;
int width =  300;
int type  = 1;

random_device seed;
mt19937 gen(seed());
uniform_int_distribution<int> randX(0, height);
uniform_int_distribution<int> randY(0, width);

unsigned int fontSize = 1;

vector<vector<Cell>> Plane(height, vector<Cell>(width));

void processKeyInput(vector<vector<Cell>>& Plane);

void genStep(vector<vector<Cell>>& Plane) {

	population = 0;
	generation++;

#pragma omp parallel for collapse(2) schedule(dynamic, 32)
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Plane[i][j].countNeigh(i, j, Plane);
		}
	}

	// Then apply the rules to all cells
#pragma omp parallel for reduction(+:population) collapse(2) schedule(dynamic, 32)
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {


			// Apply Game of Life rules
			if (Plane[i][j].isAlive) {
				if (Plane[i][j].neigh < 2 || Plane[i][j].neigh > 3) {
					Plane[i][j].isAlive = false;
				}
				else {
#pragma omp atomic
					population++;
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

void genLargeStep(vector<vector<Cell>>& Plane) {
	population = 0;
	generation++;

	// Parallel neighbor counting
#pragma omp parallel for collapse(2) schedule(dynamic, 32)
	for (int i = 0; i < Plane.size(); i++) {
		for (int j = 0; j < Plane[i].size(); j++) {
			Plane[i][j].countNeigh(i, j, Plane, 5);
		}
	}

	// Parallel state update
#pragma omp parallel for reduction(+:population) collapse(2) schedule(dynamic, 32)
	for (int i = 0; i < Plane.size(); i++) {
		for (int j = 0; j < Plane[i].size(); j++) {
			if (Plane[i][j].isAlive) {
				if (!(Plane[i][j].neigh>=34 && Plane[i][j].neigh<=58)) {
					Plane[i][j].isAlive = false;
					Plane[i][j].isDying = true;
				}
				else {
#pragma omp atomic
					population++;
				}
			}
			else if (Plane[i][j].isDying) {
				Plane[i][j].isAlive = false;
				Plane[i][j].isDying = false;
			}
			else {
				if (Plane[i][j].neigh >= 34 && Plane[i][j].neigh <= 45) {
					Plane[i][j].isAlive = true;
#pragma omp atomic
					population++;
				}
			}
		}
	}
}

void Render(const vector<vector<Cell>>& Plane, HANDLE& hConsole) {
	if (Plane.empty() || Plane[0].empty()) return;

	const size_t rows = Plane.size();
	const size_t cols = Plane[0].size();
	const int info_lines = 2; // Dodatkowe linie na informacje
	const size_t bufferWidth = 2 * cols;
	const size_t total_buffer_rows = info_lines + rows;

	vector<CHAR_INFO> buffer(total_buffer_rows * bufferWidth);


	// Wypełnianie siatki
	for (int y = 0; y < rows; ++y) {
		for (int x = 0; x < cols; ++x) {
			const Cell& cell = Plane[y][x];


			WORD color;
			if (cell.isAlive) color = 0x0B;  // Frost blue (#00FFFF)  
			else if (cell.isDying) color = 0x03;  // Deep teal (#008080)  
			else color = 0x00;  // black

			size_t bufferPos = (info_lines + y) * bufferWidth + 2 * x;

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

		buffer[0 * bufferWidth + x].Char.UnicodeChar = margin[x];
		buffer[0 * bufferWidth + x].Attributes = 0x0B;
		buffer[bufferWidth + x].Char.UnicodeChar = margin[x];
		buffer[bufferWidth + x].Attributes = 0x0B;

		buffer[(rows)*bufferWidth + x].Char.UnicodeChar = gen_text[x];
		buffer[(rows)*bufferWidth + x].Attributes = 0x0B;

		buffer[(rows + 1) * bufferWidth + x].Char.UnicodeChar = pop_text[x];
		buffer[(rows + 1) * bufferWidth + x].Attributes = 0x0B;
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
	fontInfo.dwFontSize.X = fontSize;  // Font width
	fontInfo.dwFontSize.Y = 2 * fontSize;  // Font height
	fontInfo.FontFamily = FF_DONTCARE;
	fontInfo.FontWeight = FW_NORMAL;

	// Choose a TrueType font that supports Unicode
	wcscpy_s(fontInfo.FaceName, L"Consolas");

	SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);

	COORD bufferSize = { static_cast<SHORT>(bufferWidth), static_cast<SHORT>(total_buffer_rows) };
	SetConsoleScreenBufferSize(hConsole, bufferSize);

	SMALL_RECT windowSize = { 0, 0, static_cast<SHORT>(bufferWidth - 1), static_cast<SHORT>(total_buffer_rows - 1) };
	SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

	COORD bufferCoord = { 0, 0 };
	SMALL_RECT writeArea = { 0, 0, static_cast<SHORT>(bufferWidth - 1), static_cast<SHORT>(total_buffer_rows - 1) };

	WriteConsoleOutputW(hConsole, buffer.data(), bufferSize, bufferCoord, &writeArea);
}



void chooseType() {

	system("cls");

	cout << "1. Classic Game of Life" << endl;
	cout << "2. Larger than Life" << endl;

	char key;
	cin >> key;	

	switch (key - '0') {
	case 1: type=0; break;
	case 2: type=1; break;
	default: 	MessageBoxA(nullptr, "Wrong input type! ", "Key Input Error", MB_ICONERROR | MB_OK); chooseType();
	}
	
}

void intypeMenu(){

	system("cls");

	cout << "+----------------------------+" << endl;
	cout << "|           MENU             |" << endl;
	cout << "+----------------------------+" << endl;
	cout << "| 1. Run new simulation      |" << endl;
	cout << "| 2. Save to file            |" << endl;
	cout << "| 3. Import from file        |" << endl;
	cout << "+----------------------------+" << endl;
	cout << "Chosee option (1-3): ";

	char key;
	cin >> key;

	switch (key-'0') {
	case 1:  return; break;
	case 2:  shouldSave = true; fIO.dialogOutput(); break;
	case 3:  fIO.dialogInput(); fIO.fileInput(Plane); break;
	default: MessageBoxA(nullptr, "Wrong input menu option! ", "Key Input Error", MB_ICONERROR | MB_OK); intypeMenu();
	}

}

int main() {


	initGosperGliderGun(Plane);	

	omp_set_num_threads(omp_get_max_threads());

	chooseType();
	intypeMenu();

	while (!shouldClose) {

		if (shouldRun) {

			switch (type) {
			case 0: genStep(Plane); break;
			case 1: genLargeStep(Plane); break;
			default: // Handle unexpected values
				cerr << "Invalid simulation type!\n";
				break;
			}

		}

		processKeyInput(Plane);
		Render(Plane, hConsole);
		
	}
	exit(0);

	return 0;

}

void processKeyInput(vector<vector<Cell>> &Plane) {

	if (_kbhit()) {
		char key = _getch();
		if (key == 'a') {
			shouldRun = !shouldRun;

			if(shouldSave) fIO.fileOutput(Plane);
		}
		if (key == 'g') {
			switch (type) {
			case 0: initGlider(randX(gen), randY(gen), Plane); break;
			case 1: initLargeGlider(randX(gen), randY(gen), Plane, randX(gen)%4); break;
			default: // Handle unexpected values
				cerr << "Invalid simulation type!\n";
				break;
			}
		}
		if (key == 'c') {
			clearPlane(Plane);
		}
		if (key == 'e') {
			shouldClose=true;
		}
	}

}