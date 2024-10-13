#include <SDL2/SDL.h>
#include "sprite.h"
#include "settings.h"
#include "game.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>

//Global Everything
//Window
extern SDL_Window* gameWindow;

//Renderer
extern SDL_Renderer* renderer;

//Function Prototypes

//
//BEGIN PROGRAM//
//

int main(int argc, char* args[]) {
  //Initialize gameplay objects
  if(!InitGame()) {
    CloseShop();
    return 1;
  }

  //Create sprite objects (images)
  //Create background object at 0,0
  Sprite background("graphics/spiderbackground.bmp");
  background.setLocation(Coordinate{0, 0});

  //Create player paddle and align at center right
  Sprite player("graphics/bonepaddle.bmp", RGB{241, 0, 251});
  player.setLocation(Coordinate{Settings::SCREEN_WIDTH - player.width, (Settings::SCREEN_HEIGHT - player.height) / 2});

  //Create target object and align at center left
  AnimatedSprite target("graphics/skeletonsheet.bmp", 12, RGB{0, 0, 0});
  target.setLocation(Coordinate{0, (Settings::SCREEN_HEIGHT - target.height) / 2});

  //Create the ball object and start centered horizontally with a random vertical position between 150 and 450
  AnimatedSprite ball("graphics/skullsheet.bmp", 8, RGB{0, 255, 252});
  ball.setLocation(Coordinate{(Settings::SCREEN_WIDTH - ball.width) / 2, rand()% 300 + 150});

  //Create score objects and center along bottom
  //AnimatedSprite score("graphics/score.bmp", 4);
  //AnimatedSprite lives("graphics/health.bmp", 4);
  //score.setLocation(Coordinate{(Settings::SCREEN_WIDTH - (score.width + lives.width)) / 2, Settings::SCREEN_HEIGHT - score.height});
  //lives.setLocation(Coordinate{(Settings::SCREEN_WIDTH + (score.width + lives.width)) / 2, Settings::SCREEN_HEIGHT - score.height});

  //PLAY THE GAME
  while(ProgramIsRunning()) {
    long int oldTime = SDL_GetTicks(); //The current time of program execution

    //Draw the screen
    ClearScreen();
    background.copyToRender();
    ball.copyToRender();
    player.copyToRender();
    target.copyToRender();
    //score.copyToRender();
    //lives.copyToRender();
    PresentRender();

    //Update game objects
    player.move();
    target.move();
    ball.move();

    int frameTime = SDL_GetTicks() - oldTime; //Calculate how long it took to draw and run the game

    //Determine how long to delay based on time to run
    if(frameTime < Settings::FRAME_DELAY)
      SDL_Delay(Settings::FRAME_DELAY - frameTime);
  } //END GAME LOOP

  CloseShop();
  return 0;
}//END MAIN


