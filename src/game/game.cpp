#include "game.h"
#include "../engine/macro.h"

#include <memory>
#include <string>

GLFWwindow *gameWindow = nullptr;

double DELTA_TIME = 0;
double FPS = 0;
double FPS_COUNTER = 0;
double LAST_FPS_UPDATE = glfwGetTime();
std::default_random_engine RANDOM_ENGINE;

Game::Game()
    : mainPlayer(this)
{}

void Game::update() {
        mainPlayer.update();
        // update des éléments des la game
}

void Game::Draw() {
        scene.Use();
        scene.useCamera(&mainPlayerCam);
        scene.Draw(mainPlayer);
        scene.render();
}

Camera *Game::getMainPlayerCam()
{
  return &mainPlayerCam;
}
