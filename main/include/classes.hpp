///////////////////////////////////////////////////////////////////////////////
/// Header with definition of all structs, enums and classes includind World.
///
/// @ref Used in main.cpp\n
/// Implemented in classes.cpp
///
/// @file
/// @date 2026/03/15
/// @copyright Copyright (c) 2026 -inf (@infgotoinf) v. DemDanEm (@DemDanEm).
/// All rights reserved.\n
/// This file is under the MIT License (MIT)
///////////////////////////////////////////////////////////////////////////////
#ifndef CLASSES_HPP
#define CLASSES_HPP

#include <SDL3/SDL_video.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <cstdint>


///////////////////////////////////////////////////////////////////////////////
/// Enum that represents pixel type as a physical object.
///////////////////////////////////////////////////////////////////////////////
enum PixelType : uint8_t
{
     SEG_FAULT
    ,VOID
    ,SAND
    ,WATER
    ,STONE
    ,LAVA
};


///////////////////////////////////////////////////////////////////////////////
/// Struct what represends objects position in 2D space.
/// @note [0, 0] coordinate is left top corner of the window.
///////////////////////////////////////////////////////////////////////////////
struct Vector2
{
    int x; ///< X coordinate.
    int y; ///< Y coordinate.

    ///////////////////////////////////////////////////////////////////////////
    /// Compares Vector2's
    ///
    /// @returns true - if x and y of both objects are not equal.
    /// @returns false - overwise.
    ///////////////////////////////////////////////////////////////////////////
    bool operator!=(Vector2 v)
    {
        if ((x != v.x) || (y != v.y)) {
            return true;
        }
        else {
            return false;
        }
    };
};


///////////////////////////////////////////////////////////////////////////////
/// Enum that represents pixel type as a physical object.
///////////////////////////////////////////////////////////////////////////////
struct Pixel
{
    SDL_Color color; ///< Color of the pixel.
    PixelType type; ///< PixelType.
    bool was_updated; ///< Bolean value for World::recalcWorld.
};


///////////////////////////////////////////////////////////////////////////////
/// Singleton class of the world
///
/// @note Took this as a reference
/// https://www.geeksforgeeks.org/cpp/implementation-of-singleton-class-in-cpp/
/// @author -inf (infgotoinf)
/// @date 2026/03/15
///////////////////////////////////////////////////////////////////////////////
class World
{
    static World *instance; ///< Poiner to the one existing instance of the world.

protected:
    SDL_Window *window; ///< SDL window.
    SDL_Renderer *renderer; ///< SDL renderer.
    TTF_TextEngine *text_renderer; ///< TTF engine what is tied to World::renderer to easly render text.
    SDL_Texture *texture; ///< SDL texture.
    TTF_Font *font; ///< Fonts that are used for the text on the screen.
    Pixel **pixel_matrix; ///< Dynamicaly sized Pixel matrix what contains all phical object pixels.
    Vector2 pixel_matrix_size; ///< Size of pixel_matrix
    Vector2 window_size; ///< Window width and height in pixels devided by PIXEL_SIZE.
    int pixel_size; ///< Defines how bigger pixels will be than screen pixels.

    ///////////////////////////////////////////////////////////////////////////
    /// Class constructor, here we define World::selected_pixel_type and
    /// window_size.
    ///////////////////////////////////////////////////////////////////////////
    World();

    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    void calcPixelMatrixSize();

    ///////////////////////////////////////////////////////////////////////////
    /// Checks if position on the screen is avalible for movement.
    ///
    /// Checks if pos out of window bounds, taken by pixel in World::pixel_map
    /// of pixel in new_pixel_map.
    ///
    /// @param[in] pos X,Y coordinates of the window that are being checked for
    /// movement.
    /// @param[in] new_pixel_map PixelMap what is being created.
    /// @returns true - if pos not avalible
    /// @returns false - overwise
    ///////////////////////////////////////////////////////////////////////////
    PixelType checkPixel(Vector2 pos);

