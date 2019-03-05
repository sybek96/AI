#ifndef _ENEMY_H
#define _ENEMY_H

#include "Character.h"
#include "SFML/Graphics.hpp"

class Enemy : public Character
{
public:
	Enemy(sf::Vector2f position, sf::Vector2f scale);
protected:
	virtual void update(float dt) override;
private:
};

#endif // !_ENEMY_H