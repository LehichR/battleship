#include "ui.h"

void DrawText(SDL_Renderer* renderer,
    TTF_Font* font,
    const char* text,
    SDL_FRect button)
{
    SDL_Color color =
    {
        255,
        255,
        255,
        255
    };

    SDL_Surface* surface =
        TTF_RenderText_Blended(
            font,
            text,
            0,
            color);

    if (!surface)
    {
        SDL_Log("Text render error: %s",
            SDL_GetError());

        return;
    }

    SDL_Texture* texture =
        SDL_CreateTextureFromSurface(
            renderer,
            surface);

    if (!texture)
    {
        SDL_DestroySurface(surface);

        SDL_Log("Texture error: %s",
            SDL_GetError());

        return;
    }

    float textX =
        button.x + (button.w - surface->w) / 2;

    float textY =
        button.y + (button.h - surface->h) / 2;

    SDL_FRect rect =
    {
        textX,
        textY,
        (float)surface->w,
        (float)surface->h
    };

    SDL_RenderTexture(renderer,
        texture,
        NULL,
        &rect);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}