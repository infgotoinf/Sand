///////////////////////////////////////////////////////////////////////////////
/// File with implementation of all World functions.
///
/// @ref Defined in include/classes.hpp
///
/// @file
/// @date 2026/03/15
/// @copyright Copyright (c) 2026 -inf (@infgotoinf) v. DemDanEm (@DemDanEm).
/// All rights reserved.\n
/// This file is under the MIT License (MIT)
///////////////////////////////////////////////////////////////////////////////
#include "include/classes.hpp"
#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_stdinc.h"

#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <algorithm>
#include <utility>


/// Defines pixel size in pixels.
/// Makes pixel that much bigger than a regular pixel on the screen.
#define PIXEL_SIZE 4

/// Defines how off the mouse cursor's coordinates pixels can spawn.
#define BRUSH_SPREAD 5

/// Defines how much pixels will me spawned each time World::addPixel() inwoked.
#define BRUSH_DENCITY (BRUSH_SPREAD * 3)

/// Defines with how off the default color pixel color can be.
#define COLOR_SPREAD 100

/// Background color of the screen.
#define BG_COLOR   (SDL_Color) { (Uint8) (50) \
                               , (Uint8) (50) \
                               , (Uint8) (50) \
                               , (Uint8) (255) }

#define SAND_COLOR (SDL_Color) { (Uint8) (255) \
                               , (Uint8) (255 - SDL_rand(COLOR_SPREAD)) \
                               , (Uint8) (0 + SDL_rand(COLOR_SPREAD)) \
                               , (Uint8) (255) }

#define WATER_COLOR (SDL_Color) { (Uint8) (0) \
                                , (Uint8) (0) \
                                , (Uint8) (255 - SDL_rand(COLOR_SPREAD)) \
                                , (Uint8) (255) }

#define STONE_COLOR (SDL_Color) { (Uint8) (50 + SDL_rand(COLOR_SPREAD / 2)) \
                                , (Uint8) (75 + SDL_rand(COLOR_SPREAD / 2)) \
                                , (Uint8) (100) \
                                , (Uint8) (255) }

/// Defines size of the text that is drawn on the screen.
#define TEXT_SIZE 8



World::World() {
    window_size = {640, 480};
    selected_pixel_type = SAND;
}


World::~World()
{
    for (int i = 0; i < pixel_matrix_size.y; ++i) {
        delete [] pixel_matrix[i];
    }
    delete [] pixel_matrix;
    SDL_DestroyTexture(texture);
    TTF_Quit();
}


void fillWithVoidPixels(Pixel **pixel_matrix, Vector2 pixel_matrix_size)
{
    Pixel void_pixel = { BG_COLOR, VOID, true };

    for (int y = 0; y < pixel_matrix_size.y; ++y)
    {
        pixel_matrix[y] = new Pixel[pixel_matrix_size.x] {void_pixel};
        for (int x = 0; x < pixel_matrix_size.x; ++x)
        {
            pixel_matrix[y][x] = void_pixel;
        }
    }
}

