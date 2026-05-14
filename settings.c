#include "settings.h"

#include <stdio.h>
#include <string.h>

void SaveSettings(GameSettings* settings)
{
    FILE* file =
        fopen("settings.txt", "w");

    if (!file)
    {
        return;
    }

    fprintf(file,
        "%d %d %d %d %d %d %d %d %d %d %d\n",
        settings->boardSize,
        settings->cellSize,
        settings->aiDifficulty,
        settings->soundEnabled,
        settings->shipColorR,
        settings->shipColorG,
        settings->shipColorB,
        settings->shipCounts[0],
        settings->shipCounts[1],
        settings->shipCounts[2],
        settings->shipCounts[3]);

    fprintf(file,
        "%s\n",
        settings->playerName);

    fclose(file);
}

void LoadSettings(GameSettings* settings)
{
    FILE* file = fopen("settings.txt", "r");
    if (!file) return;

    int tmpBoardSize, tmpCellSize, tmpAiDiff, tmpSound;
    int tmpR, tmpG, tmpB;
    int tmpCounts[4];
    char tmpName[32];

    int items = fscanf_s(file,
        "%d %d %d %d %d %d %d %d %d %d %d\n",
        &tmpBoardSize,
        &tmpCellSize,
        &tmpAiDiff,
        &tmpSound,
        &tmpR,
        &tmpG,
        &tmpB,
        &tmpCounts[0],
        &tmpCounts[1],
        &tmpCounts[2],
        &tmpCounts[3]);

    if (items == 11)
    {
        settings->boardSize = tmpBoardSize;
        settings->cellSize = tmpCellSize;
        settings->aiDifficulty = tmpAiDiff;
        settings->soundEnabled = tmpSound;
        settings->shipColorR = tmpR;
        settings->shipColorG = tmpG;
        settings->shipColorB = tmpB;
        for (int i = 0; i < 4; i++)
            settings->shipCounts[i] = tmpCounts[i];

        if (fgets(tmpName, sizeof(tmpName), file))
        {
            tmpName[strcspn(tmpName, "\n")] = '\0';
            strcpy_s(settings->playerName, sizeof(settings->playerName), tmpName);
        }
    }

    fclose(file);
}