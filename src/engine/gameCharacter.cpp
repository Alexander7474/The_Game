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

GameCharacter::GameCharacter()
  : legs("assets/Top_Down_Survivor/feet/idle/survivor-idle_0.png"),
    body("assets/Top_Down_Survivor/handgun/idle/survivor-idle_handgun_0.png")
{

}

void GameCharacter::update() {
        // fais suivre les legs et le body sur la posisition du character 
        legs.setPosition(getPosition());
        body.setPosition(getPosition());

        // calcule angle avec le looking point
        glm::vec2 pos(getPosition().x, getPosition().y);
        float dot = glm::dot(glm::normalize(pos), glm::normalize(lookingPoint));
        dot = glm::clamp(dot, -1.f, 1.f);
        float angle = glm::acos(dot);

#ifdef IMGUI_DEBUG
        // Interface character info
        ImGui::Begin("GameCharacter Info");
        ImGui::Text("Health Point: %f", hp);
        ImGui::Text("Position: (%f, %f)", getPosition().x, getPosition().y);
        ImGui::Text("Looking point: (%f, %f)", lookingPoint.x, lookingPoint.y);
        ImGui::Text("Looking Angle: %f", angle);
        ImGui::End();
#endif
}

void GameCharacter::Draw(GLint *renderUniforms) const {
        legs.Draw(renderUniforms);
        body.Draw(renderUniforms);
}


void GameCharacter::lookAt(glm::vec2 point){
        lookingPoint = point;
}

void GameCharacter::move(glm::vec2 moveVec) {
        setPosition(getPosition().x + moveVec.x, getPosition().y + moveVec.y);
}

void GameCharacter::move(float x, float y) {
        setPosition(getPosition().x + x, getPosition().y + y);
}
