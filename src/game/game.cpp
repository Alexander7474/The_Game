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
double GAME_SPEED = 1.0;
double ANIM_SPEED = 1.0/15.0;
std::random_device rd;
std::default_random_engine RANDOM_ENGINE(rd());

Game::Game()
    : mainPlayer(this)
{
  dummy.setPosition(200,200);
  weapons.push_back(new Weapon(WeaponName::bate, "assets/weapons/bate.png"));
  weapons[0]->setPosition(200,150);
}

void Game::update() {
        if(dummy.getBody()->getCollisionBox().check(mainPlayer.getCharacter().getBody()->getCollisionBox()) && mainPlayer.getCharacter().isLetal())
          dummy.switchState(BodyState::dead);

        for (auto w = weapons.begin(); w != weapons.end(); ) {
          if((*w) != nullptr){
            if(mainPlayer.getCharacter().getBody()->getCollisionBox().check((*w)->getCollisionBox()) && mainPlayer.getCharacter().isPicking()){
              mainPlayer.getCharacter().changeWeapon((*w));
            }

            if((*w)->getName() == WeaponName::fist){
              weapons.erase(w);
            }else{
              (*w)->update();
              w++;
            }
          }else{
            weapons.erase(w);
          }
        }

        // TODO -- optimiser le drop des arme je sens que cette merde est mal foutu
        // TODO -- patcher le crash lors la suppression de point dans la liste d'arme
        // TODO -- trouvzer un moyen de suppr les points de la liste d'arme
        if(mainPlayer.getCharacter().isPicking()){
          Weapon* w = mainPlayer.getCharacter().dropWeapon();
          if(w != nullptr)
            weapons.push_back(w);
        }

        mainPlayer.update();
        dummy.update();
#ifdef IMGUI_DEBUG
        // Interface character info
        ImGui::Begin("Game Info");
        if(dummy.getBody()->getCollisionBox().check(mainPlayer.getCharacter().getBody()->getCollisionBox()))
          ImGui::Text("Hit");
        ImGui::Text("Weapons vector size: %zu", weapons.size());
        ImGui::End();
#endif
}

void Game::Draw() {
        scene.Use();
        scene.useCamera(&mainPlayerCam);
        scene.Draw(dummy);
        for(auto &w : weapons){
          if(w != nullptr) {
            scene.Draw(*w);
            bbopDebugCollisionBox(w->getCollisionBox(), scene);
          }
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
