#ifndef CONFIG_HPP
#define CONFIG_HPP

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
#define BG_COLOR (SDL_Color) { (Uint8) (50) \
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



/// Defines how many times per second game will be updated.
#define FPS 200

/// Defines how many miliseconds should be waited to update.
#define BETWEEN_FRAME_INTERVAL (1000 / FPS)

#endif // CONFIG_HPP
