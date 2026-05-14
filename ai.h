#pragma once

#include "board.h"

// ai.h
typedef struct
{
    bool hasTarget;
    int targetX;
    int targetY;

    // --- Режим направленного добивания (Hard) ---
    bool hunting;             // true = сейчас идём вдоль корабля
    int originX, originY;     // координаты первого попадания (начало поиска)
    int huntDirX, huntDirY;   // текущее направление движения (1,0 / -1,0 / 0,1 / 0,-1)
    int aiDifficulty;         // уровень сложности (1 = Easy, 2 = Hard)
} AIState;

void AI_Move(Board* playerBoard,
    AIState* aiState);