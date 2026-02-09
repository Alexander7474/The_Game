#ifdef IMGUI_DEBUG
#include "../backends/imgui_impl_glfw.h"
#include "../backends/imgui_impl_opengl3.h"
#include "../imgui/imgui.h"
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <memory>
#include <utility>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "../game/game.h"
#include "gameCharacter.h"
#include "ressourceManager.h"

std::string bodyStateToString(BodyState e) noexcept
{
    switch (e)
    {
      case BodyState::idle: return "idle";
      case BodyState::running: return "running";
      case BodyState::smoking: return "smoking";
      case BodyState::attacking: return "attacking";
      case BodyState::dead: return "dead";
      default: return "unknow";
    }
}

GameCharacter::GameCharacter(std::string characterFolder)
  : legs("assets/default.png"),
    body("assets/default.png")
{
        acceleration = 2500.f;
        deceleration = 10.f;

        lookingPoint = glm::vec2(200,200);

        body.setSize(64,64);
        legs.setSize(64,64);
        body.setOrigin(body.getSize().x/2.f, body.getSize().y/2.f);
        legs.setOrigin(legs.getSize().x/2.f, legs.getSize().y/2.f);

        // TODO - Patcher les collision de Bbop (checkRotation et gestion de l'offset avec l'origine)
        body.getCollisionBox().setSize(20,20);
        body.getCollisionBox().setOrigin(10,10);
        body.setAutoUpdateCollision(false);

        weapon = new Fist;
        switchWeaponCooldown = 0.2;
        lastWeaponSwitch = 0.0;

        bState = {BodyState::idle, weapon->getName()};
        lState = LegsState::idle;

        // chargement des animations du character pour chaque état
        std::string path = "assets/"+characterFolder+"body/idle.png";
        bodyAnimations[{BodyState::idle, WeaponName::fist}].textures = bbopLoadSpriteSheet(path.c_str(), 1, 1);

        path = "assets/"+characterFolder+"body/running.png";
        bodyAnimations[{BodyState::running, WeaponName::fist}].textures = bbopLoadSpriteSheet(path.c_str(), 8, 1);

        path = "assets/"+characterFolder+"body/attacking.png";
        bodyAnimations[{BodyState::attacking, WeaponName::fist}].textures = bbopLoadSpriteSheet(path.c_str(), 6, 1);
        bodyAnimations[{BodyState::attacking, WeaponName::fist}].knockoutRange = {1,4};

        path = "assets/"+characterFolder+"body/idle_bate.png";
        bodyAnimations[{BodyState::idle, WeaponName::bate}].textures = bbopLoadSpriteSheet(path.c_str(), 1, 1);

        path = "assets/"+characterFolder+"body/running_bate.png";
        bodyAnimations[{BodyState::running, WeaponName::bate}].textures = bbopLoadSpriteSheet(path.c_str(), 8, 1);

        path = "assets/"+characterFolder+"body/attacking_bate.png";
        bodyAnimations[{BodyState::attacking, WeaponName::bate}].textures = bbopLoadSpriteSheet(path.c_str(), 1, 8);
        bodyAnimations[{BodyState::attacking, WeaponName::bate}].letalRange = {3,7};

        path = "assets/"+characterFolder+"body/idle_gun.png";
        bodyAnimations[{BodyState::idle, WeaponName::gun}].textures = bbopLoadSpriteSheet(path.c_str(), 1, 1);

        path = "assets/"+characterFolder+"body/running_gun.png";
        bodyAnimations[{BodyState::running, WeaponName::gun}].textures = bbopLoadSpriteSheet(path.c_str(), 8, 1);

        path = "assets/"+characterFolder+"body/attacking_gun.png";
        bodyAnimations[{BodyState::attacking, WeaponName::gun}].textures = bbopLoadSpriteSheet(path.c_str(), 1, 2);

        // nombre aléatoire pour le sprite de mort
        std::string deadId = std::to_string(std::uniform_int_distribution<int>(0, 2)(RANDOM_ENGINE));
        path = "assets/"+characterFolder+"body/dead"+deadId+".png";
        bodyAnimations[{BodyState::dead, WeaponName::fist}].textures = bbopLoadSpriteSheet(path.c_str(), 1, 1);

        path = "assets/"+characterFolder+"legs/"+bodyStateToString(BodyState::idle)+".png";
        legsAnimations[LegsState::idle].textures = bbopLoadSpriteSheet(path.c_str(), 1, 1);

        path = "assets/"+characterFolder+"legs/"+bodyStateToString(BodyState::running)+".png";
        legsAnimations[LegsState::running].textures = bbopLoadSpriteSheet(path.c_str(), 12, 1);
}

