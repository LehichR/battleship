#pragma once

typedef struct
{
    int boardSize;
    int cellSize;
    int aiDifficulty;
    int soundEnabled;
    int shipColorR;
    int shipColorG;
    int shipColorB;
    int shipCounts[4];   // [0] - 1-палубные, [1] - 2-палубные, [2] - 3-палубные, [3] - 4-палубные
    char playerName[32];
} GameSettings;

void SaveSettings(GameSettings* settings);

void LoadSettings(GameSettings* settings);