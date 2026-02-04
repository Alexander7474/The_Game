#pragma once 

#include <string>
#include <BBOP/Graphics.h>

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
  public:
    Weapon(WeaponName name = WeaponName::fist, std::string texturePath = "assets/default.png");
 
    void update();

    const WeaponName getName();

    virtual ~Weapon() = default;
};

class Firearme : public Weapon 
{
  private:
    int ammo;
  public:
    Firearme(WeaponName name = WeaponName::gun,  std::string texturePath = "assets/weapons/gun.png");
};
