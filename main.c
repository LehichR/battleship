#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "board.h"
#include "ai.h"
#include "game.h"
#include "settings.h"
#include "ui.h"
#include "results.h"

typedef enum
{
    STATE_MENU,
    STATE_GAME,
    STATE_SETTINGS,
    STATE_RESULTS,
    STATE_PLAYER_WIN,
    STATE_AI_WIN

} GameState;

int main(int argc, char* argv[])
{
    srand(time(NULL));

    SDL_Init(SDL_INIT_VIDEO);

    TTF_Init();

    SDL_Window* window =
        SDL_CreateWindow(
            "Battleship",
            1280,
            720,
            0
        );

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, NULL);

    TTF_Font* font =
        TTF_OpenFont("Roboto-SemiBold.ttf",
            36);

    SDL_StartTextInput(window);

    if (!font)
    {
        SDL_Log("Font error: %s", SDL_GetError());
    }

    bool running = true;

    GameState gameState = STATE_MENU;

    Uint64 gameEndTime = 0;

    AIState aiState = { 0 };

    //конфиги игры
    GameSettings settings =
    {
        10,            // boardSize
        40,            // cellSize
        2,             // aiDifficulty
        1,             // soundEnabled
        0, 200, 0,     // shipColor
        {4, 3, 2, 1},  // shipCounts: 4 однопалубных, 3 двухпалубных, 2 трёхпалубных, 1 четырёхпалубный
        "Player"
    };

    bool typingName = false;

    char results[20][64];
    int resultsCount = 0;

    LoadSettings(&settings);

    //тут короче создается игровое поле
    Board playerBoard = { 0 };
    Board enemyBoard = { 0 };

    //генерация кораблей
    StartNewGame(&playerBoard,
        &enemyBoard,
        &aiState,
        &settings);

    //кнопки меню
    SDL_FRect startButton =
    {
        490,
        180,
        300,
        80
    };

    //кнопка настроки
    SDL_FRect settingsButton =
    {
        490,
        300,
        300,
        80
    };

    //кнопка выхода
    SDL_FRect exitButton =
    {
        490,
        420,
        300,
        80
    };

    //кнопка результатов
    SDL_FRect resultsButton =
    {
        490,
        540,
        300,
        80
    };

    //кнопка выхода в меню
    SDL_FRect backButton =
    {
        40,
        40,
        180,
        60
    };

    //кнопки настроек
    SDL_FRect redButton =
    {
        420,
        200,
        120,
        70
    };

    SDL_FRect greenButton =
    {
        580,
        200,
        120,
        70
    };

    SDL_FRect blueButton =
    {
        740,
        200,
        120,
        70
    };

    //ввод имени
    SDL_FRect nameBox =
    {
        440,
        340,
        400,
        70
    };

    //выход в меню
    SDL_FRect menuButton =
    {
        20,
        20,
        160,
        55
    };

    //очистка результатов
    SDL_FRect clearButton =
    {
        1060,
        40,
        180,
        60
    };

    // --- Настройки количества кораблей ---
    // Кнопки "-" и "+" для каждого типа, плюс текст между ними
    SDL_FRect ship1DecBtn = { 400, 420, 40, 40 };
    SDL_FRect ship1IncBtn = { 500, 420, 40, 40 };
    // Текст «1‑deck» будем рисовать над кнопками (или слева, как удобнее), а число — между кнопками

    SDL_FRect ship2DecBtn = { 400, 480, 40, 40 };
    SDL_FRect ship2IncBtn = { 500, 480, 40, 40 };

    SDL_FRect ship3DecBtn = { 400, 540, 40, 40 };
    SDL_FRect ship3IncBtn = { 500, 540, 40, 40 };

    SDL_FRect ship4DecBtn = { 400, 600, 40, 40 };
    SDL_FRect ship4IncBtn = { 500, 600, 40, 40 };

    // --- Кнопки сложности AI (сдвинуты ниже) ---
    SDL_FRect diffEasyBtn = { 400, 670, 120, 50 };
    SDL_FRect diffHardBtn = { 560, 670, 120, 50 };

    // --- Кнопка сброса на дефолтные настройки ---
    SDL_FRect resetBtn = { 960, 40, 280, 60 };   

    //тут оно рисуется
    while (running)
    {
        SDL_Event event;

        //обработка нажатий
        while (SDL_PollEvent(&event))
        {
            if (gameState == STATE_MENU)
            {
                if (event.type ==
                    SDL_EVENT_MOUSE_BUTTON_DOWN)
                {
                    float mx = event.button.x;
                    float my = event.button.y;

                    SDL_FPoint point =
                    {
                        mx,
                        my
                    };

                    if (SDL_PointInRectFloat(
                        &point,
                        &startButton))
                    {
                        gameState = STATE_GAME;

                        continue;
                    }

                    if (SDL_PointInRectFloat(
                        &point,
                        &settingsButton))
                    {
                        gameState = STATE_SETTINGS;

                        continue;
                    }

                    if (SDL_PointInRectFloat(
                        &point,
                        &exitButton))
                    {
                        running = false;
                    }

                    if (SDL_PointInRectFloat(
                        &point,
                        &resultsButton))
                    {
                        resultsCount = LoadResults(results, 20);

                        gameState = STATE_RESULTS;

                        continue;
                    }
                }
            }

            if (gameState == STATE_SETTINGS)
            {
                if (event.type ==
                    SDL_EVENT_MOUSE_BUTTON_DOWN)
                {
                    float mx = event.button.x;
                    float my = event.button.y;

                    SDL_FPoint point =
                    {
                        mx,
                        my
                    };

                    if (SDL_PointInRectFloat(
                        &point,
                        &backButton))
                    {
                        gameState = STATE_MENU;

                        SaveSettings(&settings);
                    }

                    if (SDL_PointInRectFloat(
                        &point,
                        &redButton))
                    {
                        settings.shipColorR = 255;
                        settings.shipColorG = 0;
                        settings.shipColorB = 0;

                        SaveSettings(&settings);
                    }

                    if (SDL_PointInRectFloat(
                        &point,
                        &greenButton))
                    {
                        settings.shipColorR = 0;
                        settings.shipColorG = 255;
                        settings.shipColorB = 0;

                        SaveSettings(&settings);
                    }

                    if (SDL_PointInRectFloat(
                        &point,
                        &blueButton))
                    {
                        settings.shipColorR = 0;
                        settings.shipColorG = 120;
                        settings.shipColorB = 255;

                        SaveSettings(&settings);
                    }

                    // Обработка кнопок количества кораблей
                    if (SDL_PointInRectFloat(&point, &ship1DecBtn))
                    {
                        if (settings.shipCounts[0] > 0) settings.shipCounts[0]--;
                    }
                    if (SDL_PointInRectFloat(&point, &ship1IncBtn))
                    {
                        if (settings.shipCounts[0] < 9) settings.shipCounts[0]++;
                    }
                    // ... аналогично для ship2, ship3, ship4 ...
                    if (SDL_PointInRectFloat(&point, &ship2DecBtn))
                    {
                        if (settings.shipCounts[1] > 0) settings.shipCounts[1]--;
                    }
                    if (SDL_PointInRectFloat(&point, &ship2IncBtn))
                    {
                        if (settings.shipCounts[1] < 9) settings.shipCounts[1]++;
                    }
                    if (SDL_PointInRectFloat(&point, &ship3DecBtn))
                    {
                        if (settings.shipCounts[2] > 0) settings.shipCounts[2]--;
                    }
                    if (SDL_PointInRectFloat(&point, &ship3IncBtn))
                    {
                        if (settings.shipCounts[2] < 9) settings.shipCounts[2]++;
                    }
                    if (SDL_PointInRectFloat(&point, &ship4DecBtn))
                    {
                        if (settings.shipCounts[3] > 0) settings.shipCounts[3]--;
                    }
                    if (SDL_PointInRectFloat(&point, &ship4IncBtn))
                    {
                        if (settings.shipCounts[3] < 9) settings.shipCounts[3]++;
                    }

                    // Сложность AI
                    if (SDL_PointInRectFloat(&point, &diffEasyBtn))
                    {
                        settings.aiDifficulty = 1;
                    }
                    if (SDL_PointInRectFloat(&point, &diffHardBtn))
                    {
                        settings.aiDifficulty = 2;
                    }

                    // Сброс на дефолтные настройки
                    if (SDL_PointInRectFloat(&point, &resetBtn))
                    {
                        // Устанавливаем дефолтные значения
                        settings.boardSize = 10;
                        settings.cellSize = 40;
                        settings.aiDifficulty = 1;
                        settings.soundEnabled = 1;
                        settings.shipColorR = 0;
                        settings.shipColorG = 200;
                        settings.shipColorB = 0;
                        settings.shipCounts[0] = 4;
                        settings.shipCounts[1] = 3;
                        settings.shipCounts[2] = 2;
                        settings.shipCounts[3] = 1;
                        // Имя не сбрасываем, оставляем как есть
                    }

                    if (SDL_PointInRectFloat(
                        &point,
                        &nameBox))
                    {
                        typingName = true;
                    }
                    else
                    {
                        typingName = false;
                    }
                }
            }

            if (gameState == STATE_RESULTS)
            {
                if (event.type ==
                    SDL_EVENT_MOUSE_BUTTON_DOWN)
                {
                    float mx = event.button.x;
                    float my = event.button.y;

                    SDL_FPoint point =
                    {
                        mx,
                        my
                    };

                    if (SDL_PointInRectFloat(
                        &point,
                        &backButton))
                    {
                        gameState = STATE_MENU;
                    }

                    if (SDL_PointInRectFloat(
                        &point,
                        &clearButton))
                    {
                        ClearResults();

                        resultsCount =
                            LoadResults(results, 20);
                    }
                }
            }

            if (event.type == SDL_EVENT_TEXT_INPUT &&
                typingName)
            {
                if (strlen(settings.playerName) < 31)
                {
                    strcat_s(settings.playerName,
                        sizeof(settings.playerName),
                        event.text.text);
                }
            }

            if (event.type == SDL_EVENT_KEY_DOWN &&
                typingName)
            {
                if (event.key.key == SDLK_BACKSPACE)
                {
                    int len =
                        strlen(settings.playerName);

                    if (len > 0)
                    {
                        settings.playerName[len - 1] = '\0';
                    }
                }
            }

            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }

            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
                gameState == STATE_GAME)
            {
                float mx = event.button.x;
                float my = event.button.y;

                SDL_FPoint point =
                {
                    mx,
                    my
                };

                if (SDL_PointInRectFloat(
                    &point,
                    &menuButton))
                {
                    gameState = STATE_MENU;

                    StartNewGame(&playerBoard,
                        &enemyBoard,
                        &aiState,
                        &settings);

                    continue;
                }

                int mouseX = event.button.x;
                int mouseY = event.button.y;

                int gridX = (mouseX - 700) / settings.cellSize;
                int gridY = (mouseY - 100) / settings.cellSize;

                if (gridX >= 0 &&
                    gridX < settings.boardSize &&
                    gridY >= 0 &&
                    gridY < settings.boardSize)
                {
                    // Игнорируем повторные клики по подбитым/промахнутым клеткам
                    int cellState = enemyBoard.cells[gridY][gridX];
                    if (cellState == HIT || cellState == MISS)
                    {
                        continue;
                    }

                    bool hit = PlayerShoot(&enemyBoard, gridX, gridY);

                    if (AllShipsDestroyed(&enemyBoard))
                    {
                        SaveResult(settings.playerName, "WIN");
                        gameState = STATE_PLAYER_WIN;
                        gameEndTime = SDL_GetTicks();
                    }
                    else if (!hit)
                    {
                        AI_Move(&playerBoard, &aiState);
                    }

                    if (AllShipsDestroyed(&playerBoard))
                    {
                        SaveResult(settings.playerName, "LOSE");
                        gameState = STATE_AI_WIN;
                        gameEndTime = SDL_GetTicks();
                    }
                }
            }
        }

        if (gameState == STATE_MENU)
        {
            SDL_SetRenderDrawColor(renderer,
                40,
                40,
                70,
                255);

            SDL_RenderClear(renderer);

            // START
            SDL_SetRenderDrawColor(renderer,
                0,
                180,
                0,
                255);

            SDL_RenderFillRect(renderer,
                &startButton);

            DrawText(renderer,
                font,
                "START",
                startButton);

            // SETTINGS
            SDL_SetRenderDrawColor(renderer,
                180,
                180,
                0,
                255);

            SDL_RenderFillRect(renderer,
                &settingsButton);

            DrawText(renderer,
                font,
                "SETTINGS",
                settingsButton);

            // EXIT
            SDL_SetRenderDrawColor(renderer,
                180,
                0,
                0,
                255);

            SDL_RenderFillRect(renderer,
                &exitButton);

            DrawText(renderer,
                font,
                "EXIT",
                exitButton);

            //результаты
            SDL_SetRenderDrawColor(renderer,
                0,
                120,
                220,
                255);

            SDL_RenderFillRect(renderer,
                &resultsButton);

            DrawText(renderer,
                font,
                "RESULTS",
                resultsButton);
        }

        else if (gameState == STATE_GAME)
        {
            SDL_SetRenderDrawColor(renderer,
                30,
                30,
                40,
                255);

            SDL_RenderClear(renderer);

            DrawBoard(renderer,
                &playerBoard,
                100,
                100,
                false,
                &settings);

            DrawBoard(renderer,
                &enemyBoard,
                700,
                100,
                true,
                &settings);

            DrawText(renderer,
                font,
                settings.playerName,
                (SDL_FRect)
                {
                100, 60, 250, 50
                }
            );

            SDL_SetRenderDrawColor(renderer,
                120,
                120,
                120,
                255);

            SDL_RenderFillRect(renderer,
                &menuButton);

            DrawText(renderer,
                font,
                "MENU",
                menuButton);

            char shipsText[64];

            sprintf_s(shipsText,
                sizeof(shipsText),
                "Enemy HP: %d",
                CountAliveShips(&enemyBoard));

            DrawText(renderer,
                font,
                shipsText,
                (SDL_FRect)
            {
                700, 30, 300, 50
            });
        }

        else if (gameState == STATE_PLAYER_WIN)
        {
            SDL_SetRenderDrawColor(renderer,
                20,
                80,
                20,
                255);

            SDL_RenderClear(renderer);

            DrawBoard(renderer,
                &playerBoard,
                100,
                100,
                false,
                &settings);

            DrawBoard(renderer,
                &enemyBoard,
                700,
                100,
                false,
                &settings);
        }

        else if (gameState == STATE_AI_WIN)
        {
            SDL_SetRenderDrawColor(renderer,
                80,
                20,
                20,
                255);

            SDL_RenderClear(renderer);

            DrawBoard(renderer,
                &playerBoard,
                100,
                100,
                false,
                &settings);

            DrawBoard(renderer,
                &enemyBoard,
                700,
                100,
                false,
                &settings);
        }

        else if (gameState == STATE_SETTINGS)
        {
            SDL_SetRenderDrawColor(renderer,
                25,
                25,
                50,
                255);

            SDL_RenderClear(renderer);

            SDL_SetRenderDrawColor(renderer,
                120,
                120,
                120,
                255);

            SDL_RenderFillRect(renderer,
                &backButton);

            DrawText(renderer,
                font,
                "BACK",
                backButton);

            DrawText(renderer,
                font,
                "SETTINGS",
                (SDL_FRect) {
                440, 60, 400, 80 
                }
            );

            // RED
            SDL_SetRenderDrawColor(renderer,
                200,
                50,
                50,
                255);

            SDL_RenderFillRect(renderer,
                &redButton);

            DrawText(renderer,
                font,
                "RED",
                redButton);

            // GREEN
            SDL_SetRenderDrawColor(renderer,
                50,
                200,
                50,
                255);

            SDL_RenderFillRect(renderer,
                &greenButton);

            DrawText(renderer,
                font,
                "GREEN",
                greenButton);

            // BLUE
            SDL_SetRenderDrawColor(renderer,
                50,
                50,
                200,
                255);

            SDL_RenderFillRect(renderer,
                &blueButton);

            DrawText(renderer,
                font,
                "BLUE",
                blueButton);

            DrawText(renderer,
                font,
                "PLAYER NAME",
                (SDL_FRect) {
                440, 280, 400, 50
                }
            );

            SDL_SetRenderDrawColor(renderer,
                70,
                70,
                70,
                255);

            SDL_RenderFillRect(renderer,
                &nameBox);

            DrawText(renderer,
                font,
                settings.playerName,
                nameBox);

            // --- Отрисовка количества кораблей ---
            char ship1Label[] = "1-deck";
            DrawText(renderer, font, ship1Label, (SDL_FRect) { 300, 420, 90, 40 });

            // Число между кнопками
            char ship1Num[8];
            sprintf_s(ship1Num, sizeof(ship1Num), "%d", settings.shipCounts[0]);
            DrawText(renderer, font, ship1Num, (SDL_FRect) { 460, 420, 40, 40 });

            // Кнопки "-" и "+"
            SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
            SDL_RenderFillRect(renderer, &ship1DecBtn);
            DrawText(renderer, font, "-", ship1DecBtn);
            SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
            SDL_RenderFillRect(renderer, &ship1IncBtn);
            DrawText(renderer, font, "+", ship1IncBtn);

            // 2-deck
            DrawText(renderer, font, "2-deck", (SDL_FRect) { 300, 480, 90, 40 });
            char ship2Num[8];
            sprintf_s(ship2Num, sizeof(ship2Num), "%d", settings.shipCounts[1]);
            DrawText(renderer, font, ship2Num, (SDL_FRect) { 460, 480, 40, 40 });
            SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
            SDL_RenderFillRect(renderer, &ship2DecBtn);
            DrawText(renderer, font, "-", ship2DecBtn);
            SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
            SDL_RenderFillRect(renderer, &ship2IncBtn);
            DrawText(renderer, font, "+", ship2IncBtn);

            // 3-deck
            DrawText(renderer, font, "3-deck", (SDL_FRect) { 300, 540, 90, 40 });
            char ship3Num[8];
            sprintf_s(ship3Num, sizeof(ship3Num), "%d", settings.shipCounts[2]);
            DrawText(renderer, font, ship3Num, (SDL_FRect) { 460, 540, 40, 40 });
            SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
            SDL_RenderFillRect(renderer, &ship3DecBtn);
            DrawText(renderer, font, "-", ship3DecBtn);
            SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
            SDL_RenderFillRect(renderer, &ship3IncBtn);
            DrawText(renderer, font, "+", ship3IncBtn);

            // 4-deck
            DrawText(renderer, font, "4-deck", (SDL_FRect) { 300, 600, 90, 40 });
            char ship4Num[8];
            sprintf_s(ship4Num, sizeof(ship4Num), "%d", settings.shipCounts[3]);
            DrawText(renderer, font, ship4Num, (SDL_FRect) { 460, 600, 40, 40 });
            SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
            SDL_RenderFillRect(renderer, &ship4DecBtn);
            DrawText(renderer, font, "-", ship4DecBtn);
            SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
            SDL_RenderFillRect(renderer, &ship4IncBtn);
            DrawText(renderer, font, "+", ship4IncBtn);

            // --- Сложность AI ---
            DrawText(renderer, font, "AI Difficulty:", (SDL_FRect) { 200, 670, 200, 40 });

            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            SDL_RenderFillRect(renderer, &diffEasyBtn);
            DrawText(renderer, font, "Easy", diffEasyBtn);
            SDL_RenderFillRect(renderer, &diffHardBtn);
            DrawText(renderer, font, "Hard", diffHardBtn);

            // Подсветка активной сложности
            if (settings.aiDifficulty == 1)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_RenderRect(renderer, &diffEasyBtn);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_RenderRect(renderer, &diffHardBtn);
            }

            // --- Кнопка сброса ---
            SDL_SetRenderDrawColor(renderer, 180, 80, 80, 255);
            SDL_RenderFillRect(renderer, &resetBtn);
            DrawText(renderer, font, "Reset Defaults", resetBtn);
        }

        else if (gameState == STATE_RESULTS)
        {
            SDL_SetRenderDrawColor(renderer,
                20,
                20,
                20,
                255);

            SDL_RenderClear(renderer);

            DrawText(renderer,
                font,
                "RESULTS",
                (SDL_FRect)
                {
                440, 50, 400, 80
                }
                );

            SDL_SetRenderDrawColor(renderer,
                120,
                120,
                120,
                255);

            SDL_RenderFillRect(renderer,
                &backButton);

            DrawText(renderer,
                font,
                "BACK",
                backButton);

            SDL_SetRenderDrawColor(renderer,
                180,
                50,
                50,
                255);

            SDL_RenderFillRect(renderer,
                &clearButton);

            DrawText(renderer,
                font,
                "CLEAR",
                clearButton);

            for (int i = 0; i < resultsCount; i++)
            {
                DrawText(renderer,
                    font,
                    results[i],
                    (SDL_FRect)
                {
                    350,
                        160 + i * 50,
                        600,
                        40
                });
            }
        }

        if ((gameState == STATE_PLAYER_WIN ||
            gameState == STATE_AI_WIN) &&
            SDL_GetTicks() - gameEndTime > 3000)
        {
            StartNewGame(&playerBoard,
                &enemyBoard,
                &aiState,
                &settings);

            gameState = STATE_MENU;
        }

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();

    return 0;
}