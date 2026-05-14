#pragma once

#include "board.h"

typedef struct
{
    bool hasTarget;

    int targetX;
    int targetY;

} AIState;

void AI_Move(Board* playerBoard,
    AIState* aiState);