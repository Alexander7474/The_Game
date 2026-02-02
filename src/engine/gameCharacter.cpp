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

#include "../game/game.h"
#include "gameCharacter.h"

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
        acceleration = 1500.f;
        deceleration = 10.f;

        body.setSize(64,64);
        legs.setSize(64,64);
        body.setOrigin(body.getSize().x/2.f, body.getSize().y/2.f);
        legs.setOrigin(legs.getSize().x/2.f, legs.getSize().y/2.f);

        bState = {BodyState::idle, WeaponName::fist};
        lState = LegsState::idle;

        // chargement des animations du character pour chaque état
        std::string path = "assets/"+characterFolder+"body/idle.png";
        bodyAnimations[{BodyState::idle, WeaponName::fist}].textures = bbopLoadSpriteSheet(path.c_str(), 1, 1);

        path = "assets/"+characterFolder+"body/running.png";
        bodyAnimations[{BodyState::running, WeaponName::fist}].textures = bbopLoadSpriteSheet(path.c_str(), 8, 1);

        path = "assets/"+characterFolder+"body/attacking.png";
        bodyAnimations[{BodyState::attacking, WeaponName::fist}].textures = bbopLoadSpriteSheet(path.c_str(), 6, 1);

        path = "assets/"+characterFolder+"body/idle_bate.png";
        bodyAnimations[{BodyState::idle, WeaponName::bate}].textures = bbopLoadSpriteSheet(path.c_str(), 1, 1);

        path = "assets/"+characterFolder+"body/running_bate.png";
        bodyAnimations[{BodyState::running, WeaponName::bate}].textures = bbopLoadSpriteSheet(path.c_str(), 8, 1);

        path = "assets/"+characterFolder+"body/attacking_bate.png";
        bodyAnimations[{BodyState::attacking, WeaponName::bate}].textures = bbopLoadSpriteSheet(path.c_str(), 1, 8);

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

        // calcule angle avec le looking point
        glm::vec2 pos(getPosition().x, getPosition().y);
        glm::vec2 dir = lookingPoint - pos;
        float angle = atan2(dir.y, dir.x);

        body.setRotation(angle);

        // calcule angle des jambes
        angle = atan2(speed.y, speed.x);
        legs.setRotation(angle);

        if(movement == glm::vec2(0,0)){
          if(bState.first == BodyState::running)
            this->switchState(BodyState::idle);
          legs.setRotation(body.getRotation());
        }

        // animation du personnage
        if(glfwGetTime() - bodyAnimations[bState].lastFrameTime >= GAME_SPEED){
                bodyAnimations[bState].frame++;
                bodyAnimations[bState].lastFrameTime = glfwGetTime();

                if(bodyAnimations[bState].frame >= bodyAnimations[bState].textures.size()){
                  bodyAnimations[bState].frame = 0;
                  
                  this->switchState(BodyState::idle);
                }
                
        }
          
        // animation des jambes
        if(glfwGetTime() - legsAnimations[lState].lastFrameTime >= GAME_SPEED){
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
        ImGui::Text("speed: %f", abs(speed.x) + abs(speed.y));
        ImGui::Text("Looking point: (%f, %f)", lookingPoint.x, lookingPoint.y);
        ImGui::Text("Body Angle: %f", body.getRotation());
        ImGui::Text("Legs Angle: %f", legs.getRotation());
        ImGui::Text("BodyState pair: {%s, %s}", bodyStateToString(bState.first).c_str(), weaponNameToString(bState.second).c_str());
        ImGui::Text("Body Frame: %d", bodyAnimations[bState].frame);
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
        switch(state){
          case BodyState::idle:
            if(bState.first == BodyState::idle)
              break;
            //si le joueur était en attaque 
            if(bState.first == BodyState::attacking)
              body.flipHorizontally();
            bState.first = BodyState::idle;
            lState = LegsState::idle;
            bodyAnimations[bState].lastFrameTime = glfwGetTime();
            break;
          case BodyState::smoking:
            bState.first = BodyState::smoking;
            bodyAnimations[bState].lastFrameTime = glfwGetTime();
            break;
          case BodyState::running:
            if(bState.first == BodyState::attacking || bState.first == BodyState::running)
              break;
            bState.first = BodyState::running;
            lState = LegsState::running;
            bodyAnimations[bState].lastFrameTime = glfwGetTime();
            break;
          case BodyState::attacking:
            if(bState.first == BodyState::attacking)
              break;
            bState.first = BodyState::attacking;
            bodyAnimations[bState].lastFrameTime = glfwGetTime();
            break;
          case BodyState::dead:
            bState.first = BodyState::dead;
            bodyAnimations[bState].lastFrameTime = glfwGetTime();
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
