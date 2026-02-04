#include "../engine/gameCharacter.h"
#include "../engine/macro.h"
#include "player.h"
#include "game.h"
#include "../engine/keys.h"

#include <GLFW/glfw3.h>
#include <BBOP/Graphics/cameraClass.h>
#include <BBOP/Graphics/bbopMathClass.h>
#include <glm/vec2.hpp>

using namespace std;

Player::Player(Game *game) : Player() { this->game = game; }

void Player::Draw(GLint *renderUniforms) const {
        character.Draw(renderUniforms);
        crossair.Draw(renderUniforms);
}

void Player::update() {
        // recup de la pos de la souris pour faire regarder le character
        double mouseX, mouseY;
        glfwGetCursorPos(gameWindow, &mouseX, &mouseY);
        Vector2f mousePos = game->getMainPlayerCam()->screenPosToCamPos(
            Vector2f(static_cast<float>(mouseX), static_cast<float>(mouseY)));
        crossair.setPosition(mousePos.x, mousePos.y);
        character.lookAt(glm::vec2(mousePos.x, mousePos.y));

        // gestion des inputs
        if (keys[GLFW_KEY_W]) {
          character.move(0.f, -1.f);
        }
        if (keys[GLFW_KEY_S]) {
          character.move(0.f, 1.f);
        }
        if (keys[GLFW_KEY_D]) {
          character.move(1.f, 0.f);
        }
        if (keys[GLFW_KEY_A]) {
          character.move(-1.f, 0.f);
        }

        if (mouseButton[GLFW_MOUSE_BUTTON_LEFT]) {
          character.useWeapon();
        }
        if (mouseButton[GLFW_MOUSE_BUTTON_RIGHT]) {
          character.pickup(true);
        }else{
          character.pickup(false);
        }

        character.update();
}

GameCharacter &Player::getCharacter() { return character; }
Crossair &Player::getCrossair() { return crossair; }
