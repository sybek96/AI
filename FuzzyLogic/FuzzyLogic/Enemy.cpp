#include "Enemy.h"

Enemy::Enemy(sf::Vector2f position, sf::Vector2f scale)
{
	m_texture.loadFromFile("./alien.png");
	m_texture.setSmooth(true);
	m_sprite.setTexture(m_texture);
	m_position = sf::Vector2f(position);
	m_sprite.setPosition(m_position);
	m_sprite.setOrigin(m_sprite.getGlobalBounds().width / 2, m_sprite.getGlobalBounds().height / 2);
	//initial scaling for the default size of sprite
	m_sprite.scale(0.04, 0.04);
	//further scaling based on unit distance
	m_sprite.scale(scale);
}

void Enemy::update(float dt)
{
	m_sprite.rotate(0.005f);
}
