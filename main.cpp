#include <SDL2/SDL.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>

//Global Everything
//Window
SDL_Window* gameWindow = NULL;

//Renderer
SDL_Renderer* renderer;

//Surface
SDL_Surface* tempSurface;

//Game settings (global constants)
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int FPS = 30;
const int FRAME_DELAY = 1000/FPS;

//Global variables
int score = 0;
int lives = 3;

//Function Prototypes
bool ProgramIsRunning();
void CloseShop();

//RGB Colors
//Helps us hold and pass RGB color objects
struct RGB {
  Uint8 R = 255;
  Uint8 G = 255;
  Uint8 B = 255;
};

//Sprites
class Sprite {
private:
  bool animated;
public:
  SDL_Texture* texture = NULL;  //Texture object
  SDL_Rect rect;                //Rectangle object for one sprite
  int width;                    //Sprite height
  int height;                   //Sprite width
  const std::string path;
  //Transparency variables
  const RGB transparency;

  //Movement variables
  int x = 0;                    //X-position
  int y = 0;                    //Y-position
  int xDir = 0;                 //X-direction, 1 = right, -1 = left, 0 = none
  int yDir = 0;                 //Y-direction, 1 = down, -1 = up, 0 = none
  double speed = 0;             //How many pixels to move sprite by
  //Animation variables
  SDL_Texture* textureSheet = NULL;
  SDL_Rect rectPlacement;   //Where to place the sprite on the screen
  SDL_Rect rectSheet;       //Rectangle for entire sprite sheet
  int spriteFrame = 0;
  int frameCounter = 0;
  const int MaxSpriteFrame = 1;
  const int FrameDelay = 2;

public:
  //Function prototypes
  void FillRect(SDL_Rect& rect);
  SDL_Surface* loadImage();
  SDL_Texture* loadTexture(SDL_Surface* tempSurface);
  SDL_Surface* setTransparentColor(SDL_Surface* tempSurface);
  void setLocation(int x, int y);
  void NextFrame();
  void copyToRender();

public:
  //Constructors
  //Single, no transparency
  Sprite(std::string imagePath) 
    : path{imagePath}
  {
    //Set flag for no animation
    animated = false;

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
  //Single, transparent
  Sprite(std::string imagePath, RGB color)
    : path{imagePath}, transparency{color}
  {
    //Set flag for no animation
    animated = false;

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
  //Animated, no transparency
  Sprite(std::string imagePath, int numFrames)
    : path{imagePath}, MaxSpriteFrame{numFrames}
  {
    //Set flag for animation
    animated = true;

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
  //Animated, transparent
  Sprite(std::string imagePath, int numFrames, RGB color)
    : path{imagePath}, MaxSpriteFrame{numFrames}, transparency{color}
  {
    //Set flag for animation
    animated = true;

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
  //Destructor - Clean up textures when object goes out of scope
  ~Sprite() {
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(textureSheet);
  }
};

//
//BEGIN PROGRAM//
//

int main(int argc, char* args[]) {
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    std::cout << "Failed to initialize SDL!\n";
    return 1;
  }

  //Create the game window
  gameWindow = SDL_CreateWindow("Matthew Kavanagh - Midterm Program", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  //Create a renderer
  renderer = SDL_CreateRenderer(gameWindow, -1, 0);

  //Set up the graphics objects
  //Create background object at 0,0
  Sprite background("/graphics/background.bmp");
  background.setLocation(0, 0);

  //Create player paddle and align at center right
  Sprite player("/graphics/paddle.bmp");
  player.setLocation(SCREEN_WIDTH - player.width, (SCREEN_HEIGHT - player.height) / 2);

  //Create target object and align at center left
  Sprite target("/graphics/target.bmp", 8);
  target.setLocation(0, (SCREEN_HEIGHT - target.height) / 2);

  //Create the ball object and start in random location
  Sprite ball("/graphics/ball.bmp", 8);

  //Create score objects and center along bottom
  Sprite score("graphics/score.bmp", 4);
  Sprite lives("graphics/health.bmp", 4);
  score.setLocation((SCREEN_WIDTH - (score.width + lives.width)) / 2, SCREEN_HEIGHT - score.height);
  lives.setLocation((SCREEN_WIDTH + (score.width + lives.width)) / 2, SCREEN_HEIGHT - score.height);
}

bool ProgramIsRunning() {
  SDL_Event event;
  bool running = true;
  while(SDL_PollEvent(&event)) {
    if(event.type == SDL_QUIT)
      running = false;
  }
  return running;
}

void CloseShop() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(gameWindow);
  SDL_Quit();
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
  if((xPosition < 0) || (xPosition > (SCREEN_WIDTH - width))) {
      std::cout << "Invalid move coordinate entered. Sprite not moved\n";
      return;
  }
  if((yPosition < 0) || (yPosition > (SCREEN_HEIGHT - height))) {
      std::cout << "Invalid move coordinate entered. Sprite not moved\n";
      return;
  }

  x = xPosition;
  y = yPosition;
  
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
/************************** Function: NextFrame() *****************************
*******************************************************************************/
void Sprite::NextFrame() {
}

void Sprite::copyToRender() {
  if(animated)
    SDL_RenderCopy(renderer, textureSheet, &rect, &rectPlacement);
  else
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}
