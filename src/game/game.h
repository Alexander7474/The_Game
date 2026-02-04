#pragma once

#include "player.h"
#include "../engine/weapon.h"

#include <GLFW/glfw3.h>
#include <random>
#include <string>
#include <vector>
#include <BBOP/Graphics.h>

// pointeur vers la fentre de jeu utilisable de partout
extern GLFWwindow *gameWindow;

// Variable global du moteur de jeu
extern double DELTA_TIME;
extern double FPS;
extern double FPS_COUNTER;
extern double LAST_FPS_UPDATE;
extern double GAME_SPEED;
extern double ANIM_SPEED;
extern std::default_random_engine RANDOM_ENGINE;

class Game {
      private:
        Scene scene;
        Camera mainPlayerCam;
        Player mainPlayer;
 
        std::vector<Weapon*> weapons;
        GameCharacter dummy;
      public:
        Game();

        /**
         * @brief Met a jour la game
         */
        void update();

        /**
         * @brief Dessine la game avec la scene
         */
        void Draw();

        //GETTER 
        Camera *getMainPlayerCam();
};
