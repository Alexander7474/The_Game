#include "../engine/gameCharacter.h"
#include "../engine/macro.h"
#include "player.h"
#include "game.h"

#include <GLFW/glfw3.h>
#include <BBOP/Graphics/cameraClass.h>
#include <BBOP/Graphics/bbopMathClass.h>

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
}

GameCharacter &Player::getCharacter() { return character; }
Crossair &Player::getCrossair() { return crossair; }
