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
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <utility>


/// Defines pixel size in pixels.
/// Makes pixel that much bigger than a regular pixel on the screen.
#define PIXEL_SIZE 5

/// Defines how off the mouse cursor's coordinates pixels can spawn.
#define BRUSH_SPREAD 3

/// Defines with how off the default color pixel color can be.
#define COLOR_SPREAD 100

#define SAND_COLOR (SDL_Color) { (Uint8) (255) \
                               , (Uint8) (255 - SDL_rand(COLOR_SPREAD)) \
                               , (Uint8) (0 + SDL_rand(COLOR_SPREAD)), 255 }

#define WATER_COLOR (SDL_Color) { (Uint8) (0) \
                                , (Uint8) (0) \
                                , (Uint8) (255 - SDL_rand(COLOR_SPREAD)), 255 }

/// Defines size of the text that is drawn on the screen.
#define TEXT_SIZE 8



World::World() {
    bg_color = {0, 0, 0, 255};
    selected_pixel_type = SAND;
}

World::~World() {
    SDL_DestroyTexture(texture);
    TTF_Quit();
}


void World::clearWorld() {
    pixel_map.clear();
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

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}


void World::addPixel() {
    float mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    Vector2 mouse_pos = { (int)mouse_x / PIXEL_SIZE + (SDL_rand(BRUSH_SPREAD) * (SDL_rand(2) ? -1 : 1))
                        , (int)mouse_y / PIXEL_SIZE + (SDL_rand(BRUSH_SPREAD) * (SDL_rand(2) ? -1 : 1))};

    // Fix pixel pos if it's off the screen
    if (mouse_pos.x > window_size.x) mouse_pos.x = window_size.x;
    if (mouse_pos.y > window_size.y) mouse_pos.y = window_size.y;
    if (mouse_pos.x < 0) mouse_pos.x = 0;
    if (mouse_pos.y < 0) mouse_pos.y = 0;

    // Add the pixel to pixel_map if there is no pixel with such coordinats
    if (!pixel_map.contains({ mouse_pos.x, mouse_pos.y }))
    {
        SDL_Color color = selected_pixel_type == SAND ? SAND_COLOR : WATER_COLOR;
        pixel_map.insert({ { mouse_pos.x, mouse_pos.y }, Pixel{ color, selected_pixel_type } });
    }
}



bool World::checkIfCanMove(Vector2 pos, PixelMap *new_pixel_map)
{
    const bool is_out_of_bounds = pos.x > window_size.x / PIXEL_SIZE || pos.x < 0
                               || pos.y > window_size.y / PIXEL_SIZE || pos.y < 0;
    const bool collides_with_other_pixel       = pixel_map.contains({ pos.x, pos.y });
    const bool collides_with_other_moved_pixel = new_pixel_map->contains({ pos.x, pos.y });
    return !(is_out_of_bounds || collides_with_other_pixel || collides_with_other_moved_pixel);
}


void World::recalcWorld() {
    PixelMap new_pixel_map;

    // https://stackoverflow.com/questions/1500064/renaming-first-and-second-of-a-map-iterator
    for (auto & [pos, pixel] : pixel_map)
    {
        // https://stackoverflow.com/questions/32590764/can-i-use-stdpair-but-rename-first-and-second-member-names
        auto & [pos_x, pos_y] = pos;

        const bool can_fall_down = checkIfCanMove({ pos_x, pos_y + 1 }, &new_pixel_map); 
        if (can_fall_down) {
            new_pixel_map.insert({ { pos_x, pos_y + 1 }, pixel });
        }
        else {
            const bool can_fall_down_left  = checkIfCanMove({ pos_x - 1, pos_y + 1 }, &new_pixel_map);
            const bool can_fall_down_right = checkIfCanMove({ pos_x + 1, pos_y + 1 }, &new_pixel_map);

            const bool can_fall_left  = checkIfCanMove({ pos_x - 1, pos_y }, &new_pixel_map);
            const bool can_fall_right = checkIfCanMove({ pos_x + 1, pos_y }, &new_pixel_map);

            switch(pixel.type) {
            case SAND:
                if (can_fall_down_left && can_fall_down_right && can_fall_left && can_fall_right)
                    new_pixel_map.insert({ { pos_x + (SDL_rand(2) ? 1 : -1), pos_y }, pixel });

                else if (can_fall_down_left && can_fall_left)
                    new_pixel_map.insert({ { pos_x - 1, pos_y }, pixel });

                else if (can_fall_down_right && can_fall_right)
                    new_pixel_map.insert({ { pos_x + 1, pos_y }, pixel });

                else
                    new_pixel_map.insert({ { pos_x, pos_y }, pixel });
                break;

            case WATER:
                if (can_fall_down_left && can_fall_down_right)
                    new_pixel_map.insert({ { pos_x + (SDL_rand(2) ? 1 : -1), pos_y + 1}, pixel });

                else if (can_fall_down_left)
                    new_pixel_map.insert({ { pos_x - 1, pos_y + 1}, pixel });

                else if (can_fall_down_right)
                    new_pixel_map.insert({ { pos_x + 1, pos_y + 1}, pixel });

                else
                {
                    if (can_fall_left && can_fall_right)
                        new_pixel_map.insert({ { pos_x + (SDL_rand(2) ? 1 : -1), pos_y }, pixel });

                    else if (can_fall_left)
                        new_pixel_map.insert({ { pos_x - 1, pos_y }, pixel });

                    else if (can_fall_right)
                        new_pixel_map.insert({ { pos_x + 1, pos_y }, pixel });

                    else
                        new_pixel_map.insert({ { pos_x, pos_y }, pixel });
                }
            }
        }
    }
    pixel_map = new_pixel_map;
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
        // Update window_size and texture to match actual window size
        window_size = current_window_size;
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

        // Draw all pixels from pixel_map
        SDL_Rect r;
        for (auto & [ pos, pixel ] : pixel_map)
        {
            auto & [pos_x, pos_y] = pos;
            r.w = r.h = PIXEL_SIZE;
            r.x = pos_x * PIXEL_SIZE;
            r.y = pos_y * PIXEL_SIZE;
            SDL_FillSurfaceRect(surface, &r, SDL_MapRGB(SDL_GetPixelFormatDetails(surface->format), NULL
                               , pixel.color.r, pixel.color.g, pixel.color.b));
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
