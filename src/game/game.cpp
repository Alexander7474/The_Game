#ifdef IMGUI_DEBUG
#include "../backends/imgui_impl_glfw.h"
#include "../backends/imgui_impl_opengl3.h"
#include "../imgui/imgui.h"
#endif
#include "game.h"
#include "../engine/macro.h"

#include <memory>
#include <string>
#include <BBOP/Graphics.h>

GLFWwindow *gameWindow = nullptr;

double DELTA_TIME = 0;
double FPS = 0;
double FPS_COUNTER = 0;
double LAST_FPS_UPDATE = glfwGetTime();
double GAME_SPEED = 1.0/15.0;
std::random_device rd;
std::default_random_engine RANDOM_ENGINE(rd());

Game::Game()
    : mainPlayer(this)
{
  dummy.setPosition(200,200);
  testWeapon = std::make_unique<Weapon>(WeaponName::bate, "assets/weapons/bate.png");
  testWeapon->setPosition(200,150);
}

void Game::update() {
        if(dummy.getBody()->getCollisionBox().check(mainPlayer.getCharacter().getBody()->getCollisionBox()) && mainPlayer.getCharacter().isLetal())
          dummy.switchState(BodyState::dead);

        if(testWeapon){
          if(mainPlayer.getCharacter().getBody()->getCollisionBox().check(testWeapon->getCollisionBox()) && mainPlayer.getCharacter().isPicking())
            mainPlayer.getCharacter().changeWeapon(testWeapon);
          testWeapon->update();
        }
 
        mainPlayer.update();

        dummy.update();
#ifdef IMGUI_DEBUG
        // Interface character info
        ImGui::Begin("Game Info");
        if(dummy.getBody()->getCollisionBox().check(mainPlayer.getCharacter().getBody()->getCollisionBox()))
          ImGui::Text("Hit");
        ImGui::End();
#endif
}

void Game::Draw() {
        scene.Use();
        scene.useCamera(&mainPlayerCam);
        scene.Draw(dummy);
        if(testWeapon){
          scene.Draw(*testWeapon.get());
          bbopDebugCollisionBox(testWeapon->getCollisionBox(), scene);
        }
        scene.Draw(mainPlayer);
        bbopDebugCollisionBox(dummy.getBody()->getCollisionBox(), scene);
        bbopDebugCollisionBox(mainPlayer.getCharacter().getBody()->getCollisionBox(), scene);
        scene.render();
}

Camera *Game::getMainPlayerCam()
{
  return &mainPlayerCam;
}
