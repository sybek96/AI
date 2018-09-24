#pragma once
#include <SFML/Graphics.hpp>
#include "Window.h"
#include "KeyHandler.h"
///
/// Player class is the player that is controlled by the user
/// 
class Player
{
public:
	//Default Constructor
	Player(KeyHandler& kh);
	//overloaded constructor
	Player(sf::Texture& texture, KeyHandler& m_keyHandler);

	//Default Destructor
	~Player();
	//Update player logic
	void update(float dt);
	//Draw the player
	void draw(sf::RenderWindow & window);
	//Getter for position
	sf::Vector2f getPos();
	//Setter for position
	void setPos(sf::Vector2f pos);
	//method to alter the velocity by passed value
	void changeVel(sf::Vector2f vel);
	//getter for width of the sprite
	float getWidth();
	//getter for the height of the sprite
	float getHeight();
	//border collision method
	void borderCollision();
	//handles the input
	void handleInput();
	//increase speed method
	void increaseSpeed();
	//decrease speed method
	void decreaseSpeed();
	//rotating right
	void rotateRight();
	//rotating left
	void rotateLeft();
	//this will calculate the movement to be done
	void move(float dt);
private:
	//velocity
	sf::Vector2f m_velocity;
	//sprite
	sf::Sprite m_sprite;
	//position
	sf::Vector2f m_position;
	//reference to the key handler
	KeyHandler& m_keyHandler;
	//max velocity of the sprite
	static const float s_MAX_SPEED;
	//angle in which sprite is turning
	float m_rotation;
	//speed at which the sprite is moving
	float m_speed;
	//value of pi
	static const float s_PI;
	//maximum amount to rotate by
	static const float s_MAX_ROTATION;
};

