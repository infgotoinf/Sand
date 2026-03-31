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

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <algorithm>


/// Defines pixel size in pixels.
/// Makes pixel that much bigger than a regular pixel on the screen.
#define PIXEL_SIZE 3

/// Defines how off the mouse cursor's coordinates pixels can spawn.
#define BRUSH_SPREAD 3

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

/// Defines size of the text that is drawn on the screen.
#define TEXT_SIZE 8



World::World() {
    selected_pixel_type = SAND;
}


World::~World()
{
    for (int i = 0; i < window_size.y / PIXEL_SIZE; ++i) {
        delete [] pixel_matrix[i];
    }
    delete [] pixel_matrix;
    SDL_DestroyTexture(texture);
    TTF_Quit();
}


void fillWithVoidPixels(Pixel **pixel_matrix, Vector2 window_size)
{
    Pixel void_pixel = { BG_COLOR, VOID, true };

    for (int y = 0; y < window_size.y; ++y) {
        pixel_matrix[y] = new Pixel[window_size.x] {void_pixel};
        // for (int x = 0; x < window_size.x; ++x) {
        //     pixel_matrix[y][x] = void_pixel;
        // }
    }
}

SDL_AppResult World::initWorld() {
    SDL_SetAppMetadata("Sand the sandbox", "1", NULL);

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    Vector2 initial_window_size = {640, 480};
    window_size = initial_window_size;
    if (!SDL_CreateWindowAndRenderer("Sandbox", initial_window_size.x, initial_window_size.y
                                    , SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING
                              , initial_window_size.x, initial_window_size.y);
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
    pixel_matrix = new Pixel*[window_size.y / PIXEL_SIZE];
    Vector2 window_size_copy = { window_size.x / PIXEL_SIZE
                               , window_size.y / PIXEL_SIZE };
    fillWithVoidPixels(pixel_matrix, window_size_copy);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}


void World::addPixel() {
    float mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    SDL_Color color = (selected_pixel_type == SAND ? SAND_COLOR : WATER_COLOR);

    for (int i = 0; i < BRUSH_DENCITY; ++i)
    {
        Vector2 mouse_pos = { (int)mouse_x / PIXEL_SIZE + (SDL_rand(BRUSH_SPREAD) * (SDL_rand(2) ? -1 : 1))
                            , (int)mouse_y / PIXEL_SIZE + (SDL_rand(BRUSH_SPREAD) * (SDL_rand(2) ? -1 : 1))};

        // Fix pixel pos if it's off the screen
        if (mouse_pos.x > window_size.x / PIXEL_SIZE) mouse_pos.x = window_size.x / PIXEL_SIZE;
        if (mouse_pos.y > window_size.y / PIXEL_SIZE) mouse_pos.y = window_size.y / PIXEL_SIZE;
        if (mouse_pos.x < 0) mouse_pos.x = 0;
        if (mouse_pos.y < 0) mouse_pos.y = 0;

        // Add the pixel to pixel_map if there is no pixel with such coordinats
        if (pixel_matrix[mouse_pos.y][mouse_pos.x].type == VOID)
        {
            pixel_matrix[mouse_pos.y][mouse_pos.x] = { color, selected_pixel_type, false };
        }
    }
}


void World::resizePixelMatrix(Vector2 old_window_size)
{
    old_window_size.x /= PIXEL_SIZE;
    old_window_size.y /= PIXEL_SIZE;
    Vector2 window_size_copy = { window_size.x / PIXEL_SIZE
                               , window_size.y / PIXEL_SIZE };

    Pixel **new_pixel_matrix = new Pixel*[window_size_copy.y];

    Vector2 common_window_size = { std::min(old_window_size.x, window_size_copy.x)
                                 , std::min(old_window_size.y, window_size_copy.y) };

    // Copy all pixels from pixel_matrix to new_pixel_matrix;
    for (int i = 0; i < common_window_size.y; ++i)
    {
        new_pixel_matrix[i] = new Pixel[window_size.x];
        std::copy(pixel_matrix[i], pixel_matrix[i] + common_window_size.x, new_pixel_matrix[i]);
    }

    // Clear pixel_matrix
    for (int i = 0; i < old_window_size.y; ++i) {
        delete [] pixel_matrix[i];
    }
    delete [] pixel_matrix;

    fillWithVoidPixels(new_pixel_matrix, window_size_copy);

    pixel_matrix = std::move(new_pixel_matrix);
}


void World::clearWorld()
{
    pixel_matrix = new Pixel*[window_size.y / PIXEL_SIZE];
    Vector2 window_size_copy = { window_size.x / PIXEL_SIZE
                               , window_size.y / PIXEL_SIZE };
    fillWithVoidPixels(pixel_matrix, window_size_copy);
}


bool World::checkIfCanMove(Vector2 pos)
{
    const bool is_out_of_bounds = pos.x > window_size.x / PIXEL_SIZE - 1 || pos.x < 0
                               || pos.y > window_size.y / PIXEL_SIZE - 1 || pos.y < 0;
    if (is_out_of_bounds)
        return false;

    if (pixel_matrix[pos.y][pos.x].type != VOID)
        return false;

    return true;
}


void World::recalcWorld()
{
    for (int y = 0; y < window_size.y / PIXEL_SIZE; ++y)
    {
        for (int x = 0; x < window_size.x / PIXEL_SIZE; ++x)
        {
            if (pixel_matrix[y][x].was_updated)
                continue;

            if (pixel_matrix[y][x].type == VOID)
            {
                pixel_matrix[y][x].was_updated = true;
                continue;
            }

            pixel_matrix[y][x].was_updated = true;

            const bool can_fall_down = checkIfCanMove({ x, y + 1 });
            if (can_fall_down) {
                std::swap(pixel_matrix[y][x], pixel_matrix[y + 1][x]);
            }
            else {
                const bool can_fall_down_left  = checkIfCanMove({ x - 1, y + 1 });
                const bool can_fall_down_right = checkIfCanMove({ x + 1, y + 1 });

                const bool can_fall_left  = checkIfCanMove({ x - 1, y });
                const bool can_fall_right = checkIfCanMove({ x + 1, y });

                switch(pixel_matrix[y][x].type) {
                case SAND:
                    if (can_fall_down_left && can_fall_down_right && can_fall_left && can_fall_right)
                        std::swap(pixel_matrix[y][x], pixel_matrix[y][x + (SDL_rand(2) ? 1 : -1)]);

                    else if (can_fall_down_left && can_fall_left)
                        std::swap(pixel_matrix[y][x], pixel_matrix[y][x - 1]);

                    else if (can_fall_down_right && can_fall_right)
                        std::swap(pixel_matrix[y][x], pixel_matrix[y][x + 1]);

                    break;

                case WATER:
                    if (can_fall_down_left && can_fall_down_right)
                        std::swap(pixel_matrix[y][x], pixel_matrix[y][x + (SDL_rand(2) ? 1 : -1)]);

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
                }
                pixel_matrix[y][x].was_updated = true;
            }
        }
    }

    for (int y = 0; y < window_size.y / PIXEL_SIZE; ++y)
    {
        for (int x = 0; x < window_size.x / PIXEL_SIZE; ++x)
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
        Vector2 old_window_size = window_size;
        window_size = current_window_size;
        resizePixelMatrix(old_window_size);
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING
                                  , window_size.x, window_size.y);
        if (!texture) {
            SDL_Log("Couldn't create streaming texture: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }
    }

    recalcWorld();

    if (mouse_is_down) {
        addPixel();
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
        for (int y = 0; y < window_size.y / PIXEL_SIZE; ++y)
        {
            for (int x = 0; x < window_size.x / PIXEL_SIZE; ++x)
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
1 - to select Sand"
2 - to select Water"
0 - to clean the screen)";

    static TTF_Text* text_text = TTF_CreateText(text_renderer, font, text, 0);
    TTF_DrawRendererText(text_text, TEXT_SIZE, TEXT_SIZE);


    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}
