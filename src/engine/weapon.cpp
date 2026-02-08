#include "weapon.h"
#include "../game/game.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

std::string weaponNameToString(WeaponName e) noexcept
{
    switch (e)
    {
      case WeaponName::fist: return "fist";
      case WeaponName::bate: return "bate";
      case WeaponName::gun: return "gun";
      case WeaponName::shotgun: return "shotgun";
      case WeaponName::riffle: return "riffle";
      default: return "unknow";
    }
}

Weapon::Weapon(WeaponName name, std::string texturePath)
  : Sprite(texturePath.c_str())
{
  this->name = name;
  setSize(64,64);
  setOrigin(32,32);
  getCollisionBox().setSize(32,32);
  getCollisionBox().setOrigin(16,16);
  setAutoUpdateCollision(false);
}

void Weapon::update(){
  shapeCollisionBox.setPosition(getPosition());
}

void Weapon::use(){
  Mix_Chunk* gunSound = Mix_LoadWAV("assets/sounds/punch0.wav");
  Mix_PlayChannel(-1, gunSound, 0);
}

const WeaponName Weapon::getName()
{
  return name;
}

Firearme::Firearme(WeaponName name, std::string texturePath, Game* game)
  : Weapon(name, texturePath)
{
  armed = true;
  ammo = 15;
  firerate = 0.2;
  lastShot = 0;
  this->game = game;
}

bool Firearme::canFire()
{
  return (glfwGetTime() - lastShot >= firerate);
}

void Firearme::fire(glm::vec2 cible)
{
  if(!canFire())
    return;
  
  //calcule direction 
  glm::vec2 pos(getPosition().x, getPosition().y);
  glm::vec2 dir = cible - pos;

  //ajout de la balle dans la game
  Bullet* b = new Bullet("assets/weapons/bullet2.png", dir, pos);
  game->addBullet(b);

  Mix_Chunk* gunSound = Mix_LoadWAV("assets/sounds/gun.wav");
  Mix_PlayChannel(-1, gunSound, 0);
  
  lastShot = glfwGetTime();
}

Bullet::Bullet(std::string texturePath, glm::vec2 dir, glm::vec2 pos)
  : Sprite("assets/default.png")
{
  this->dir = glm::normalize(dir);
  float angle = atan2(dir.y, dir.x);
  setRotation(angle);
  speed = 2000.f;
  setPosition(pos.x, pos.y);
  setSize(16,16);
  setOrigin(0,8);
  animation.textures = bbopLoadSpriteSheet(texturePath.c_str(), 1, 4);
  setTexture(animation.textures[animation.frame]);

  move(this->dir.x*32,this->dir.y*32);
}

void Bullet::update()
{
    glm::vec2 mov = dir * speed * static_cast<float>(DELTA_TIME);
    move(mov.x, mov.y);

    // animation du personnage
    if(glfwGetTime() - animation.lastFrameTime >= ANIM_SPEED/GAME_SPEED/4.0){
        animation.frame++;
        animation.lastFrameTime = glfwGetTime();

        if(animation.frame >= animation.textures.size()){
            animation.frame = 0;
        }

    }

    setTexture(animation.textures[animation.frame]);
  
}
