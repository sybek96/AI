#include "Enemy.h"

const float Enemy::s_MAX_SPEED = 10.0f;
const float Enemy::s_PI = 3.14159;
const float Enemy::s_MAX_ROTATION = 0.1f;

/// <summary>
/// Default constructor
/// 
/// 
/// </summary>
Enemy::Enemy()
{
}

/// <summary>
/// Constructor taking in the texture from texture holder
/// 
/// 
/// </summary>
Enemy::Enemy(sf::Texture & texture)
	: m_sprite(texture)
	, m_velocity(0.4f, 0.2f)
	, m_position(200.0f,200.0f)
	, m_orientation(0.0f)
	, m_speed(0.0f)
	, m_timeToTarget(0.25f)
	, m_distToArrive(100.0f)
{

	m_sprite.setPosition(m_position);
	m_sprite.setOrigin(m_sprite.getTextureRect().width / 2, m_sprite.getTextureRect().height / 2);
	m_sprite.setScale(0.3f, 0.3f);

}

/// <summary>
/// Default destructor
/// 
/// 
/// </summary>
Enemy::~Enemy()
{
}

/// <summary>
/// update player logic
/// 
/// 
/// </summary>
void Enemy::update(float dt)
{
	borderCollision();
	m_sprite.setPosition(m_position.x, m_position.y);
	move(dt);
	m_sprite.setRotation(atan2(m_velocity.y, m_velocity.x) * (180/ s_PI) + 90);
}

/// <summary>
/// draw the player
/// 
/// 
/// </summary>
/// <param name="window">reference to the window object</param>
void Enemy::draw(sf::RenderWindow & window)
{
	if (m_sprite.getTexture())
	{
		window.draw(m_sprite);
	}
}

/// <summary>
/// get the position of the player
/// </summary>
/// <returns>position of the player circle as a vector2f</returns>
sf::Vector2f Enemy::getPos()
{
	return m_position;
}

/// <summary>
/// set the position of the player circle based on the passed in position
/// 
/// 
/// </summary>
/// <param name="pos">vector2f representing the new position</param>
void Enemy::setPos(sf::Vector2f pos)
{
	m_position = pos;
}

/// <summary>
/// Sets the target position to be the passed in position
/// </summary>
/// <param name="targetPos">new target position as vector2f</param>
void Enemy::setTargetPos(sf::Vector2f targetPos)
{
	m_targetPos = targetPos;
}

/// <summary>
/// Change the velocity by the passed vector, 
/// adds the passed vector to the existing velocity
/// 
/// 
/// </summary>
/// <param name="vel">velocity to add as a vector2f</param>
void Enemy::changeVel(sf::Vector2f vel)
{
	m_velocity += vel;
}

/// <summary>
/// Get the width of the sprite
/// </summary>
/// <returns>width of intRect of sprite</returns>
float Enemy::getWidth()
{
	return m_sprite.getGlobalBounds().width;
}

/// <summary>
/// Get the height of the sprite
/// </summary>
/// <returns>height of the intrect of the sprite</returns>
float Enemy::getHeight()
{
	return m_sprite.getGlobalBounds().height;
}

/// <summary>
/// Detect from which side the enemy left the screen and move them where they should be in a wrap-around world
/// </summary>
void Enemy::borderCollision()
{
	if (m_position.x - (getWidth() / 2) > Window::WIDTH) //if reach right border (divide widths and heights by 2 because origin is in centre of rect)
	{
		setPos(sf::Vector2f(-(getWidth() / 2), m_position.y));
	}
	else if (m_position.x + (getWidth() / 2) < 0) //if reach left border
	{
		setPos(sf::Vector2f(Window::WIDTH + (getWidth() / 2), m_position.y));
	}

	if (m_position.y - (getHeight() / 2) > Window::HEIGHT) //if reach bottom
	{
		setPos(sf::Vector2f(m_position.x, -getHeight()));
	}
	else if (m_position.y + getHeight() < 0) //if reach top
	{
		setPos(sf::Vector2f(m_position.x, Window::HEIGHT + (getHeight() / 2)));
	}
}

/// <summary>
/// moves in a direction based on velocity and angle
/// </summary>
/// <param name="dt">time since last update</param>
void Enemy::move(float dt)
{
	switch (m_ai)
	{
	case Enemy::AI::Wander:
		wander();
		break;
	case Enemy::AI::Seek:
		seek();
		break;
	case Enemy::AI::Flee:
		flee();
		break;
	case Enemy::AI::Arrive:
		arrive();
		break;
	default:
		float angle = m_rotation * (s_PI / 180);
		sf::Vector2f direction(cos(angle), sin(angle));
		m_velocity = m_speed * direction;
		break;
	}
	m_position += m_velocity * dt;
	
}

void Enemy::wander()
{
	auto vecToTarget = m_targetPos - m_position;
	vecToTarget = normalizeVec(vecToTarget);
	m_orientation = getNewOrientation(m_orientation, vecToTarget);
	float random = rand() % 200 - 100;
	m_orientation += s_MAX_ROTATION * (random / 100);

	m_velocity = sf::Vector2f(-sin(m_orientation), cos(m_orientation)) * s_MAX_SPEED;
}

void Enemy::seek()
{
	auto vecToTarget = m_targetPos - m_position;
	if (lengthVec(vecToTarget) > m_distToArrive)
	{
		m_velocity = m_targetPos - m_position;
		m_velocity = normalizeVec(m_velocity);
		m_velocity = m_velocity * s_MAX_SPEED;
		m_orientation = getNewOrientation(m_orientation, m_velocity);
	}
	else
	{
		m_ai = AI::Arrive;
	}


}

void Enemy::flee()
{
	m_velocity =  m_position - m_targetPos;
	m_velocity = normalizeVec(m_velocity);
	m_velocity = m_velocity * s_MAX_SPEED;
	m_orientation = getNewOrientation(m_orientation, m_velocity);
}

void Enemy::arrive()
{
	m_velocity = m_targetPos - m_position;
	if (!lengthVec(m_velocity) < 90.0f)
	{
		m_velocity /= m_timeToTarget;
		if (lengthVec(m_velocity) > s_MAX_SPEED)
		{
			m_velocity = normalizeVec(m_velocity);
			m_velocity *= s_MAX_SPEED;
		}
	}
	else
	{
		m_ai = AI::Seek;
	}
}

void Enemy::setAIState(AI state)
{
	m_ai = state;
}

float Enemy::getNewOrientation(float currentOrientation, sf::Vector2f vel)
{
	if (lengthVec(vel) > 0)
	{
		return (atan2(-m_position.y, m_position.x)) * 180 / s_PI;
	}
	else
	{
		return currentOrientation;
	}
}

sf::Vector2f Enemy::normalizeVec(sf::Vector2f vec)
{
	float length = lengthVec(vec);

	if (length != 0)
	{
		vec.x = vec.x / length;
		vec.y = vec.y / length;
	}
	return vec;
}

float Enemy::lengthVec(sf::Vector2f vec)
{
	return sqrt((vec.x * vec.x) + (vec.y * vec.y));
}





