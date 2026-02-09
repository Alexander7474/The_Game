#ifndef GAME_CHARACTER_H
#define GAME_CHARACTER_H

#include <BBOP/Graphics.h>
#include <BBOP/Graphics/bbopMathClass.h>
#include <glm/vec2.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "animation.h"
#include "weapon.h"

enum class BodyState { idle, running, smoking, attacking, dead };
std::string bodyStateToString(BodyState e) noexcept;

enum class LegsState { idle, running };

// Permet de hash la paire entre BodyState et WeaponName
struct PairHash {
	std::size_t operator()(
		const std::pair<BodyState, WeaponName> &p) const noexcept
	{
		return (static_cast<std::size_t>(p.first) << 32) ^
			static_cast<std::size_t>(p.second);
	}
};

class GameCharacter : public BbopDrawable, public Geometric
{
      private:
	Sprite legs;
	Sprite body;

	std::unordered_map<std::pair<BodyState, WeaponName>, Animation,
		PairHash>
		bodyAnimations;
	std::unordered_map<LegsState, Animation> legsAnimations;

	std::pair<BodyState, WeaponName> bState;
	LegsState lState;

	Weapon *weapon;

	// utiliser la la logique d'update
	float hp;
	glm::vec2 lookingPoint;
	glm::vec2 movement;
	float acceleration;
	float deceleration;
	glm::vec2 speed;
	bool pickupFlag;

	// Pour l'instant le cooldown ne prend pas en compte la vitesse du temps dans
	// le jeu je n'en voit pas l'interet
	double switchWeaponCooldown;
	double lastWeaponSwitch;

	// canal audio attribué
	int walkAudioCanal = -1;

      public:
	GameCharacter(std::string characterFolder = "player/");

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

	/**
   * @brief Utiise l'arme équipé
   */
	void useWeapon();

	/**
   * @brief Met le pickup flag sur true ou false
   */
	void pickup(bool s);
	bool isPicking();

	/**
   * @brief remplace le weapon par un autre
   */
	void changeWeapon(Weapon *&newWeapon);
	Weapon *dropWeapon();

	/**
   * @brief Change l'état du personnage
   */
	void switchState(BodyState state);

	/*
   * @brief attaque ?
   */
	bool isLetal();

	Sprite *getBody();
	Sprite *getLegs();
};

#endif
