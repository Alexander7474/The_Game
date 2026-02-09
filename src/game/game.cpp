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
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

GLFWwindow *gameWindow = nullptr;

double DELTA_TIME = 0;
double FPS = 0;
double FPS_COUNTER = 0;
double LAST_FPS_UPDATE = glfwGetTime();
double GAME_SPEED = 1;
double ANIM_SPEED = 1.0/15.0;
std::random_device rd;
std::default_random_engine RANDOM_ENGINE(rd());

Game::Game()
    : mainPlayer(this)
{
  dummy.setPosition(200,200);
  weapons.push_back(new Bat());
  weapons.push_back(new Gun(this));
  weapons[0]->setPosition(200,150);
  weapons[1]->setPosition(200,100);
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
        
        for (auto b = bullets.begin(); b != bullets.end(); ) {
          if((*b) != nullptr){
            if((*b)->getPosition().x < 0
                || (*b)->getPosition().y < 0
                || (*b)->getPosition().x > BBOP_WINDOW_RESOLUTION.x
                || (*b)->getPosition().y > BBOP_WINDOW_RESOLUTION.y
                ){
              bullets.erase(b);
            }else{
              if(dummy.getBody()->getCollisionBox().check((*b)->getCollisionBox()))
                dummy.switchState(BodyState::dead);
              (*b)->update();
              b++;
            }
          }else{
            bullets.erase(b);
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
        ImGui::Text("Bullets vector size: %zu", bullets.size());
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
        for(auto &b : bullets){
          if(b != nullptr){
            scene.Draw(*b);
            bbopDebugCollisionBox(b->getCollisionBox(), scene);
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

void Game::addBullet(Bullet *b){
  bullets.push_back(b);
}