    ///////////////////////////////////////////////////////////////////////////
    /// Recalculates position of each pixel in pixel_map according to their
    /// physic behavior.
    ///
    /// Creates new_pixel_map and for each pixel in World::pixel_map checks on
    /// what coordinates it can move with checkIfCanMove() and inserts them in
    /// new_pixel_map. And after that replaces World::pixel_map with
    /// new_pixel_map.
    ///
    /// @startuml
    /// title Calculation of pixel's new coordinate
    /// start
    /// floating note left
    ///     X and Y are pixel's current
    ///     coordinates in 2D space
    /// end note
    /// floating note right
    ///     I'M SO SORRY FOR CREATING YOU, MY SON
    ///     GOD FORBID ME FOR MY SINS
    /// end note
    /// if ([X, Y - 1]) then (can move)
    ///     :[X, Y - 1];
    /// else (can't move)
    ///     switch (PixelType)
    ///     case (SAND)
    ///         if ([X - 1, Y - 1] & [X - 1, Y] &
    /// [X + 1, Y - 1] & [X + 1, Y]) then (can move)
    ///             :rand([X - 1, Y]
    /// or [X + 1, Y]);
    ///
    ///         else (can't move)
    ///         if ([X - 1, Y - 1]
    /// & [X - 1, Y]) then (can move)
    ///             :[X - 1, Y];
    ///
    ///         else (can't move)
    ///         if ([X + 1, Y - 1]
    /// & [X + 1, Y]) then (can move)
    ///             :[X + 1, Y];
    ///
    ///         else (can't move)
    ///             :[X, Y];
    ///         endif
    ///         endif
    ///         endif
    ///
    ///     case (WATER)
    ///         if ([X - 1, Y - 1] &
    /// [X + 1, Y - 1]) then (can move)
    ///             :rand([X - 1, Y - 1]
    /// or [X + 1, Y - 1]);
    ///
    ///         else (can't move)
    ///         if ([X - 1, Y - 1]) then (can move)
    ///             :[X - 1, Y - 1];
    ///
    ///         else (can't move)
    ///         if ([X + 1, Y - 1]) then (can move)
    ///             :[X + 1, Y - 1];
    ///
    ///         else (can't move)
    ///             if ([X - 1, Y] &
    /// [X + 1, Y]) then (can move)
    ///                 :rand([X - 1, Y]
    /// or [X + 1, Y]);
    ///
    ///             else (can't move)
    ///             if ([X - 1, Y] then (can move)
    ///                 :[X - 1, Y];
    ///
    ///             else (can't move)
    ///             if ([X + 1, Y] then (can move)
    ///                 :[X + 1, Y];
    ///
    ///             else (can't move)
    ///                 :[X, Y];
    ///             endif
    ///             endif
    ///             endif
    ///         endif
    ///         endif
    ///         endif
    ///     endswitch
    /// @enduml
    ///
    /// @note Took this as a refference:
    /// - https://stackoverflow.com/questions/1500064/renaming-first-and-second-of-a-map-iterator
    /// - https://stackoverflow.com/questions/32590764/can-i-use-stdpair-but-rename-first-and-second-member-names
    /// @todo Make sand fall under water and water lift above the snad.
    ///////////////////////////////////////////////////////////////////////////
    void recalcWorld();

    ///////////////////////////////////////////////////////////////////////////
    /// Adds new pixel to pixel_map at current mouse position in a radius of
    /// BRUSH_SPREAD.
    ///
    /// @param[in] Vector2 of current mouse position reletive to window
    /// borders.
    ///////////////////////////////////////////////////////////////////////////
    void addPixel(Vector2 mouse_pos);

    ///////////////////////////////////////////////////////////////////////////
    /// Resizes pixel_matrix with all of it's content to an actual window_size.
    ///
    /// @param[in] Vector2 pixel_matrix of an old window_size.
    ///////////////////////////////////////////////////////////////////////////
    void resizePixelMatrix();

public:
    bool mouse_is_down = false; ///< Stores current state of the mouse button, used in redrawWorld() to detect then addPixel() should be inwoked.
    PixelType selected_pixel_type; ///< Currently selected PixelType, used in addPixel().

    ~World();

    /// Remove copy constructor.
    World(const World& obj) = delete;

    ///////////////////////////////////////////////////////////////////////////
    /// Function what returns the worlds instance. If it's nullptr inwokes
    /// World().
    ///
    /// @note This is how singleton works.
    /// @returns Pointer to World::instance.
    ///////////////////////////////////////////////////////////////////////////
    static World* getIstance() {
        if (instance == nullptr) {
            instance = new World();
        }
        return instance;
    }

    ///////////////////////////////////////////////////////////////////////////
    /// Complitely clears pixel_map.
    ///////////////////////////////////////////////////////////////////////////
    void clearWorld();

    ///////////////////////////////////////////////////////////////////////////
    /// Function what defines World::window, World::texture and World::font.
    ///
    /// @returns SDL_APP_CONTINUE on success
    /// @returns SDL_APP_FAILURE on error
    ///////////////////////////////////////////////////////////////////////////
    SDL_AppResult initSDL();

    ///////////////////////////////////////////////////////////////////////////
    /// Redraws pixels on the screen, calls recalcWorld() and addPixel().
    ///
    /// @startuml
    /// title Function flow
    /// if (window was resized?) then (yes)
    ///     :Update World::window_size and
    /// World::texture to match actual
    ///                 window size;
    /// else (no)
    /// endif
    /// :Call recalcWorld();
    /// if (mouse_is_down) then (true)
    ///     :Call addPixel();
    /// else (false)
    /// endif
    /// :Redraw pixels on World::texture and render;
    /// :Text render;
    /// @enduml
    ///
    /// @callgraph
    /// @returns SDL_APP_CONTINUE on success
    /// @returns SDL_APP_FAILURE on error
    /// @todo (Not sure if we really need this but) Make screen resizing change
    /// pixel coordinates in pixel_map, so they don't go off the screen.
    ///////////////////////////////////////////////////////////////////////////
    SDL_AppResult redrawWorld();
};

#endif // CLASSES_HPP
