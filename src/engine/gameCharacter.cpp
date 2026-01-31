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
#ifdef IMGUI_DEBUG
        // Interface character info
        ImGui::Begin("GameCharacter Info");
        ImGui::Text("Health Point: %f", hp);
        ImGui::Text("Position: (%f, %f)", getPosition().x, getPosition().y);
        ImGui::End();
#endif
}

void GameCharacter::Draw(GLint *renderUniforms) const {
        legs.Draw(renderUniforms);
        body.Draw(renderUniforms);
}
