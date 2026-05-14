#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>

#include "settings.h"

#define CELL_SIZE 40

//состояния ячеек
#define EMPTY 0
#define SHIP 1
#define HIT 2
#define MISS 3

//это структура поля
typedef struct
{
    int cells[20][20];

    int size;

} Board;

//расстановка кораблей
void DrawBoard(SDL_Renderer* renderer,
    Board* board,
    int startX,
    int startY,
    bool hideShips,
    GameSettings* settings);

bool CanPlaceShip(Board* board,
    int x,
    int y,
    int length,
    bool horizontal);

void PlaceShip(Board* board,
    int x,
    int y,
    int length,
    bool horizontal);

void GenerateShips(Board* board, int shipCounts[4]);