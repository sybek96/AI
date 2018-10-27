#include "Player.h"

const float Player::s_MAX_SPEED = 30.0f;
const float Player::s_PI = 3.14159;
const float Player::s_MAX_ROTATION = 0.1f;
/// <summary>
/// Default constructor
/// 
/// 
/// </summary>
Player::Player(KeyHandler& kh)
	: m_keyHandler(kh)
{
}

/// <summary>
/// Constructor taking in the texture from texture holder
/// 
/// 
/// </summary>
Player::Player(sf::Texture& texture, KeyHandler& keyHandler)
	: m_velocity(0.0f, 0.0f)
	, m_position(400.0f, 300.0f)
	, m_keyHandler(keyHandler)
	, m_rotation(0.0f)
	, m_speed(0.0f)
{
	m_sprite.setTexture(texture);
	m_sprite.setPosition(m_position);
	m_sprite.setOrigin(m_sprite.getTextureRect().width / 2, m_sprite.getTextureRect().height / 2);
	m_sprite.setScale(0.3f, 0.3f);
}

/// <summary>
/// Destructor
/// 
/// 
/// </summary>
Player::~Player()
{
}

/// <summary>
/// Update the player logic
/// 
/// 
/// </summary>
void Player::update(float dt)
{
	handleInput();
	borderCollision();
	m_sprite.setPosition(m_position.x, m_position.y);
	m_sprite.setRotation(m_rotation + 90);
	move(dt);
}

/// <summary>
/// Draw the player
/// 
/// 
/// </summary>
/// <param name="window">reference to th window object</param>
void Player::draw(sf::RenderWindow & window)
{
	if (m_sprite.getTexture())
	{
		window.draw(m_sprite);
	}
}

/// <summary>
/// Getter for the position of the circle
/// 
/// 
/// </summary>
/// <returns>vector representing the position of the player</returns>
sf::Vector2f Player::getPos()
{ 
	return m_position;
}

/// <summary>
/// Setter for the position of the circle
/// 
/// 
/// </summary>
/// <param name="pos">new position of the circle as a vector2f</param>
void Player::setPos(sf::Vector2f pos)
{
	m_position = pos;
}

/// <summary>
/// This method will change the current velocity of the player by the passed value
/// 
/// 
/// </summary>
/// <param name="vel">amount to change velocity by in a vector2f</param>
void Player::changeVel(sf::Vector2f vel)
{
	if (std::sqrt((m_velocity.x * m_velocity.x) + (m_velocity.y * m_velocity.y)) < s_MAX_SPEED)
	{
		m_velocity += vel;
	}
}

sf::Vector2f Player::getVel()
{
	return m_velocity;
}

/// <summary>
/// Get the width of the sprite
/// </summary>
/// <returns>width of intRect of sprite</returns>
float Player::getWidth()
{
	return m_sprite.getGlobalBounds().width;
}

/// <summary>
/// Get the height of the sprite
/// </summary>
/// <returns>height of the intrect of the sprite</returns>
float Player::getHeight()
{
	return m_sprite.getGlobalBounds().height;
}

/// <summary>
/// Detect from which side the player left the screen and move them where they should be in a wrap-around world
/// </summary>
void Player::borderCollision()
{
	if (m_position.x - (getWidth() /2) > Window::WIDTH) //if reach right border (divide widths and heights by 2 because origin is in centre of rect)
	{
		setPos(sf::Vector2f(-(getWidth()/2), m_position.y));
	}
	else if (m_position.x + (getWidth()/2) < 0) //if reach left border
	{
		setPos(sf::Vector2f(Window::WIDTH + (getWidth()/2), m_position.y));
	}

	if (m_position.y - (getHeight()/2) > Window::HEIGHT) //if reach bottom
	{
		setPos(sf::Vector2f(m_position.x, -getHeight()));
	}
	else if (m_position.y + getHeight() < 0) //if reach top
	{
		setPos(sf::Vector2f(m_position.x, Window::HEIGHT + (getHeight()/2)));
	}
}

/// <summary>
/// This method will handle the players input
/// 
/// </summary>
void Player::handleInput()
{
	if (m_keyHandler.isPressed(sf::Keyboard::Up))
	{

		increaseSpeed();
	}
	if (m_keyHandler.isPressed(sf::Keyboard::Down))
	{
		decreaseSpeed();
	}
	if (m_keyHandler.isPressed(sf::Keyboard::Left))
	{
		rotateLeft();
	}
	if (m_keyHandler.isPressed(sf::Keyboard::Right))
	{
		rotateRight();
	}
}

/// <summary>
/// This will cause speed to go up but not above the max
/// </summary>
void Player::increaseSpeed()
{
	if (m_speed < s_MAX_SPEED)
	{
		m_speed += 0.01f;
	}
}

/// <summary>
/// will decrease speed but not below zero
/// </summary>
void Player::decreaseSpeed()
{
	if (m_speed > 0)
	{
		m_speed -= 0.01f;
	}
}

/// <summary>
/// Rotates right by 0.1 degrees
/// </summary>
void Player::rotateRight()
{
	m_rotation += 0.1f;
	if (m_rotation >= 360)
	{
		m_rotation = 0;
	}
}

/// <summary>
/// Rotates left by 0.1 degrees
/// </summary>
void Player::rotateLeft()
{
	m_rotation -= 0.1f;
	if(m_rotation < 0)
	{
		m_rotation = 360;
	}
}

float Player::getOrientation()
{
	return m_rotation;
}

/// <summary>
/// moves the ship in a direction based on velocity and angle
/// </summary>
/// <param name="dt">time since last update</param>
void Player::move(float dt)
{
	float angle = m_rotation * (s_PI / 180);
	sf::Vector2f direction(cos(angle), sin(angle));
	m_velocity = m_speed * direction;
	m_position += m_velocity * dt;
}




