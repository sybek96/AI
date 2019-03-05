#include "Ally.h"

Ally::Ally(sf::Vector2f position)
{
	m_texture.loadFromFile("./ally.png");
	m_texture.setSmooth(true);
	m_sprite.setTexture(m_texture);
	m_position = sf::Vector2f(position);
	m_sprite.setPosition(m_position);
	m_sprite.setOrigin(m_sprite.getGlobalBounds().width / 2, m_sprite.getGlobalBounds().height / 2);
	m_sprite.setRotation(180);
	//initial scaling for the default size of sprite
	m_sprite.scale(0.07, 0.07);
}

void Ally::update(float dt)
{
}
