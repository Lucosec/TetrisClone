#include <raylib.h>
#include <iostream>
#include <string>

#define SCALE 3

#define CELL_WIDE 10
#define CELL_HIGH 20

#define WIDTH (CELL_WIDE - 1) * SCALE * 8
#define HEIGHT (CELL_HIGH - 1) * SCALE * 8

#define SPEED 0.2

typedef struct Timer {
    double startTime;   // Start time (seconds)
    double lifeTime;    // Lifetime (seconds)
} Timer;


void StartTimer(Timer* timer, double lifetime) {
    timer->startTime = GetTime();
    timer->lifeTime = lifetime;
}

bool TimerDone(Timer timer) {
    return GetTime() - timer.startTime >= timer.lifeTime;
}

double GetElapsed(Timer timer) {
    return GetTime() - timer.startTime;
}

bool AreGridsEqual(int grid1[CELL_HIGH][CELL_WIDE], int grid2[CELL_HIGH][CELL_WIDE]) {
    for (int y = 0; y < CELL_HIGH; y++) {
        for (int x = 0; x < CELL_WIDE; x++) {
            if (grid1[y][x] != grid2[y][x]) {
                return false; // Grids are not equal
            }
        }
    }
    return true; // Grids are equal
}

bool ClearCompleteLine(int grid[CELL_HIGH][CELL_WIDE]) {
    int line = 1;
    
    for (int y = CELL_HIGH-1; y > 0; y--) {
        for (int x = 0; x < CELL_WIDE; x++) {
            if (grid[y][x] == 3) {
                line++;
            }

            if (line == CELL_WIDE) {
                for (int x = 0; x < CELL_WIDE; x++){
                    grid[y][x] = 0;
                }

                for (int n = y; n > 0; n--) {
                    for (int x = 0; x < CELL_WIDE; x++) {
                        grid[n][x] = grid[n-1][x];      
                    }
                }
            }
        }
        line = 1;
    }

    return false;
}

bool RotateClockwise(int grid[CELL_HIGH][CELL_WIDE]) {
    int rotatedGrid[CELL_HIGH][CELL_WIDE] = {0}; // Correct dimensions for the rotated grid

    // Find the center of the active block (1)
    int centerX = -1;
    int centerY = -1;
    for (int y = 0; y < CELL_HIGH; y++) {
        for (int x = 0; x < CELL_WIDE; x++) {
            if (grid[y][x] == 1 || grid[y][x] == 2) {
                centerX = x;
                centerY = y;
                break;
            }
        }
        if (centerX != -1) {
            break;
        }
    }

    if (centerX == -1 || centerY == -1) {
        // No active block found
        return false;
    }

    for (int y = 0; y < CELL_HIGH; y++) {
        for (int x = 0; x < CELL_WIDE; x++) {
            if (grid[y][x] == 1 || grid[y][x] == 2) {
                // Calculate the new position after clockwise rotation around the center
                int xOffset = x - centerX;
                int yOffset = y - centerY;
                int newX = centerX - yOffset;
                int newY = centerY + xOffset;

                // Check if the new position is within bounds
                if (newX >= 0 && newX < CELL_WIDE && newY >= 0 && newY < CELL_HIGH) {
                    rotatedGrid[newY][newX] = grid[y][x];
                } else {
                    return false; // Rotation is not possible
                }
            } else {
                // Keep the non-active blocks in their original positions
                rotatedGrid[y][x] = grid[y][x];
            }
        }
    }

    // Copy the rotated grid back to the original grid
    for (int y = 0; y < CELL_HIGH; y++) {
        for (int x = 0; x < CELL_WIDE; x++) {
            grid[y][x] = rotatedGrid[y][x];
        }
    }

    return true; // Rotation succeeded
}