void GameCharacter::update() {
        if(abs(movement.x) + abs(movement.y) >= 2){
          movement /= 2.f;
        }
        speed = (speed + (movement * acceleration * static_cast<float>(DELTA_TIME)));
        
        // gestion du déplacement
        setPosition(getPosition().x + speed.x * DELTA_TIME, getPosition().y + speed.y * DELTA_TIME);
        // fais suivre les legs et le body sur la posisition du character 
        legs.setPosition(getPosition());
        body.setPosition(getPosition());
        if(weapon == nullptr)
          std::cerr << "Erreur avec le pointeur weapon de GameCharacter" << std::endl;
        weapon->setPosition(getPosition());
        weapon->update();
        body.getCollisionBox().setPosition(getPosition());

        // calcule angle avec le looking point
        glm::vec2 pos(getPosition().x, getPosition().y);
        glm::vec2 dir = lookingPoint - pos;
        float angle = atan2(dir.y, dir.x);

        body.setRotation(angle);

        // calcule angle des jambes
        angle = atan2(speed.y, speed.x);
        legs.setRotation(angle);

        if(movement == glm::vec2(0,0)){
          legs.setRotation(body.getRotation());
          if(walkAudioCanal != -1){
            Mix_HaltChannel(walkAudioCanal);
            walkAudioCanal = -1;
          }
          if(bState.first == BodyState::running)
            this->switchState(BodyState::idle);
        }else{
          if(walkAudioCanal == -1){
            walkAudioCanal = Mix_PlayChannel(-1, RessourceManager::getSound("assets/sounds/walking.wav"), -1);
          }
        }

        // animation du personnage
        if(glfwGetTime() - bodyAnimations[bState].lastFrameTime >= ANIM_SPEED/GAME_SPEED){
                bodyAnimations[bState].frame++;
                bodyAnimations[bState].lastFrameTime = glfwGetTime();

                if(bodyAnimations[bState].frame >= bodyAnimations[bState].textures.size()){
                  bodyAnimations[bState].frame = 0;
                  
                  this->switchState(BodyState::idle);
                }
                
        }
          
        // animation des jambes
        if(glfwGetTime() - legsAnimations[lState].lastFrameTime >= ANIM_SPEED/GAME_SPEED){
                legsAnimations[lState].frame++;
                legsAnimations[lState].lastFrameTime = glfwGetTime();

                if(legsAnimations[lState].frame == legsAnimations[lState].textures.size())
                  legsAnimations[lState].frame = 0;
                
        }

        legs.setTexture(legsAnimations[lState].textures[legsAnimations[lState].frame]);
        body.setTexture(bodyAnimations[bState].textures[bodyAnimations[bState].frame]);

#ifdef IMGUI_DEBUG
        // Interface character info
        ImGui::Begin("GameCharacter Info");
        ImGui::Text("Health Point: %f", hp);
        ImGui::Text("Position: (%f, %f)", getPosition().x, getPosition().y);
        ImGui::Text("speed vector: (%f, %f)", speed.x, speed.y);
        ImGui::Text("movement vector: (%f, %f)", movement.x, movement.y);
        ImGui::Text("speed: %f", abs(speed.x) + abs(speed.y));
        ImGui::Text("Looking point: (%f, %f)", lookingPoint.x, lookingPoint.y);
        ImGui::Text("Body Angle: %f", body.getRotation());
        ImGui::Text("Legs Angle: %f", legs.getRotation());
        ImGui::Text("BodyState pair: {%s, %s}", bodyStateToString(bState.first).c_str(), weaponNameToString(bState.second).c_str());
        ImGui::Text("Body Frame: %d", bodyAnimations[bState].frame);
        ImGui::Text("isLetal: %d", this->isLetal());
        ImGui::Text("pickupFlag: %d", pickupFlag);
        ImGui::End();
#endif
 

        // reset et diminution
        movement = glm::vec2(0.f,0.f);
        speed *= 1 - deceleration * static_cast<float>(DELTA_TIME);
}

