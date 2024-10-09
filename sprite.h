#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <iostream>

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
public:
  SDL_Renderer* renderer = NULL; //Pointer to a render object, set to &renderer in constructor
  SDL_Texture* texture = NULL;  //Texture object
  SDL_Rect rect;                //Rectangle object for one sprite
  static SDL_Surface* tempSurface;      //Temporary surface for creating textured objects
  int width;                    //Sprite height
  int height;                   //Sprite width
  std::string path;       //Path to bitmap file
  //Transparency variables
  const RGB transparency;       //Transparency color
  //Movement variables
  int x = 0;                    //X-position
  int y = 0;                    //Y-position
  int xDir = 0;                 //X-direction, 1 = right, -1 = left, 0 = none
  int yDir = 0;                 //Y-direction, 1 = down, -1 = up, 0 = none
  double speed = 0;             //How many pixels to move sprite by

public:
  //Function prototypes
  void FillRect(SDL_Rect& rect);
  SDL_Surface* loadImage();
  SDL_Texture* loadTexture(SDL_Surface* tempSurface);
  SDL_Surface* setTransparentColor(SDL_Surface* tempSurface);
  void setLocation(int xPosition, int yPosition);       //Move the sprite to the given coordinates (check if in-bounds)
  void copyToRender(SDL_Renderer* renderer);          //Copy the sprite to the render to be presented to the screen

public:
  //Constructors
  Sprite(SDL_Renderer* gameRenderer, std::string imagePath); 
  Sprite(SDL_Renderer* gameRenderer, std::string imagePath, RGB color);
  //Destructor - Clean up textures when object goes out of scope
  ~Sprite() {
    renderer = NULL;
    SDL_DestroyTexture(texture);
  }
};

class AnimatedSprite : public Sprite {
public:
  SDL_Texture* textureSheet = NULL; //Texture for entire animation sheet
  SDL_Rect rectPlacement;   //Where to place the sprite on the screen
  SDL_Rect rectSheet;       //Rectangle for entire sprite sheet
  int spriteFrame = 0;      
  int frameCounter = 0;
  const int MaxSpriteFrame = 1; //Total number of frames
  const int FrameDelay = 2;
  void NextFrame();                 //Advance sprite animation to the next frame
  AnimatedSprite(SDL_Renderer* gameRenderer, std::string imagePath, int numFrames);
  AnimatedSprite(SDL_Renderer* gameRenderer, std::string imagePath, int numFrames, RGB color);
};

#endif
