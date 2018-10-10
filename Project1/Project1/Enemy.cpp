#include "Enemy.h"

const float Enemy::s_PI = 3.14159;
const float Enemy::s_MAX_ROTATION = 1.0f;
const float Enemy::s_DEG_TO_RAD = Enemy::s_PI / 180;
const float Enemy::s_RAD_TO_DEG = 180 / Enemy::s_PI;

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
	, m_position(200.0f, 200.0f)
	, m_orientation(0.0f)
	, m_speed(0.0f)
	, m_timeToTarget(0.25f)
	, m_distToArrive(100.0f)
	, m_distToSlow(200.0f)
	, m_wanderAngle(0.0f)
	, m_wanderOffset(100.0f)
	, m_wanderRadius(25.0f)
	, m_wanderRate(2.0f)
	, m_wanderOrientation(0.0f)
	, m_maxAcc(1.0f)
	, m_rotation(0.0f)
	, m_maxSpeed(6.0f)
	, m_maxTimePrediction(10.0f)
{
	m_steering.angularVel = 0;
	m_steering.linearVel = sf::Vector2f(0.0f,0.0f);
	m_sprite.setPosition(m_position);
	m_sprite.setOrigin(m_sprite.getTextureRect().width / 2, m_sprite.getTextureRect().height / 2);
	m_sprite.setScale(0.2f, 0.2f);
	m_targetCircle.setFillColor(sf::Color::Red);
	m_targetCircle.setRadius(10.0f);
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
	m_targetCircle.setPosition(m_targetPos);

	m_sprite.setRotation(m_orientation + 180);
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
	window.draw(m_targetCircle);
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
void Enemy::setTarget(sf::Vector2f targetPos, sf::Vector2f targetVel, float orientation)
{
	m_targetPos = targetPos;
	m_targetVel = targetVel;
	m_targetOrientation = orientation;
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
	case Enemy::AI::Pursue:
		pursue();
		break;
	default:
		float angle = m_rotation * (s_PI / 180);
		sf::Vector2f direction(cos(angle), sin(angle));
		m_velocity = m_speed * direction;
		break;
	}


	//m_position += m_velocity * dt;
	m_velocity = m_velocity + m_steering.linearVel * dt;
	m_rotation = m_rotation + m_steering.angularVel * dt;
	if (lengthVec(m_velocity) > m_maxSpeed)
	{
		m_velocity = normalizeVec(m_velocity);
		m_velocity = m_velocity * m_maxSpeed;
	}
	if (m_rotation > s_MAX_ROTATION)
	{
		m_rotation = s_MAX_ROTATION;
	}
	else if (-m_rotation > s_MAX_ROTATION)
	{
		m_rotation = -s_MAX_ROTATION;
	}
	m_position = m_position + m_velocity * dt;
	m_orientation = m_orientation + m_rotation * dt;
    m_orientation = getNewOrientation(m_orientation, m_velocity);
}

void Enemy::wander()
{
	//m_velocity = m_targetPos - m_position;
	//m_velocity = normalizeVec(m_velocity);
	//m_orientation = getNewOrientation(m_orientation, m_velocity);
	//float random = rand() % 200 - 100;
	//std::cout << random / 100 << std::endl;
	//m_orientation += s_MAX_ROTATION * (random / 100);
	
	//m_velocity = sf::Vector2f(-sin(m_orientation * (s_PI / 180)), cos(m_orientation * (s_PI / 180))) * s_MAX_SPEED;
	//m_velocity = m_targetPos - m_position;


	//This is my version of wander
	//auto circleCenterVector = normalizeVec(m_velocity) * 50.0f;
	//auto displacementVector = sf::Vector2f(0, -1) * 1.0f;
	//float random = rand() % 20 - 10;
	//setVectorAngle(displacementVector, m_wanderAngle);
	//m_wanderAngle += (random * s_MAX_ROTATION) - (s_MAX_ROTATION * 0.5f);
	//auto wanderForce = circleCenterVector + displacementVector;
	//m_velocity = normalizeVec(wanderForce + m_velocity) * s_MAX_SPEED;
	//m_orientation = getNewOrientation(m_orientation, m_velocity);

	m_steering = getWanderSteering();
	//m_velocity = normalizeVec( m_velocity + m_steering.linearVel) * s_MAX_SPEED;
	//m_orientation = getNewOrientation(m_orientation, m_velocity);
}