void GameCharacter::Draw(GLint *renderUniforms) const {
        legs.Draw(renderUniforms);
        body.Draw(renderUniforms);
}

void GameCharacter::switchState(BodyState state){
        if(bState.first == BodyState::dead)
          return;

        switch(state){
          case BodyState::idle:
            if(bState.first == BodyState::idle)
              break;
            if(bState.first == BodyState::attacking && dynamic_cast<Firearme*>(weapon) == nullptr)
              body.flipHorizontally();
            bState.first = BodyState::idle;
            lState = LegsState::idle;
            bodyAnimations[bState].lastFrameTime = glfwGetTime();
            break;
          case BodyState::smoking:
            bState.first = BodyState::smoking;
            bodyAnimations[bState].lastFrameTime = glfwGetTime();
            break;
          case BodyState::running:{
            if(bState.first == BodyState::attacking || bState.first == BodyState::running)
              break;
            bState.first = BodyState::running;
            lState = LegsState::running;
            bodyAnimations[bState].lastFrameTime = glfwGetTime();
            break;
            }
          case BodyState::attacking:{
            if(bState.first == BodyState::attacking)
              break;
            Firearme* a = dynamic_cast<Firearme*>(weapon);
            if(a != nullptr){
              if(a->canFire()){
                a->fire(lookingPoint);
                bState.first = BodyState::attacking;
                bodyAnimations[bState].lastFrameTime = glfwGetTime();
              }
              break;
            }
            weapon->use();
            bState.first = BodyState::attacking;
            bodyAnimations[bState].lastFrameTime = glfwGetTime();
            break;}
          case BodyState::dead:{
            bState = {BodyState::dead, WeaponName::fist};
            bodyAnimations[bState].lastFrameTime = glfwGetTime();
            Mix_Chunk* s = Mix_LoadWAV("assets/sounds/blood_split.wav");
            Mix_PlayChannel(-1, s, 0);
            }
            break;
          default:
            break;
        }
}

void GameCharacter::lookAt(glm::vec2 point){
        lookingPoint = point;
}

void GameCharacter::move(glm::vec2 moveVec) {
        movement += moveVec;
        this->switchState(BodyState::running);
}

void GameCharacter::move(float x, float y) {
        this->move(glm::vec2(x,y));
}

void GameCharacter::useWeapon(){
        this->switchState(BodyState::attacking);
}

Sprite *GameCharacter::getBody(){
        return &body;
}

Sprite *GameCharacter::getLegs(){
        return &legs;
}

bool GameCharacter::isLetal(){
        if(bState.first == BodyState::attacking && bodyAnimations[bState].frame >= bodyAnimations[bState].letalRange.first && bodyAnimations[bState].frame <= bodyAnimations[bState].letalRange.second)
          return true;
        return false;
}

void GameCharacter::pickup(bool s){
        pickupFlag = s;
}

bool GameCharacter::isPicking() {
        return pickupFlag;
}

void GameCharacter::changeWeapon(Weapon*& newWeapon){
        if(glfwGetTime() - lastWeaponSwitch <= switchWeaponCooldown)
          return;
        Weapon *tmp = weapon;
        weapon = newWeapon;
        newWeapon = tmp;
        bState.second = weapon->getName();
        lastWeaponSwitch = glfwGetTime();
}

Weapon* GameCharacter::dropWeapon() {
        // TODO - c'est de la merde
        if(glfwGetTime() - lastWeaponSwitch <= switchWeaponCooldown)
          return nullptr;
        if(weapon->getName() == WeaponName::fist)
          return nullptr;
        Weapon *tmp = weapon;
        weapon = new Fist();
        bState.second = weapon->getName();
        lastWeaponSwitch = glfwGetTime();
        return tmp;
}
