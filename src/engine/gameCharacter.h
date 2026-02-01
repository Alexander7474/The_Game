#ifndef GAME_CHARACTER_H
#define GAME_CHARACTER_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <glm/vec2.hpp>
#include <BBOP/Graphics.h>
#include <BBOP/Graphics/bbopMathClass.h>

#include "animation.h"

enum class BodyState { 
  idle,
  running,
  smoking,
  gun,
  gunShooting,
  dead 
};

std::string bodyStateToString(BodyState e) noexcept;

enum class LegsState {
  idle,
  running
};

class GameCharacter : public BbopDrawable, public Geometric{
      private:
        float hp;

        Sprite legs;
        Sprite body;
        
        std::unordered_map<BodyState, Animation> bodyAnimations;
        std::unordered_map<LegsState, Animation> legsAnimations;

        BodyState bState;
        LegsState lState;

        // utiliser la la logique d'update
        glm::vec2 lookingPoint;
        glm::vec2 movement;
        float acceleration;
        float deceleration;
        glm::vec2 speed;
      public:
        GameCharacter(std::string characterFolder = "player/");

        /**
         * @brief Draw the character
         */
        void Draw(GLint *renderUniforms) const override;

        /**
         * @brief Update the character
         */
        void update();

        /** Ensemble de méthode utiliser pour controller le character **/

        /**
         *  @brief Change l'endroit ou regarde le character
         */
        void lookAt(glm::vec2 point);

        /**
         * @brief deplace le GameCharacter
         *
         * @param moveVec vecteur de déplacement
         */
        void move(glm::vec2 moveVec);
        void move(float x, float y);
};

#endif
