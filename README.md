# Cellular Automata: Conway's Game of Life and Extended Variant

This project implements Conway's Game of Life cellular automaton simulation with both the classic ruleset and an extended variant with larger neighborhoods. It features multithreaded processing using OpenMP for performance optimization and a console-based visualization.

## Overview

The Game of Life is a cellular automaton devised by mathematician John Conway in 1970. It consists of a grid of cells that evolve according to simple rules based on their neighbors' states. This implementation includes:

- Standard Conway's Game of Life rules (survival with 2-3 neighbors, birth with exactly 3)
- Extended variant with larger neighborhood radius and different survival/birth thresholds
- Multiple preset patterns (Gosper Glider Gun, Gliders, Rings)
- Interactive controls during simulation

| Classic Game of Life | Extended Variant |
|:-------------------:|:----------------:|
| ![Classic Game of Life](https://github.com/user-attachments/assets/352c6b0e-0e13-4425-896f-ab39316bf72c) | ![Extended Variant](https://github.com/user-attachments/assets/e4a1cd6b-cf10-4ea3-8843-19a863ba496a) |
| *Standard Conway's Game of Life with traditional rules* | *Extended variant with larger neighborhood and different thresholds* |

## Features

- **Dual Simulation Modes**:
  - Classic Game of Life (type 0)
  - Extended variant, which is called "Larger than Life" with 5-cell radius neighborhood (type 1)
- **Multi-threading** with OpenMP for faster processing of large grids
- **Interactive Controls**:
  - `a` - Toggle pause/resume simulation
  - `g` - Add random glider/large glider based on simulation type
  - `c` - Clear the simulation plane
- **Visual Representation**:
  - Console-based Unicode rendering with colored cells
  - Information display showing generation count and population size ( Right now it is really small, i'm going to fix it in the future)

## Implementation Details

### Cell Class
Each cell manages its own state and can count its neighbors within a specified radius.

### Simulation Rules
- **Classic Game of Life**:
  - A live cell survives if it has 2 or 3 live neighbors
  - A dead cell becomes alive if it has exactly 3 live neighbors
  - Otherwise, cells die or remain dead

- **Extended Variant (Larger than Life)**:
  - Uses a 5-cell radius neighborhood (examining up to 121 cells)
  - A live cell survives if it has 34-58 neighbors
  - A dead cell becomes alive if it has 34-45 neighbors
  - Includes a dying state transition for smoother visuals

### Patterns
- **Gosper Glider Gun**: A pattern that continuously creates gliders
- **Glider**: A simple pattern that moves diagonally across the grid
- **Ring**: A diamond-shaped ring pattern
- **Large Glider**: A dense pattern designed for the extended variant

## Requirements

- Windows OS (uses Windows Console API)
- C++ compiler with OpenMP support
- Console that supports Unicode characters

## Building and Running

1. Compile the source code with OpenMP enabled:
   ```
   g++ -fopenmp -o game_of_life main.cpp
   ```

2. Run the executable:
   ```
   ./game_of_life
   ```

## Customization

You can modify these parameters in the main.cpp file:
- `height` and `width`: Dimensions of the simulation grid
- `type`: Simulation mode (0 for classic, 1 for extended)
- `fontSize`: Adjust the console font size for display

## License

[MIT License](https://github.com/AdamCofala/Game-of-Life/blob/master/LICENSE)

## Future Improvements

- Configuration file or command-line arguments for simulation parameters
- More pattern presets and ability to load custom patterns
- Platform-independent rendering options
- Save/load simulation state
