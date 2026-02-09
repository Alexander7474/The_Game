#pragma once

#include "../engine/weapon.h"
#include "player.h"

#include <BBOP/Graphics.h>
#include <GLFW/glfw3.h>
#include <random>
#include <string>
#include <vector>

// TODO -- Créer un getionnaire de ressource pour ne pas recharger plusieur
// fois des textures pointeur vers la fentre de jeu utilisable de partout
extern GLFWwindow *gameWindow;

// Variable global du moteur de jeu
extern std::default_random_engine RANDOM_ENGINE;
extern double DELTA_TIME;
extern double FPS;
extern double FPS_COUNTER;
extern double LAST_FPS_UPDATE;
extern double GAME_SPEED;
extern double ANIM_SPEED;
extern bool TOGGLE_MUSIC;

class Game
{
      private:
	Scene scene;
	Camera mainPlayerCam;
	Player mainPlayer;

	std::vector<Weapon *> weapons;
	std::vector<Bullet *> bullets;
	std::vector<GameCharacter *> dummys;

	int musicChannel = -1;

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

	void addBullet(Bullet *b);

	// GETTER
	Camera *getMainPlayerCam();
};
