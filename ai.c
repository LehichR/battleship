#include "ai.h"
#include "game.h"
#include <stdlib.h>

void AI_Move(Board* playerBoard, AIState* aiState)
{
    bool hit = true;

    while (hit)
    {
        hit = false;
        int x, y;

        // --- EASY MODE (1) ---
        if (aiState->aiDifficulty <= 1)
        {
            if (aiState->hasTarget)
            {
                int directions[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };
                bool found = false;

                for (int i = 0; i < 4; i++)
                {
                    int nx = aiState->targetX + directions[i][0];
                    int ny = aiState->targetY + directions[i][1];

                    if (nx < 0 || nx >= playerBoard->size ||
                        ny < 0 || ny >= playerBoard->size)
                        continue;

                    int cell = playerBoard->cells[ny][nx];
                    if (cell == HIT || cell == MISS)
                        continue;

                    x = nx; y = ny;
                    found = true;
                    break;
                }

                if (!found)
                    aiState->hasTarget = false;
            }

            if (!aiState->hasTarget)
            {
                while (true)
                {
                    x = rand() % playerBoard->size;
                    y = rand() % playerBoard->size;
                    int cell = playerBoard->cells[y][x];
                    if (cell == HIT || cell == MISS)
                        continue;
                    break;
                }
            }
        }
        // --- HARD MODE (>=2) ---
        else
        {
            if (aiState->hunting)
            {
                // Продолжаем движение в текущем направлении
                int nx = aiState->targetX + aiState->huntDirX;
                int ny = aiState->targetY + aiState->huntDirY;

                if (nx >= 0 && nx < playerBoard->size &&
                    ny >= 0 && ny < playerBoard->size &&
                    playerBoard->cells[ny][nx] != HIT &&
                    playerBoard->cells[ny][nx] != MISS)
                {
                    x = nx;
                    y = ny;
                }
                else
                {
                    // Промах или край поля – разворачиваемся
                    aiState->huntDirX = -aiState->huntDirX;
                    aiState->huntDirY = -aiState->huntDirY;
                    // Начинаем с первой клетки после origin
                    nx = aiState->originX + aiState->huntDirX;
                    ny = aiState->originY + aiState->huntDirY;

                    if (nx >= 0 && nx < playerBoard->size &&
                        ny >= 0 && ny < playerBoard->size &&
                        playerBoard->cells[ny][nx] != HIT &&
                        playerBoard->cells[ny][nx] != MISS)
                    {
                        x = nx;
                        y = ny;
                        aiState->targetX = nx;
                        aiState->targetY = ny;
                    }
                    else
                    {
                        // Обе стороны недоступны – корабль должен быть уже потоплен
                        aiState->hunting = false;
                        aiState->hasTarget = false;
                        hit = false;
                        continue;
                    }
                }
            }
            else if (aiState->hasTarget)
            {
                // Первое попадание: выбираем направление для охоты
                int directions[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };
                bool found = false;

                for (int i = 0; i < 4; i++)
                {
                    int nx = aiState->targetX + directions[i][0];
                    int ny = aiState->targetY + directions[i][1];

                    if (nx < 0 || nx >= playerBoard->size ||
                        ny < 0 || ny >= playerBoard->size)
                        continue;

                    int cell = playerBoard->cells[ny][nx];
                    if (cell == HIT || cell == MISS)
                        continue;

                    x = nx; y = ny;
                    found = true;

                    // Начинаем охоту
                    aiState->hunting = true;
                    aiState->originX = aiState->targetX;
                    aiState->originY = aiState->targetY;
                    aiState->huntDirX = directions[i][0];
                    aiState->huntDirY = directions[i][1];
                    break;
                }

                if (!found)
                {
                    // Все соседние клетки уже прострелены, сбрасываем цель
                    aiState->hasTarget = false;
                    hit = false;
                    continue;
                }
            }
            else
            {
                // Нет цели – случайный выстрел
                while (true)
                {
                    x = rand() % playerBoard->size;
                    y = rand() % playerBoard->size;
                    int cell = playerBoard->cells[y][x];
                    if (cell == HIT || cell == MISS)
                        continue;
                    break;
                }
            }
        }

        // --- Общая обработка попадания/промаха ---
        int* cell = &playerBoard->cells[y][x];

        if (*cell == SHIP)
        {
            *cell = HIT;
            hit = true;

            if (aiState->aiDifficulty >= 2 && aiState->hunting)
            {
                // В режиме охоты обновляем текущую цель на новую клетку
                aiState->targetX = x;
                aiState->targetY = y;
                // Направление не меняем
            }
            else
            {
                // Easy mode или первый выстрел в Hard
                aiState->hasTarget = true;
                aiState->targetX = x;
                aiState->targetY = y;
            }

            if (IsShipDestroyed(playerBoard, x, y))
            {
                MarkDestroyedShip(playerBoard, x, y);
                aiState->hasTarget = false;
                aiState->hunting = false;  // на всякий случай
            }
        }
        else if (*cell == EMPTY)
        {
            *cell = MISS;

            if (aiState->aiDifficulty >= 2 && aiState->hunting)
            {
                // Промахнулись в режиме охоты – разворачиваем направление
                aiState->huntDirX = -aiState->huntDirX;
                aiState->huntDirY = -aiState->huntDirY;
                // Попробуем выстрелить в противоположную сторону от origin
                int nx = aiState->originX + aiState->huntDirX;
                int ny = aiState->originY + aiState->huntDirY;
                if (nx >= 0 && nx < playerBoard->size &&
                    ny >= 0 && ny < playerBoard->size &&
                    playerBoard->cells[ny][nx] != HIT &&
                    playerBoard->cells[ny][nx] != MISS)
                {
                    aiState->targetX = nx;
                    aiState->targetY = ny;
                }
                else
                {
                    // Обе стороны заблокированы – корабль уничтожен?
                    aiState->hunting = false;
                    aiState->hasTarget = false;
                }
            }
        }
    }
}