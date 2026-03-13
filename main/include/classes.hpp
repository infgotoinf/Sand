#ifndef CLASSES_H
#define CLASSES_H

#include <SDL3/SDL_video.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>

#include <map>


enum PixelType {
   SAND
  ,WATER
};


struct Vector2 {
  int x;
  int y;
};


struct Pixel {
  SDL_Color color;
  PixelType type;
};


// Took this as a reference https://www.geeksforgeeks.org/cpp/implementation-of-singleton-class-in-cpp/
/// @brief Singleton class of the world
class World {
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Color bg_color;
  std::map<Vector2, Pixel> pixel_map; ///< Map with key being pixel's current [x,y] coordinates

  static World* instance;

  World();

  /// @brief Recalculates position of each pixel in pixel_map according to their physic behavior
  void recalcWorld();

  /// @brief Clears pixel_map
  void clearWorld();


public:
  // Remove copy constructor
  World(const World& obj) = delete;

  static World* getIstance() {
    if (instance == nullptr) {
      instance = new World();
    }
    return instance;
  }

  SDL_AppResult initWorld();

  /// @brief Redraws pixels on the screen
  SDL_AppResult redrawWorld();

  /// @brief Adds new pixels to the pixel_map
  /// @iparams[in] pixel_type Type of created pixel
  void addPixel(PixelType pixel_type);
};

#endif // CLASSES_H