bool RotateCounterclockwise(int grid[CELL_HIGH][CELL_WIDE]) {
    int rotatedGrid[CELL_HIGH][CELL_WIDE] = {0};

    // Find the center of the active block (1)
    int centerX = -1;
    int centerY = -1;
    for (int y = 0; y < CELL_HIGH; y++) {
        for (int x = 0; x < CELL_WIDE; x++) {
            if (grid[y][x] == 1 || grid[y][x] == 2) {
                centerX = x;
                centerY = y;
                break;
            }
        }
        if (centerX != -1) {
            break;
        }
    }

    if (centerX == -1 || centerY == -1) {
        // No active block found
        return false;
    }

    for (int y = 0; y < CELL_HIGH; y++) {
        for (int x = 0; x < CELL_WIDE; x++) {
            if (grid[y][x] == 1 || grid[y][x] == 2) {
                // Calculate the new position after counterclockwise rotation around the center
                int xOffset = x - centerX;
                int yOffset = y - centerY;
                int newX = centerX + yOffset;
                int newY = centerY - xOffset;

                // Check if the new position is within bounds
                if (newX >= 0 && newX < CELL_WIDE && newY >= 0 && newY < CELL_HIGH) {
                    rotatedGrid[newY][newX] = grid[y][x];
                } else {
                    return false; // Rotation is not possible
                }
            } else {
                // Keep the 3's in their original positions
                rotatedGrid[y][x] = grid[y][x];
            }
        }
    }

    // Copy the rotated grid back to the original grid
    for (int y = 0; y < CELL_HIGH; y++) {
        for (int x = 0; x < CELL_WIDE; x++) {
            grid[y][x] = rotatedGrid[y][x];
        }
    }

    return true; // Rotation succeeded
}

void GenerateNextFigure(int grid[CELL_HIGH][CELL_WIDE], int &currentFigure, int &nextFigure) {
    currentFigure = nextFigure;
    nextFigure = GetRandomValue(0, 6);  // Randomly select the next figure (0 to 6)

    // Define Tetris figures (1) and their initial positions (2)
    int figures[7][4][4] = {
        // I - Line
        {
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {1, 1, 1, 1},
            {0, 0, 0, 0}
        },
        // J - L Shape
        {
            {0, 0, 0, 0},
            {1, 2, 2, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0}
        },
        // L - Mirrored L Shape
        {
            {0, 0, 0, 0},
            {2, 2, 1, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0}
        },
        // O - Square
        {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        },
        // S - S Shape
        {
            {0, 0, 0, 0},
            {2, 1, 1, 0},
            {1, 1, 2, 0},
            {0, 0, 0, 0}
        },
        // T - T Shape
        {
            {0, 0, 0, 0},
            {2, 1, 2, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0}
        },
        // Z - Z Shape
        {
            {0, 0, 0, 0},
            {1, 1, 2, 0},
            {2, 1, 1, 0},
            {0, 0, 0, 0}
        }
    };

    // Copy the selected figure into the grid (1's)
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            grid[y][x + (CELL_WIDE / 2 - 2)] = figures[currentFigure][y][x];
        }
    }
}

void UpdateGrid(int grid[CELL_HIGH][CELL_WIDE]) {
    int old_grid[CELL_HIGH][CELL_WIDE] = {0};
    for (int y = 0; y < CELL_HIGH; y++) {
        for (int x = 0; x < CELL_WIDE; x++) {
            old_grid[y][x] = grid[y][x];
        }
    }

    for (int y = CELL_HIGH-1; y > 0; y--) {
        for (int x = 0; x < CELL_WIDE; x++) {
            if (grid[y][x] == 2 && grid[y+1][x] == 3 && y < CELL_HIGH-1) {
                grid[y][x] = 0;
            }

            if (grid[y][x] == 1 && grid[y+1][x] == 3) {
                for (int c = 0; c < CELL_HIGH; c++) {
                    for (int z = 0; z < CELL_WIDE; z++) {
                        if (grid[c][z] == 1) {
                            grid[c][z] = 3;
                        }
                        if (grid[c][z] == 2) {
                            grid[c][z] = 0;
                        }
                    }
                }
            }
        }
    }

    

    for (int y = CELL_HIGH-1; y > 0; y--) {
        for (int x = 0; x < CELL_WIDE; x++) {
            if (grid[y][x] != 0 && grid[y][x] != 3 && grid[y][x] != 4 && grid[y+1][x] == 0 && y < CELL_HIGH-1) {
                int buffer = grid[y][x];
                grid[y][x] = 0;
                grid[y+1][x] = buffer;                
            }
        }
    }

    int after_grid[CELL_HIGH][CELL_WIDE] = {0};
    for (int y = 0; y < CELL_HIGH; y++) {
        for (int x = 0; x < CELL_WIDE; x++) {
            after_grid[y][x] = grid[y][x];
        }
    }



    if (AreGridsEqual(old_grid, after_grid)) {
        for (int y = CELL_HIGH-1; y > 0; y--) { // Start from the second-to-last row
            for (int x = 0; x < CELL_WIDE; x++) {
                if (grid[y][x] == 1 && grid[y - 1][x] == 2 && grid[y-2][x] != 1) {
                    grid[y-1][x] = 0;
                }
            }
        }

        for (int y = 0; y < CELL_HIGH; y++) { // Start from the second-to-last row
            for (int x = 0; x < CELL_WIDE; x++) {
                if ((grid[y+1][x] == 1 || grid[y-1][x] == 1 || grid[y][x+1] == 1 || grid[y][x-1] == 1) && grid[y][x] == 1) {
                    grid[y][x] = 3;
                }

                if ((grid[y+1][x] == 3 || grid[y-1][x] == 3 || grid[y][x+1] == 3 || grid[y][x-1] == 3) && grid[y][x] == 1) {
                    grid[y][x] = 3;
                }

                if ((grid[y+1][x] == 3 || grid[y-1][x] == 3 || grid[y][x+1] == 3 || grid[y][x-1] == 3) && grid[y][x] == 2) {
                    grid[y][x] = 0;
                }
            }
        }

        int currentFigure = GetRandomValue(0, 6);
        int nextFigure = GetRandomValue(0, 6);
        GenerateNextFigure(grid, currentFigure, nextFigure);
 
    }

    for (int y = 0; y < CELL_HIGH; y++) {
        for (int x = 0; x < CELL_WIDE; x++) {
            if ((grid[y][x] == 1 || grid[y][x] == 2) && (grid[y][x-1] == 3 || grid[y][x+1] == 3)) {
                return;
            }
        }
    }

    

    //printf("%i", can_rotate);

    for (int y = CELL_HIGH-1; y > 0; y--) {
        for (int x = CELL_WIDE-1; x > 0; x--) {
            if (IsKeyDown(KEY_RIGHT) && grid[y][x+1] == 0 && (grid[y][x] == 1 || grid[y][x] == 2) && x < CELL_WIDE-1) {
                int buffer = grid[y][x];
                grid[y][x] = 0;
                grid[y][x+1] = buffer;
            }
        }

        for (int x = 0; x < CELL_WIDE; x++) {
            if (IsKeyDown(KEY_LEFT) && grid[y][x-1] == 0 && (grid[y][x] == 1 || grid[y][x] == 2) && x > 1) {
                int buffer = grid[y][x];
                grid[y][x] = 0;
                grid[y][x-1] = buffer;
            }
        }
    }

}