void Enemy::seek()
{
	//auto vecToTarget = m_targetPos - m_position;
	//if (lengthVec(vecToTarget) > m_distToArrive)
	//{
	//	m_velocity = m_targetPos - m_position;
	//	m_velocity = normalizeVec(m_velocity);
	//	m_velocity = m_velocity * s_MAX_SPEED;
	//	m_orientation = getNewOrientation(m_orientation, m_velocity);
	//}
	//else
	//{
	//	m_ai = AI::Arrive;
	//}
	m_steering.linearVel = m_targetPos - m_position;
	m_velocity = normalizeVec(m_steering.linearVel);
	m_steering.linearVel *= m_maxAcc;
	m_steering.angularVel = 0;

}

void Enemy::flee()
{
	m_steering.linearVel = m_position - m_targetPos;
	m_velocity = normalizeVec(m_steering.linearVel);
	m_steering.linearVel *= m_maxAcc;
	m_steering.angularVel = 0;
}

void Enemy::arrive()
{
	float targetSpeed = 0;
	sf::Vector2f direction = m_targetPos - m_position;
	float distance = lengthVec(direction);
	//set speed
	if (distance < m_distToArrive)
	{
		float targetSpeed = 0.0f;
	}
	else if (distance > m_distToSlow)
	{
		targetSpeed = m_maxSpeed;
	}
	else
	{
		targetSpeed = m_maxSpeed * (distance / m_distToSlow);
	}
	//now set the velocity (speed and direction)
	sf::Vector2f targetVelocity = direction;
	targetVelocity = normalizeVec(targetVelocity);
	targetVelocity *= targetSpeed;

	float timeToTarget = 0.1f;

	m_steering.linearVel = targetVelocity - m_velocity;
	m_steering.linearVel /= timeToTarget;
	//Check if too fast
	if (lengthVec(m_steering.linearVel) > m_maxAcc)
	{
		m_steering.linearVel = normalizeVec(m_steering.linearVel);
		m_steering.linearVel *= m_maxAcc;
	}
	m_steering.angularVel = 0;
}

/// <summary>
/// Pursue behaviour of the AI
/// 
/// </summary>
void Enemy::pursue()
{
	sf::Vector2f direction = m_targetPos - m_position;
	float distance = lengthVec(direction);
	float speed = lengthVec(m_velocity);
	float timePrediction = 0;
	if (speed <= distance / m_maxTimePrediction)
	{
		timePrediction = m_maxTimePrediction;
	}
	else
	{
		timePrediction = distance / speed;
	}
	m_targetPos = m_targetPos + m_targetVel * timePrediction;
	seek();
}

void Enemy::setAIState(AI state)
{
	m_ai = state;
}

float Enemy::getNewOrientation(float currentOrientation, sf::Vector2f vel)
{
	if (lengthVec(vel) > 0)
	{
		return (atan2(-vel.x, vel.y)) * (180 / s_PI);
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

void Enemy::setVectorAngle(sf::Vector2f& vector, float radians)
{
	float len = lengthVec(vector);
	vector.x = cos(radians) * len;
	vector.y = sin(radians) * len;
}

Steering Enemy::getWanderSteering()
{
	struct Steering newSteering;

	float random = rand() % 200 - 100;
	random /= 100;
	if (random >= 0)
	{
		random = 1;
	}
	else
	{
		random = -1;
	}
	m_wanderOrientation += (random * m_wanderRate);
	auto targetOrientation = m_wanderOrientation + m_orientation;

	auto target = m_position + (m_wanderOffset * sf::Vector2f(std::cos(m_orientation * s_DEG_TO_RAD), std::sin(m_orientation * s_DEG_TO_RAD)));
	target += m_wanderRadius * sf::Vector2f(std::cos(targetOrientation * s_DEG_TO_RAD), std::sin(targetOrientation * s_DEG_TO_RAD));
	m_orientation = getNewOrientation(m_orientation, target);
	newSteering.linearVel = m_maxAcc * sf::Vector2f(std::cos(m_orientation * s_DEG_TO_RAD), std::sin(m_orientation * s_DEG_TO_RAD));
	return newSteering;
	
	
	//auto circleCenter = m_velocity;
	//normalizeVec(circleCenter);
	//circleCenter *= m_wanderOffset;

	//auto displacement = sf::Vector2f(0, -1);
	//displacement *= m_wanderRadius;

	//setVectorAngle(displacement, m_wanderAngle);

	//float random = rand() % 200 - 100;
	//random /= 100;
	//m_wanderAngle += (random * s_MAX_ROTATION) * (s_MAX_ROTATION * .5f);
	//std::cout << m_wanderAngle << std::endl;
	//auto wanderForce = circleCenter + displacement;
	//return wanderForce;
}


void Enemy::setMaxSpeed(float newMaxSpeed)
{
	m_maxSpeed = newMaxSpeed;
}

void Enemy::setMaxAcc(float newMaxAcc)
{
	m_maxAcc = newMaxAcc;
}





