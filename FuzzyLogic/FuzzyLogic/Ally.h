#ifndef _ALLY_H
#define _ALLY_H

#include "Character.h"
#include "SFML/Graphics.hpp"

class Ally : public Character
{
public:
	Ally(sf::Vector2f position);
protected:
	virtual void update(float dt) override;
private:
};

#endif // !_ALLY_H