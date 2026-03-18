///////////////////////////////////////////////////////////////////////////////
/// Main file of the game, glues everything together.
///
/// @ref Uses classes and functions defined in include/classes.hpp
///
/// @file
/// @date 2026/03/15
/// @copyright Copyright (c) 2026 -inf (@infgotoinf) v. DemDanEm (@DemDanEm).
/// All rights reserved.\n
/// This file is under the MIT License (MIT)
///////////////////////////////////////////////////////////////////////////////
#include "SDL3/SDL_keycode.h"
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>

#include "include/classes.hpp"


/// Defines how many times per second game will be updated.
#define FPS 200

/// Defines how many miliseconds should be waited to update.
#define BETWEEN_FRAME_INTERVAL (1000 / FPS)


World* World::instance = nullptr;

static World* world = World::getIstance(); ///< World class instance


///////////////////////////////////////////////////////////////////////////////
/// This function runs once at startup. Calls World::initWorld().
///////////////////////////////////////////////////////////////////////////////
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    return world->initWorld();
}


///////////////////////////////////////////////////////////////////////////////
/// This function runs when a new event occurs.
///
/// - Handles switching World::mouse_is_down between true and false.
/// - Handles switching World::selected_pixel_type between SAND and WATER
/// PixelType.
/// - Hendles inwoking of World::clearWorld().
///////////////////////////////////////////////////////////////////////////////
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    // Check if mouse is pressed of no
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        world->mouse_is_down = true;
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        world->mouse_is_down = false;
        break;

    // Do some action if key on a keyboard is pressed
    case SDL_EVENT_KEY_DOWN:
        if (event->key.key == SDLK_1)
            world->selected_pixel_type = SAND;
        if (event->key.key == SDLK_2)
            world->selected_pixel_type = WATER;
        if (event->key.key == SDLK_0)
            world->clearWorld();
            
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}


///////////////////////////////////////////////////////////////////////////////
/// This function runs each frame. Calls World::redrawWorld() each time
/// #BETWEEN_FRAME_INTERVAL passes.
///////////////////////////////////////////////////////////////////////////////
SDL_AppResult SDL_AppIterate(void *appstate)
{
    static Uint64 now = 0;
    if (SDL_GetTicks() > now) {
        now += BETWEEN_FRAME_INTERVAL;
        return world->redrawWorld();
    }
    return SDL_APP_CONTINUE;
}


///////////////////////////////////////////////////////////////////////////////
/// This function runs on showdown.
///////////////////////////////////////////////////////////////////////////////
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}

///////////////////////////////////////////////////////////////////////////////
/// @mainpage Sand the sandbox game
/// @section links_sec Links
/// @subsection files_sec Files
/// @ref classes.cpp main.cpp classes.hpp
/// @subsection github_sec Github
/// https://github.com/infgotoinf/Sand \n
/// https://github.com/infgotoinf/Sand/wiki/О-проекте#основная-информация
///////////////////////////////////////////////////////////////////////////////