void RenderGrid(int grid[CELL_HIGH][CELL_WIDE], Texture2D tile, Texture2D tile_grid) {
    Vector2 position;
    
    for (int y = 0; y < CELL_HIGH; y++) {
        for (int x = 0; x < CELL_WIDE; x++) {
            DrawTextureEx(tile_grid, position, 0, SCALE, WHITE);
            
            if (grid[y][x] == 1) {
                DrawTextureEx(tile, position, 0, SCALE, WHITE);
            }
            if (grid[y][x] == 3) {
                DrawTextureEx(tile, position, 0, SCALE, WHITE);
            }
            
            position.x = x * 8 * SCALE;
        }
        position.x = 0;
        position.y = y * 8 * SCALE;
    }

    
}

void RenderDebug(int grid[CELL_HIGH][CELL_WIDE]) {
    Vector2 position = {0};
    
    for (int y = 0; y < CELL_HIGH; y++) {
        for (int x = 0; x < CELL_WIDE; x++) {
            DrawText(std::to_string(grid[y][x]).c_str(), position.x, position.y, 12, YELLOW);
            position.x = x * 8 * SCALE;
        }
        position.x = 0;
        position.y = y * 8 * SCALE;
    }
}

int main() {
    
    InitWindow(WIDTH, HEIGHT, "Tetris Clone");
    
    Texture tile = LoadTexture("res/tile.png");
    Texture tile_grid = LoadTexture("res/tile_grid.png");

    InitAudioDevice();
    Sound sound = LoadSound("res/theme.ogg.mp3");
    PlaySound(sound);

    Timer speed;
    StartTimer(&speed, SPEED);

    int grid[CELL_HIGH][CELL_WIDE] = {0};
    
    int currentFigure = GetRandomValue(0, 6);
    int nextFigure = GetRandomValue(0, 6);
    GenerateNextFigure(grid, currentFigure, nextFigure);

    while (!WindowShouldClose()) {
        ClearBackground(BLACK);

        RenderGrid(grid, tile, tile_grid);
        //RenderDebug(grid); 

        EndDrawing();

        if (!IsSoundPlaying(sound)) PlaySound(sound);

        if (IsKeyPressed(KEY_DOWN)) {
            RotateClockwise(grid);
        } 
        if (IsKeyPressed(KEY_UP)) {
            RotateCounterclockwise(grid);
        }


        if (TimerDone(speed)) {

            UpdateGrid(grid);
            
            StartTimer(&speed, SPEED);
        }

        ClearCompleteLine(grid);
    }

    UnloadSound(sound);
    CloseWindow();
    return 0;
}
