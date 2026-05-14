#include "ai.h"
#include "game.h"
#include <stdlib.h>

void AI_Move(Board* playerBoard,
    AIState* aiState)
{
    bool hit = true;

    while (hit)
    {
        hit = false;

        int x;
        int y;

        if (aiState->hasTarget)
        {
            int directions[4][2] =
            {
                { 1, 0 },
                { -1, 0 },
                { 0, 1 },
                { 0, -1 }
            };

            bool found = false;

            for (int i = 0; i < 4; i++)
            {
                int nx =
                    aiState->targetX +
                    directions[i][0];

                int ny =
                    aiState->targetY +
                    directions[i][1];

                if (nx < 0 || nx >= playerBoard->size ||
                    ny < 0 || ny >= playerBoard->size)
                {
                    continue;
                }

                int cell =
                    playerBoard->cells[ny][nx];

                if (cell == HIT ||
                    cell == MISS)
                {
                    continue;
                }

                x = nx;
                y = ny;

                found = true;

                break;
            }

            if (!found)
            {
                aiState->hasTarget = false;
            }
        }

        if (!aiState->hasTarget)
        {
            while (true)
            {
                x = rand() % playerBoard->size;
                y = rand() % playerBoard->size;

                int cell =
                    playerBoard->cells[y][x];

                if (cell == HIT ||
                    cell == MISS)
                {
                    continue;
                }

                break;
            }
        }

        int* cell =
            &playerBoard->cells[y][x];

        if (*cell == SHIP)
        {
            *cell = HIT;

            hit = true;

            aiState->hasTarget = true;
            aiState->targetX = x;
            aiState->targetY = y;

            if (IsShipDestroyed(playerBoard,
                x,
                y))
            {
                MarkDestroyedShip(playerBoard,
                    x,
                    y);

                aiState->hasTarget = false;
            }
        }

        else if (*cell == EMPTY)
        {
            *cell = MISS;
        }
    }
}