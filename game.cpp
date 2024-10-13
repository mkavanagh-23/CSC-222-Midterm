#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <cstdio>
#include <iostream>
#include "game.h"
#include "settings.h"
#include "sprite.h"

//Set up the game
//Window
SDL_Window* gameWindow = NULL;
//Renderer
SDL_Renderer* renderer;

//Gameplay Variables
int playerScore;
int playerLives;

//Functions
bool InitSDL() {
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    std::cout << "SDL failed to initialize!\n";
    SDL_Quit();
    return false;
  }

  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
    std::cout << "Audio failed to intialize!\n";
    return false;
  }
  return true;
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

bool InitGame() {
  //Initialize SDL
  if(!InitSDL())
    return false;

  //Initialize window and renderer
  gameWindow = SDL_CreateWindow("Matthew Kavanagh - Midterm Program", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Settings::SCREEN_WIDTH, Settings::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(gameWindow, -1, 0);

  //Initialize gameplay variables
  playerScore = 0;
  playerLives = 3;

  return true;
}

void ClearScreen() {
  //Create a buffer to store score/window title
  char score[64];
  //Copy score values to the buffer
  sprintf(score, "Player Score: %d | Player Lives: %d", playerScore, playerLives);
  //Set the window title to the buffer
  SDL_SetWindowTitle(gameWindow, score);

  //Clear the screen
  SDL_RenderClear(renderer);
}

void PresentRender() {
  SDL_RenderPresent(renderer);
  SDL_Delay(10);
}

void UpdateGame() {

}

void UpdatePlayer() {

}

void UpdateTarget() {

}

void UpdateBall() {

}

void UpdateScore() {

}
