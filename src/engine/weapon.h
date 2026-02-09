#pragma once 

#include <string>
#include <BBOP/Graphics.h>
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "animation.h"

class Game;

enum class WeaponName {
  fist,
  bate,
  gun,
  shotgun,
  riffle
};
std::string weaponNameToString(WeaponName e) noexcept;

class Weapon : public Sprite 
{
  private:
    WeaponName name;
    Mix_Chunk *sound;
  public:
    Weapon(WeaponName name = WeaponName::fist, std::string texturePath = "assets/default.png", std::string soundPath = "assets/default.wav");
 
    void update();

    void use();

    const WeaponName getName();

    virtual ~Weapon() = default;
};

class Firearme : public Weapon 
{
  private:
    int ammo;
    bool armed;
    double firerate; //<! cadence de tire
    double lastShot;

    Game *game;
  public:
    Firearme(WeaponName name = WeaponName::gun,  std::string texturePath = "assets/default.png", std::string soundPath = "assets/default.wav", Game *game = nullptr);

    bool canFire();

    /**
     * @brief Fais tirer l'arme 
     * 
     * @param dir direction de la balle
     */
    void fire(glm::vec2 cible);
};



class Bullet : public Sprite
{
  private:
    glm::vec2 dir;
    float speed;
    Animation animation;

  public:
    Bullet(std::string texturePath = "assets/weapons/bullet.png", glm::vec2 dir = {0,0}, glm::vec2 pos = {0,0});

    void update();
};

// declaration des différentes armes

class Fist : public Weapon {
  public:
    Fist();
};

class Bat : public Weapon {
  public:
    Bat();
};

class Gun : public Firearme {
  public:
    Gun(Game *game);
};
