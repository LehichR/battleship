#include "game.h"

bool PlayerShoot(Board* enemyBoard,
    int gridX,
    int gridY)
{
    if (enemyBoard->cells[gridY][gridX] == HIT ||
        enemyBoard->cells[gridY][gridX] == MISS)
    {
        return false;
    }

    if (enemyBoard->cells[gridY][gridX] == SHIP)
    {
        enemyBoard->cells[gridY][gridX] = HIT;

        if (IsShipDestroyed(enemyBoard,
            gridX,
            gridY))
        {
            MarkDestroyedShip(enemyBoard,
                gridX,
                gridY);
        }

        return true;
    }

    if (enemyBoard->cells[gridY][gridX] == EMPTY)
    {
        enemyBoard->cells[gridY][gridX] = MISS;

        return false;
    }

    return false;
}

bool AllShipsDestroyed(Board* board)
{
    for (int y = 0; y < board->size; y++)
    {
        for (int x = 0; x < board->size; x++)
        {
            if (board->cells[y][x] == SHIP)
            {
                return false;
            }
        }
    }

    return true;
}

bool IsShipDestroyed(Board* board,
    int x,
    int y)
{
    int directions[4][2] =
    {
        { 1, 0 },
        { -1, 0 },
        { 0, 1 },
        { 0, -1 }
    };

    for (int d = 0; d < 4; d++)
    {
        int dx = directions[d][0];
        int dy = directions[d][1];

        int nx = x;
        int ny = y;

        while (true)
        {
            nx += dx;
            ny += dy;

            if (nx < 0 || nx >= board->size ||
                ny < 0 || ny >= board->size)
            {
                break;
            }

            int cell = board->cells[ny][nx];

            if (cell == EMPTY || cell == MISS)
            {
                break;
            }

            if (cell == SHIP)
            {
                return false;
            }
        }
    }

    return true;
}

void MarkDestroyedShip(Board* board,
    int x,
    int y)
{
    int startX = x;
    int startY = y;

    int endX = x;
    int endY = y;

    // ищем начало по горизонтали
    while (startX > 0 &&
        board->cells[y][startX - 1] == HIT)
    {
        startX--;
    }

    // ищем конец по горизонтали
    while (endY < board->size - 1 &&
        board->cells[endY + 1][x] == HIT)
    {
        endY++;
    }

    // если корабль горизонтальный
    if (startX != endX)
    {
        for (int shipX = startX;
            shipX <= endX;
            shipX++)
        {
            for (int dy = -1; dy <= 1; dy++)
            {
                for (int dx = -1; dx <= 1; dx++)
                {
                    int nx = shipX + dx;
                    int ny = y + dy;

                    if (nx < 0 || nx >= board->size ||
                        ny < 0 || ny >= board->size)
                    {
                        continue;
                    }

                    if (board->cells[ny][nx] == EMPTY)
                    {
                        board->cells[ny][nx] = MISS;
                    }
                }
            }
        }

        return;
    }

    // ищем начало по вертикали
    while (startY > 0 &&
        board->cells[startY - 1][x] == HIT)
    {
        startY--;
    }

    // ищем конец по вертикали
    while (endX < board->size - 1 &&
        board->cells[endY + 1][x] == HIT)
    {
        endY++;
    }

    // вертикальный корабль
    if (startY != endY)
    {
        for (int shipY = startY;
            shipY <= endY;
            shipY++)
        {
            for (int dy = -1; dy <= 1; dy++)
            {
                for (int dx = -1; dx <= 1; dx++)
                {
                    int nx = x + dx;
                    int ny = shipY + dy;

                    if (nx < 0 || nx >= board->size ||
                        ny < 0 || ny >= board->size)
                    {
                        continue;
                    }

                    if (board->cells[ny][nx] == EMPTY)
                    {
                        board->cells[ny][nx] = MISS;
                    }
                }
            }
        }

        return;
    }

    // однопалубный корабль
    for (int dy = -1; dy <= 1; dy++)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            int nx = x + dx;
            int ny = y + dy;

            if (nx < 0 || nx >= board->size ||
                ny < 0 || ny >= board->size)
            {
                continue;
            }

            if (board->cells[ny][nx] == EMPTY)
            {
                board->cells[ny][nx] = MISS;
            }
        }
    }
}

void StartNewGame(Board* playerBoard,
    Board* enemyBoard,
    AIState* aiState,
    GameSettings* settings)
{
    // Защита: если размер поля некорректный – используем стандартный
    if (settings->boardSize <= 0)
    {
        settings->boardSize = 10;
        settings->cellSize = 40;
    }

    *playerBoard = (Board){ 0 };
    *enemyBoard = (Board){ 0 };

    playerBoard->size = settings->boardSize;
    enemyBoard->size = settings->boardSize;

    *aiState = (AIState){ 0 };

    GenerateShips(playerBoard, settings->shipCounts);
    GenerateShips(enemyBoard, settings->shipCounts);
}

int CountAliveShips(Board* board)
{
    int count = 0;

    for (int y = 0; y < board->size; y++)
    {
        for (int x = 0; x < board->size; x++)
        {
            if (board->cells[y][x] == SHIP)
            {
                count++;
            }
        }
    }

    return count;
}