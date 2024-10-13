#include "sprite.h"
#include "settings.h"
#include <SDL2/SDL_render.h>

extern SDL_Renderer* renderer;

//Constructor
//Setup - to be called by child class
Sprite::Sprite()
{
}
//Single sprite, no transparency
Sprite::Sprite(std::string imagePath) 
  : path{imagePath}
{
  //Create the texture
  tempSurface = loadImage();
  texture = loadTexture(tempSurface);

  //Get length and width of sprite
  SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
  width = rect.w;
  height = rect.h;

  //Create the rectangle to render on screen
  FillRect(rect);
}
//Single sprite, transparent
Sprite::Sprite(std::string imagePath, RGB color)
  : path{imagePath}, transparency{color}
{
  //Create the texture
  tempSurface = loadImage();
  tempSurface = setTransparentColor(tempSurface);
  texture = loadTexture(tempSurface);

  //Get length and width of sprite
  SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
  width = rect.w;
  height = rect.h;

  //Create the rectangle to render on screen
  FillRect(rect);
}

//Animated sprite, no transparency
AnimatedSprite::AnimatedSprite(std::string imagePath, int numFrames)
  : Sprite(), MAX_SPRITE_FRAME{numFrames}
{
  //Copy the image path to the base class variable
  path = imagePath;

  //Create the texture for the whole sheet
  tempSurface = loadImage();
  textureSheet = loadTexture(tempSurface);

  //Get the length and width of the sheet
  SDL_QueryTexture(textureSheet, NULL, NULL, &rectSheet.w, &rectSheet.h);

  //Get the length and width of a single sprite
  width = (rectSheet.w / MAX_SPRITE_FRAME);
  height = rectSheet.h;

  //Create sprite and placement rectangles
  FillRect(rect);
  FillRect(rectPlacement); //Maybe, maybe not
}

//Animated sprite, transparent
AnimatedSprite::AnimatedSprite(std::string imagePath, int numFrames, RGB color)
  : Sprite(), MAX_SPRITE_FRAME{numFrames}
{
  //Copy the image path to the base class variable
  path = imagePath;

  //Create the texture for the whole sheet
  tempSurface = loadImage();
  tempSurface = setTransparentColor(tempSurface);
  textureSheet = loadTexture(tempSurface);

  //Get the length and width of the sheet
  SDL_QueryTexture(textureSheet, NULL, NULL, &rectSheet.w, &rectSheet.h);

  //Get the length and width of a single sprite
  width = (rectSheet.w / MAX_SPRITE_FRAME);
  height = rectSheet.h;

  //Create sprite and placement rectangles
  FillRect(rect);
  FillRect(rectPlacement); //Maybe, maybe not
}


void Sprite::FillRect(SDL_Rect& rect) {
  rect.x = position.x;
  rect.y = position.y;
  rect.w = width;
  rect.h = height;
}

SDL_Surface* Sprite::loadImage() {
  tempSurface = SDL_LoadBMP(path.c_str());

  if(tempSurface == NULL)
    std::cout << "Unable to load image at path: " << path.c_str() << '\n';
  
  return tempSurface;
}

SDL_Texture* Sprite::loadTexture(SDL_Surface* tempSurface) {
  SDL_Texture* newTexture = NULL;

  newTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
  if(newTexture == NULL)
    std::cout << "Unable to create texture\n";

  SDL_FreeSurface(tempSurface);

  return newTexture;
}

SDL_Surface* Sprite::setTransparentColor(SDL_Surface* tempSurface) {
  Uint32 colorKey = SDL_MapRGB(tempSurface->format, transparency.R, transparency.G, transparency.B);
  
  SDL_SetColorKey(tempSurface, SDL_TRUE, colorKey);
  return tempSurface;
}

bool Sprite::setLocation(Coordinate coordinate) {
  //Check for valid point
  if((coordinate.x < 0) || (coordinate.x > (Settings::SCREEN_WIDTH - width))) {
      std::cout << "Invalid move coordinate entered. Sprite not moved\n";
      return false;
  }
  if((coordinate.y < 0) || (coordinate.y > (Settings::SCREEN_HEIGHT - height))) {
      std::cout << "Invalid move coordinate entered. Sprite not moved\n";
      return false;
  }

  //Set sprite x and y variables to given coordinates
  position.x = coordinate.x;
  position.y = coordinate.y;
  
  //Set the location for the rectangle
  rect.x = position.x;
  rect.y = position.y;

  return true;
}

void AnimatedSprite::NextFrame() {                //Advance sprite animation to the next frame
  frameCounter++;

  if(frameCounter > FRAME_DELAY) {
    frameCounter = 0;
    currentFrame++;
  }

  if(currentFrame > MAX_SPRITE_FRAME - 1)       //LAST FRAME
    currentFrame = 0;

  rect.x = currentFrame * rect.w;   //ADVANCE RECTANGLE TO NEXT FRAME ON SHEET
}

void Sprite::copyToRender() {
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}
