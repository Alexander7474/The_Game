#include "weapon.h"

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
  getCollisionBox().setPosition(getPosition());
}

const WeaponName Weapon::getName()
{
  return name;
}
