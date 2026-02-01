#ifndef GAME_CHARACTER_H
#define GAME_CHARACTER_H

#include <memory>
#include <string>
#include <glm/vec2.hpp>
#include <BBOP/Graphics.h>
#include <BBOP/Graphics/bbopMathClass.h>

class GameCharacter : public BbopDrawable, public Geometric{
      private:
        float hp;

        Sprite legs;
        Sprite body;

        glm::vec2 lookingPoint;
      public:
        GameCharacter();

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
