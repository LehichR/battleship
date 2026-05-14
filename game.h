#pragma once

#include "board.h"
#include "ai.h"

bool PlayerShoot(Board* enemyBoard,
    int gridX,
    int gridY);

bool AllShipsDestroyed(Board* board);

bool IsShipDestroyed(Board* board,
    int x,
    int y);

void MarkDestroyedShip(Board* board,
    int x,
    int y);

void StartNewGame(Board* playerBoard,
    Board* enemyBoard,
    AIState* aiState,
    GameSettings* settings);

int CountAliveShips(Board* board);