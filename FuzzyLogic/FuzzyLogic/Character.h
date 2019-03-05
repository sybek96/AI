#ifndef _CHARACTER_H
#define _CHARACTER_H

#include <SFML/Graphics.hpp>

class Character
{
public: // Constructors/Destructor/Assignments
	Character();
	Character(Character const &) = default;
	Character(Character &&) = default;

	~Character() = default;

	Character & operator=(Character const &) = default;
	Character & operator=(Character &&) = default;

public:
	virtual void update(float dt) = 0;
	void draw(sf::RenderWindow& window);

protected: 
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	float m_size;
	sf::Vector2f m_position;

private:

};

#endif // !_CHARACTER_H
