#include "board.h"

void DrawBoard(SDL_Renderer* renderer,
    Board* board,
    int startX,
    int startY,
    bool hideShips,
    GameSettings* settings)
{
    for (int y = 0; y < board->size; y++)
    {
        for (int x = 0; x < board->size; x++)
        {
            SDL_FRect rect =
            {
                startX + x * settings->cellSize,
                startY + y * settings->cellSize,
                settings->cellSize,
                settings->cellSize
            };

            //это корабли рисуем
            if (board->cells[y][x] == SHIP && !hideShips)
            {
                SDL_SetRenderDrawColor(renderer,
                    settings->shipColorR,
                    settings->shipColorG,
                    settings->shipColorB,
                    255);

                SDL_RenderFillRect(renderer, &rect);
            }

            //это клетки попадания
            if (board->cells[y][x] == HIT)
            {
                SDL_SetRenderDrawColor(renderer,
                    255,
                    0,
                    0,
                    255);

                SDL_RenderFillRect(renderer, &rect);
            }

            //это промах
            if (board->cells[y][x] == MISS)
            {
                SDL_SetRenderDrawColor(renderer,
                    220,
                    220,
                    220,
                    255);

                SDL_RenderFillRect(renderer, &rect);
            }

            //это сетка 
            SDL_SetRenderDrawColor(renderer,
                100,
                100,
                100,
                255);

            SDL_RenderRect(renderer, &rect);
        }
    }
}

bool CanPlaceShip(Board* board,
    int x,
    int y,
    int length,
    bool horizontal)
{
    for (int i = 0; i < length; i++)
    {
        int nx = x;
        int ny = y;

        if (horizontal)
        {
            nx += i;
        }
        else
        {
            ny += i;
        }

        if (nx >= board->size || ny >= board->size)
        {
            return false;
        }

        for (int dy = -1; dy <= 1; dy++)
        {
            for (int dx = -1; dx <= 1; dx++)
            {
                int checkX = nx + dx;
                int checkY = ny + dy;

                if (checkX < 0 ||
                    checkX >= board->size ||
                    checkY < 0 ||
                    checkY >= board->size)
                {
                    continue;
                }

                if (board->cells[checkY][checkX] == SHIP)
                {
                    return false;
                }
            }
        }
    }

    return true;
}

void PlaceShip(Board* board,
    int x,
    int y,
    int length,
    bool horizontal)
{
    for (int i = 0; i < length; i++)
    {
        int nx = x;
        int ny = y;

        if (horizontal)
        {
            nx += i;
        }
        else
        {
            ny += i;
        }

        board->cells[ny][nx] = SHIP;
    }
}

void GenerateShips(Board* board, int shipCounts[4])
{
    // shipCounts[0] - 1-палубные, ... [3] - 4-палубные
    for (int size = 4; size >= 1; size--)
    {
        int count = shipCounts[size - 1];
        for (int i = 0; i < count; i++)
        {
            int attempts = 0;
            while (attempts < 1000) // ограничение, чтобы не виснуть
            {
                int x = rand() % board->size;
                int y = rand() % board->size;
                bool horizontal = rand() % 2;

                if (CanPlaceShip(board, x, y, size, horizontal))
                {
                    PlaceShip(board, x, y, size, horizontal);
                    break;
                }
                attempts++;
            }
        }
    }
}