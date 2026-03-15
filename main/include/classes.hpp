#ifndef CLASSES_H
#define CLASSES_H

#include "SDL3/SDL_video.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_render.h"

#include <map>
#include <utility>


enum PixelType {
   SAND
  ,WATER
};


struct Vector2 {
  int x;
  int y;
  bool operator!=(Vector2 v) {
    if ((x != v.x) || (y != v.y)) {
      return true;
    }
    else {
      return false;
    }
  };
};


struct Pixel {
  SDL_Color color;
  PixelType type;
};

typedef std::map<std::pair<int, int>, Pixel> PixelMap;

// Took this as a reference https://www.geeksforgeeks.org/cpp/implementation-of-singleton-class-in-cpp/
/// @brief Singleton class of the world
class World {
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* texture;
  Vector2 window_size;
  PixelMap pixel_map; ///< Map with key being pixel's current [x,y] coordinates

  SDL_Color bg_color;

  static World* instance;

  World();

  /// @brief Checks if position on the screen is avalible for movement
  bool checkIfCanMove(Vector2 pos, PixelMap *new_pixel_map);
  
  /// @brief Recalculates position of each pixel in pixel_map according to their physic behavior
  void recalcWorld();

  /// @brief Adds new pixels to the pixel_map
  void addPixel();

  /// @brief Clears pixel_map
  void clearWorld();


public:
  bool mouse_is_down = false;
  PixelType selected_pixel_type;

  ~World();
  
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
};

#endif // CLASSES_H
