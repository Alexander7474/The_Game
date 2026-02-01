#ifdef IMGUI_DEBUG
#include "../backends/imgui_impl_glfw.h"
#include "../backends/imgui_impl_opengl3.h"
#include "../imgui/imgui.h"
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <memory>

#include "../game/game.h"
#include "gameCharacter.h"

std::string bodyStateToString(BodyState e) noexcept
{
    switch (e)
    {
      case BodyState::idle: return "idle";
      case BodyState::running: return "running";
      default: return "unknow";
    }
}

GameCharacter::GameCharacter(std::string characterFolder)
  : legs("assets/default.png"),
    body("assets/default.png")
{
        acceleration = 1500.f;
        deceleration = 10.f;

        body.setSize(32,32);
        legs.setSize(32,32);
        body.setOrigin(body.getSize().x/2.f, body.getSize().y/2.f);
        legs.setOrigin(legs.getSize().x/2.f, legs.getSize().y/2.f);

        bState = BodyState::idle;
        lState = LegsState::idle;

        // chargement des animations du character pour chaque état
        std::string path = "assets/"+characterFolder+"body/"+bodyStateToString(BodyState::idle)+".png";
        bodyAnimations[BodyState::idle].textures = bbopLoadSpriteSheet(path.c_str(), 1, 1);

        path = "assets/"+characterFolder+"body/"+bodyStateToString(BodyState::running)+".png";
        bodyAnimations[BodyState::running].textures = bbopLoadSpriteSheet(path.c_str(), 6, 1);

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

        // gestion des états
        if(movement == glm::vec2(0,0)){
          bState = BodyState::idle;
          lState = LegsState::idle;
          legs.setRotation(body.getRotation());
        }

        // animation du personnage
        if(glfwGetTime() - bodyAnimations[bState].lastFrameTime >= GAME_SPEED){
                bodyAnimations[bState].frame++;
                bodyAnimations[bState].lastFrameTime = glfwGetTime();

                if(bodyAnimations[bState].frame == bodyAnimations[bState].textures.size())
                  bodyAnimations[bState].frame = 0;
                
                body.setTexture(bodyAnimations[bState].textures[bodyAnimations[bState].frame]);
        }
          
        // animation des jambes
        if(glfwGetTime() - legsAnimations[lState].lastFrameTime >= GAME_SPEED){
                legsAnimations[lState].frame++;
                legsAnimations[lState].lastFrameTime = glfwGetTime();

                if(legsAnimations[lState].frame == legsAnimations[lState].textures.size())
                  legsAnimations[lState].frame = 0;
                
                legs.setTexture(legsAnimations[lState].textures[legsAnimations[lState].frame]);
        }
        


#ifdef IMGUI_DEBUG
        // Interface character info
        ImGui::Begin("GameCharacter Info");
        ImGui::Text("Health Point: %f", hp);
        ImGui::Text("Position: (%f, %f)", getPosition().x, getPosition().y);
        ImGui::Text("speed vector: (%f, %f)", speed.x, speed.y);
        ImGui::Text("speed: %f", abs(speed.x) + abs(speed.y));
        ImGui::Text("Looking point: (%f, %f)", lookingPoint.x, lookingPoint.y);
        ImGui::Text("Looking Angle: %f", angle);
        ImGui::Text("BodyState: %s", bodyStateToString(bState).c_str());
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


void GameCharacter::lookAt(glm::vec2 point){
        lookingPoint = point;
}

void GameCharacter::move(glm::vec2 moveVec) {
        movement += moveVec;
        bState = BodyState::running;
        lState = LegsState::running;
}

void GameCharacter::move(float x, float y) {
        this->move(glm::vec2(x,y));
}
