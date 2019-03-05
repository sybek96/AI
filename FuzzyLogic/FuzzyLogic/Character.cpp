#include "Character.h"

Character::Character()
{
}

void Character::draw(sf::RenderWindow& window)
{
	window.draw(m_sprite);
}
