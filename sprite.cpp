#include "sprite.h"
#include "settings.h"

//Constructor
//Single sprite, no transparency
Sprite::Sprite(SDL_Renderer* gameRenderer, std::string imagePath) 
  : path{imagePath}
{
  //Create local pointer to render object
  renderer = gameRenderer;

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
Sprite::Sprite(SDL_Renderer* gameRenderer, std::string imagePath, RGB color)
  : path{imagePath}, transparency{color}
{
  //Create local pointer to render object
  renderer = gameRenderer;

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
AnimatedSprite::AnimatedSprite(SDL_Renderer* gameRenderer, std::string imagePath, int numFrames)
  : MaxSpriteFrame{numFrames}
{
  //Copy the image path to the base class variable
  path = imagePath;

  //Create local pointer to render object
  renderer = gameRenderer;

  //Create the texture for the whole sheet
  tempSurface = loadImage();
  textureSheet = loadTexture(tempSurface);

  //Get the length and width of the sheet
  SDL_QueryTexture(textureSheet, NULL, NULL, &rectSheet.w, &rectSheet.h);

  //Get the length and width of a single sprite
  width = (rectSheet.w / MaxSpriteFrame);
  height = rectSheet.h;

  //Create sprite and placement rectangles
  FillRect(rect);
  FillRect(rectPlacement);
}

//Animated sprite, transparent
AnimatedSprite::AnimatedSprite(SDL_Renderer* gameRenderer, std::string imagePath, int numFrames, RGB color)
  : path{imagePath}, MaxSpriteFrame{numFrames}, transparency{color}
{
  //Create local pointer to render object
  renderer = gameRenderer;

  //Create the texture for the whole sheet
  tempSurface = loadImage();
  tempSurface = setTransparentColor(tempSurface);
  textureSheet = loadTexture(tempSurface);

  //Get the length and width of the sheet
  SDL_QueryTexture(textureSheet, NULL, NULL, &rectSheet.w, &rectSheet.h);

  //Get the length and width of a single sprite
  width = (rectSheet.w / MaxSpriteFrame);
  height = rectSheet.h;

  //Create sprite and placement rectangles
  FillRect(rect);
  FillRect(rectPlacement);
}


void Sprite::FillRect(SDL_Rect& rect) {
  rect.x = x;
  rect.y = y;
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

void Sprite::setLocation(int xPosition, int yPosition) {
  //Check for valid point
  if((xPosition < 0) || (xPosition > (Settings::SCREEN_WIDTH - width))) {
      std::cout << "Invalid move coordinate entered. Sprite not moved\n";
      return;
  }
  if((yPosition < 0) || (yPosition > (Settings::SCREEN_HEIGHT - height))) {
      std::cout << "Invalid move coordinate entered. Sprite not moved\n";
      return;
  }

  //Set sprite x and y variables to given coordinates
  x = xPosition;
  y = yPosition;
  
  //Update sprite rectangles for display
  if(animated) {
    //Move the sprite to the given coordinate
    rectPlacement.x = x;
    rectPlacement.y = y;
  }

  if(!animated) {
    rect.x = x;
    rect.y = y;
  }
}

void Sprite::NextFrame() {                //Advance sprite animation to the next frame
}

void Sprite::copyToRender(SDL_Renderer* renderer) {
  if(animated)
    SDL_RenderCopy(renderer, textureSheet, &rect, &rectPlacement);
  else
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}
