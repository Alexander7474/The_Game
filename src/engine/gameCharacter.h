#ifndef GAME_CHARACTER_H
#define GAME_CHARACTER_H

#include <memory>
#include <string>

#include <BBOP/Graphics.h>
#include <BBOP/Graphics/bbopMathClass.h>

class GameCharacter : public BbopDrawable, public Geometric{
      private:
        float hp;

        Sprite legs;
        Sprite body;

      public:
        GameCharacter();

        void update();

        /**
         * @brief Draw the character
         */
        void Draw(GLint *renderUniforms) const override;
};

#endif
