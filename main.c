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
        1,             // aiDifficulty
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
        350,
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