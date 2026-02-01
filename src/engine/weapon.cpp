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
}

const WeaponName Weapon::getName()
{
  return name;
}