SDL_AppResult World::initWorld() {
    SDL_SetAppMetadata("Sand the sandbox", "1", NULL);

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Sandbox", window_size.x, window_size.y
                                    , SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING
                              , window_size.x, window_size.y);
    if (!texture) {
        SDL_Log("Couldn't create streaming texture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Font part
    if (!TTF_Init()) {
        SDL_Log("Couldn't initialize TTF: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    text_renderer = TTF_CreateRendererTextEngine(renderer);
    if (!text_renderer) {
        SDL_Log("Couldn't create renderer text engine: %s", SDL_GetError());
        TTF_Quit();
        return SDL_APP_FAILURE;
    }

    font = TTF_OpenFont("vendored/UnifontExMono.ttf", TEXT_SIZE * 2);
    if (!font) {
        SDL_Log("Couldn't load font: %s", SDL_GetError());
        TTF_Quit();
        return SDL_APP_FAILURE;
    }

    // Initialising pixel_matrix
    pixel_matrix_size = { window_size.x / PIXEL_SIZE + 1, window_size.y / PIXEL_SIZE + 1 };
    pixel_matrix = new Pixel*[pixel_matrix_size.y];
    Vector2 window_size_copy = { pixel_matrix_size.x
                               , pixel_matrix_size.y };
    fillWithVoidPixels(pixel_matrix, window_size_copy);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}


void World::addPixel(Vector2 mouse_pos) {
    SDL_Color color;
    switch (selected_pixel_type) {
    case SAND:
        color = SAND_COLOR;
        break;

    case WATER:
        color = WATER_COLOR;
        break;

    case STONE:
        color = STONE_COLOR;
        break;

    default:
        color = SAND_COLOR;
    }

    for (int i = 0; i < BRUSH_DENCITY; ++i)
    {
        Vector2 pixel_spawn_coord = { (int)mouse_pos.x / PIXEL_SIZE + (SDL_rand(BRUSH_SPREAD) * (SDL_rand(2) ? -1 : 1))
                                    , (int)mouse_pos.y / PIXEL_SIZE + (SDL_rand(BRUSH_SPREAD) * (SDL_rand(2) ? -1 : 1))};

        // Fix pixel_spawn_coord if it's off the screen
        if (pixel_spawn_coord.x > pixel_matrix_size.x - 1) pixel_spawn_coord.x = pixel_matrix_size.x - 1;
        if (pixel_spawn_coord.y > pixel_matrix_size.y - 1) pixel_spawn_coord.y = pixel_matrix_size.y - 1;
        if (pixel_spawn_coord.x < 0) pixel_spawn_coord.x = 0;
        if (pixel_spawn_coord.y < 0) pixel_spawn_coord.y = 0;


        // Override pixel logic
        switch (selected_pixel_type) {
        case SAND:
            if (pixel_matrix[pixel_spawn_coord.y][pixel_spawn_coord.x].type == VOID
             || pixel_matrix[pixel_spawn_coord.y][pixel_spawn_coord.x].type == WATER)
            {
                pixel_matrix[pixel_spawn_coord.y][pixel_spawn_coord.x] = { color, selected_pixel_type, false };
            }
            break;

        case WATER:
            if (pixel_matrix[pixel_spawn_coord.y][pixel_spawn_coord.x].type == VOID)
            {
                pixel_matrix[pixel_spawn_coord.y][pixel_spawn_coord.x] = { color, selected_pixel_type, false };
            }
            break;

        case STONE:
            if (pixel_matrix[pixel_spawn_coord.y][pixel_spawn_coord.x].type != STONE)
            {
                pixel_matrix[pixel_spawn_coord.y][pixel_spawn_coord.x] = { color, selected_pixel_type, false };
            }
            break;

        default:
            if (pixel_matrix[pixel_spawn_coord.y][pixel_spawn_coord.x].type == VOID)
            {
                pixel_matrix[pixel_spawn_coord.y][pixel_spawn_coord.x] = { color, selected_pixel_type, false };
            }
        }
    }
}


void World::resizePixelMatrix(Vector2 old_pixel_matrix_size)
{
    pixel_matrix_size = { window_size.x / PIXEL_SIZE + 1, window_size.y / PIXEL_SIZE + 1 };

    Pixel **new_pixel_matrix = new Pixel*[pixel_matrix_size.y];

    Vector2 common_pixel_matrix_size = { std::min(old_pixel_matrix_size.x, pixel_matrix_size.x)
                                       , std::min(old_pixel_matrix_size.y, pixel_matrix_size.y) };


    for (int i = 0; i < common_pixel_matrix_size.y; ++i)
    {
        new_pixel_matrix[i] = new Pixel[pixel_matrix_size.x];
    }

    fillWithVoidPixels(new_pixel_matrix, pixel_matrix_size);

    // Copy all pixels from pixel_matrix to new_pixel_matrix;
    for (int i = 0; i < common_pixel_matrix_size.y; ++i)
    {
        std::copy(pixel_matrix[i], pixel_matrix[i] + common_pixel_matrix_size.x, new_pixel_matrix[i]);
    }


    // Clear pixel_matrix
    for (int i = 0; i < old_pixel_matrix_size.y; ++i) {
        delete [] pixel_matrix[i];
    }
    delete [] pixel_matrix;

    pixel_matrix = std::move(new_pixel_matrix);
}


void World::clearWorld()
{
    pixel_matrix = new Pixel*[pixel_matrix_size.y];
    fillWithVoidPixels(pixel_matrix, pixel_matrix_size);
}


PixelType World::checkPixel(Vector2 pos)
{
    const bool is_out_of_bounds = pos.x > pixel_matrix_size.x - 1 || pos.x < 0
                               || pos.y > pixel_matrix_size.y - 1 || pos.y < 0;
    if (is_out_of_bounds)
        return SEG_FAULT;

    return pixel_matrix[pos.y][pos.x].type;
}


void World::recalcWorld()
{
    for (int y = 0, switch_y = true; y >= 0; y += (switch_y ? 2 : -2))
    {
        if (switch_y == true) {
            if (y > pixel_matrix_size.y - 1) {
                y -= y - (pixel_matrix_size.y - 1) + (pixel_matrix_size.y) % 2;
                switch_y = !switch_y;
            }
        }

        for (int x = 0, switch_x = true; x >= 0; x += (switch_x ? 2 : -2))
        {
            if (switch_x == true) {
                if (x > pixel_matrix_size.x - 1) {
                    x -= x - (pixel_matrix_size.x - 1) + (pixel_matrix_size.x) % 2;
                    switch_x = !switch_x;
                }
            }

            // switch_j = ! switch_j;
            if (pixel_matrix[y][x].was_updated)
                continue;

            if (pixel_matrix[y][x].type == VOID)
            {
                pixel_matrix[y][x].was_updated = true;
                continue;
            }

            pixel_matrix[y][x].was_updated = true;

            const PixelType down = checkPixel({ x, y + 1 });
            const PixelType down_left  = checkPixel({ x - 1, y + 1 });
            const PixelType down_right = checkPixel({ x + 1, y + 1 });
            const PixelType left  = checkPixel({ x - 1, y });
            const PixelType right = checkPixel({ x + 1, y });

            bool can_fall_down;
            bool can_fall_down_right;
            bool can_fall_down_left;
            bool can_fall_right;
            bool can_fall_left;

            switch(pixel_matrix[y][x].type) {
            case SAND:
                can_fall_down = (down == VOID || down == WATER);
                can_fall_down_right = (down_right == VOID || down_right == WATER);
                can_fall_down_left = (down_left == VOID || down_left == WATER);
                can_fall_right = (right == VOID || right == WATER);
                can_fall_left = (left == VOID || left == WATER);

                if (can_fall_down)
                    std::swap(pixel_matrix[y][x], pixel_matrix[y + 1][x]);

                else if (can_fall_down_left && can_fall_down_right && can_fall_left && can_fall_right)
                    std::swap(pixel_matrix[y][x], pixel_matrix[y][x + (SDL_rand(2) ? 1: -1)]);

                else if (can_fall_down_left && can_fall_left)
                    std::swap(pixel_matrix[y][x], pixel_matrix[y][x - 1]);

                else if (can_fall_down_right && can_fall_right)
                    std::swap(pixel_matrix[y][x], pixel_matrix[y][x + 1]);

                break;

            case WATER:
                can_fall_down = (down == VOID);
                can_fall_down_right = (down_right == VOID);
                can_fall_down_left = (down_left == VOID);
                can_fall_right = (right == VOID);
                can_fall_left = (left == VOID);

                if (can_fall_down)
                    std::swap(pixel_matrix[y][x], pixel_matrix[y + 1][x]);

                else if (can_fall_down_left && can_fall_down_right)
                    std::swap(pixel_matrix[y][x], pixel_matrix[y + 1][x + (SDL_rand(2) ? 1 : -1)]);

                else if (can_fall_down_left)
                    std::swap(pixel_matrix[y][x], pixel_matrix[y + 1][x - 1]);

                else if (can_fall_down_right)
                    std::swap(pixel_matrix[y][x], pixel_matrix[y + 1][x + 1]);

                else
                {
                    if (can_fall_left && can_fall_right)
                        std::swap(pixel_matrix[y][x], pixel_matrix[y][x + (SDL_rand(2) ? 1 : -1)]);

                    else if (can_fall_left)
                        std::swap(pixel_matrix[y][x], pixel_matrix[y][x - 1]);

                    else if (can_fall_right)
                        std::swap(pixel_matrix[y][x], pixel_matrix[y][x + 1]);
                }
                break;

            default:
                pixel_matrix[y][x].was_updated = true;
                continue;
            }
            pixel_matrix[y][x].was_updated = true;
        }
    }

    for (int y = 0; y < pixel_matrix_size.y; ++y)
    {
        for (int x = 0; x < pixel_matrix_size.x; ++x)
        {
            pixel_matrix[y][x].was_updated = false;
        }
    }
}


SDL_AppResult World::redrawWorld()
{
    // Check if window size has changed
    Vector2 current_window_size;
    if (!SDL_GetWindowSizeInPixels(window, &current_window_size.x, &current_window_size.y)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    if (window_size != current_window_size)
    {
        // Update pixel_matrix, window_size and texture to match actual window size
        window_size = current_window_size;
        resizePixelMatrix(pixel_matrix_size);
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING
                                  , window_size.x, window_size.y);
        if (!texture) {
            SDL_Log("Couldn't create streaming texture: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }
    }

    recalcWorld();

    if (mouse_is_down) {
        float x, y;
        SDL_GetMouseState(&x, &y);
        addPixel(Vector2 { (int) x, (int) y });
    }

    // Redraw pixel_map on the screen
    SDL_FRect dst_rect;
    SDL_Surface *surface = NULL;

    if (SDL_LockTextureToSurface(texture, NULL, &surface))
    {
        // Draw black bg
        SDL_FillSurfaceRect(surface, NULL, SDL_MapRGB(SDL_GetPixelFormatDetails(surface->format), NULL
                           , 0, 0, 0));

        // Draw all pixels from pixel_matrix
        SDL_Rect r;
        for (int y = 0; y < pixel_matrix_size.y; ++y)
        {
            for (int x = 0; x < pixel_matrix_size.x; ++x)
            {
                Pixel current_pixel = pixel_matrix[y][x];
                r.w = r.h = PIXEL_SIZE;
                r.x = x * PIXEL_SIZE;
                r.y = y * PIXEL_SIZE;
                SDL_FillSurfaceRect(surface, &r, SDL_MapRGB(SDL_GetPixelFormatDetails(surface->format), NULL
                                   , current_pixel.color.r, current_pixel.color.g, current_pixel.color.b));
            }
        }
        SDL_UnlockTexture(texture);
    }

    dst_rect.x = dst_rect.y = 0;
    dst_rect.w = window_size.x;
    dst_rect.h = window_size.y;
    SDL_RenderTexture(renderer, texture, NULL, &dst_rect);

    // Text rendering
    static const char* text =
R"(LMB - to start drawing pixels
1 - to select Sand
2 - to select Water
3 - to select Stone
0 - to clean the screen)";

    static TTF_Text* text_text = TTF_CreateText(text_renderer, font, text, 0);
    TTF_DrawRendererText(text_text, TEXT_SIZE, TEXT_SIZE);


    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}
