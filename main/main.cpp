#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_stdinc.h"
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "include/classes.hpp"

#define FPS 120
#define BETWEEN_FRAME_INTERVAL (1000 / FPS)

World* World::instance = nullptr;

/// @brief World instance
static World* world = World::getIstance();


/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    return world->initWorld();
}


/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
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
            
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}


/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    static Uint64 now = 0;
    if (SDL_GetTicks() > now) {
        now += BETWEEN_FRAME_INTERVAL;
        return world->redrawWorld();
    }
    return SDL_APP_CONTINUE;
}


/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}
