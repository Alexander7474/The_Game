#pragma once

#include <BBOP/Graphics/bbopGlobal.h>
#include <BBOP/Graphics/cameraClass.h>

#include "../engine/crossair.h"
#include "../engine/gameCharacter.h"

class Game;

class Player final : public BbopDrawable {
      private:
        GameCharacter character;

        Crossair crossair;

        Game *game = nullptr; //<! Le player garde un pointer vers sa game

      public:
        Player() = default;

        explicit Player(Game *game);

        /**
         * @brief Met a jour le player
         */
        void update();

        virtual void Draw(GLint *renderUniforms) const override;

        GameCharacter &getCharacter();
        Crossair &getCrossair();
};
