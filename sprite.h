#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <iostream>

//Coordinates
//Helps us hold and pass x,y coordinate values
struct Coordinate {
  int x, y;
};

//RGB Colors
//Helps us hold and pass RGB color objects
struct RGB {    //Defaults to white
  Uint8 R = 255;
  Uint8 G = 255;
  Uint8 B = 255;
};

//Sprites
/****************** CREATE A POINTER TO THE RENDER OBJECT *********************/
class Sprite {
protected:
  SDL_Texture* texture = NULL;  //Texture object
  SDL_Rect rect;                //Rectangle object for one sprite
  static SDL_Surface* tempSurface;      //Temporary surface for creating textured objects
  std::string path;             //Path to bitmap file
  //Transparency variables
  const RGB transparency;       //Transparency color
public:
  int width;                    //Sprite height
  int height;                   //Sprite width
  //Movement variables
  Coordinate position {0, 0};    //X and Y-position
  int xDir = 0;                 //X-direction, 1 = right, -1 = left, 0 = none
  int yDir = 0;                 //Y-direction, 1 = down, -1 = up, 0 = none
  double speed = 0;             //How many pixels to move sprite by
protected:  
  void FillRect(SDL_Rect& rect);
  SDL_Surface* loadImage();
  SDL_Texture* loadTexture(SDL_Surface* tempSurface);
  SDL_Surface* setTransparentColor(SDL_Surface* tempSurface);
  
public:
  //Function prototypes
  bool setLocation(Coordinate coordinate);       //Move the sprite to the given coordinates (check if in-bounds)
  void copyToRender();          //Copy the sprite to the render to be presented to the screen
  void move();

public:
  //Constructors
  Sprite();
  Sprite(std::string imagePath); 
  Sprite(std::string imagePath, RGB color);
  //Destructor - Clean up textures when object goes out of scope
  ~Sprite() {
    SDL_DestroyTexture(texture);
  }
};

class AnimatedSprite : public Sprite {
private:
  SDL_Texture* textureSheet = NULL; //Texture for entire animation sheet
  SDL_Rect rectPlacement;   //Rectangle for a single frame
  SDL_Rect rectSheet;       //Rectangle for entire sprite sheet
  int currentFrame = 0;      
  int frameCounter = 0;
  const int MAX_SPRITE_FRAME = 1; //Total number of frames
  const int FRAME_DELAY = 2;
public:
  void NextFrame();                 //Advance sprite animation to the next frame
  AnimatedSprite(std::string imagePath, int numFrames);
  AnimatedSprite(std::string imagePath, int numFrames, RGB color);
};

#endif
