#include <SDL2/SDL.h>
#include "sprite.h"
#include "settings.h"
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
//SDL_Surface* tempSurface;

//Function Prototypes
bool ProgramIsRunning();
void CloseShop();

//
//BEGIN PROGRAM//
//

int main(int argc, char* args[]) {
  //Initialize all SDL variables
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    std::cout << "Failed to initialize SDL!\n";
    return 1;
  }

  //Create the game window
  gameWindow = SDL_CreateWindow("Matthew Kavanagh - Midterm Program", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Settings::SCREEN_WIDTH, Settings::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  //Create a renderer
  renderer = SDL_CreateRenderer(gameWindow, -1, 0);

  //Set up Gameplay Variables
  int playerScore = 0;
  int playerLives = 3;

  //Set up the graphics objects
  //Create background object at 0,0
  Sprite background(renderer, "/graphics/background.bmp");
  background.setLocation(0, 0);

  //Create player paddle and align at center right
  Sprite player(renderer, "/graphics/paddle.bmp", RGB{});
  player.setLocation(Settings::SCREEN_WIDTH - player.width, (Settings::SCREEN_HEIGHT - player.height) / 2);

  //Create target object and align at center left
  AnimatedSprite target(renderer, "/graphics/target.bmp", 8, RGB{});
  target.setLocation(0, (Settings::SCREEN_HEIGHT - target.height) / 2);

  //Create the ball object and start in random location
  AnimatedSprite ball(renderer, "/graphics/ball.bmp", 8, RGB{});

  //Create score objects and center along bottom
  AnimatedSprite score(renderer, "graphics/score.bmp", 4);
  AnimatedSprite lives(renderer, "graphics/health.bmp", 4);
  score.setLocation((Settings::SCREEN_WIDTH - (score.width + lives.width)) / 2, Settings::SCREEN_HEIGHT - score.height);
  lives.setLocation((Settings::SCREEN_WIDTH + (score.width + lives.width)) / 2, Settings::SCREEN_HEIGHT - score.height);

  //PLAY THE GAME
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

void Sprite::NextFrame() {
}

