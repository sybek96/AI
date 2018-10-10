#pragma once
#include <SFML/Graphics.hpp>
#include "Window.h"
#include <iostream>

struct Steering
{
	float angularVel;
	sf::Vector2f linearVel;
};
/// <summary>
/// The enemy entity class
/// 
/// 
/// </summary>
class Enemy
{

public:
	enum class AI {Wander,Seek,Flee,Arrive};
	//default constructor
	Enemy();
	//overloaded constructor
	Enemy(sf::Texture& texture);
	//destructor
	~Enemy();
	//update enemy logic
	void update(float dt);
	//draw the entity
	void draw(sf::RenderWindow & window);
	//getter for position
	sf::Vector2f getPos();
	//setter for position
	void setPos(sf::Vector2f pos);
	//setter for target position
	void setTargetPos(sf::Vector2f targetPos);
	//change velocity by passed value
	void changeVel(sf::Vector2f vel);
	//getter for width of the sprite
	float getWidth();
	//getter for the height of the sprite
	float getHeight();
	//border collision method
	void borderCollision();
	//cause movement
	void move(float dt);
	//Wander type of AI logic
	void wander();
	//Seek type of AI logic
	void seek();
	//Flee type of AI logic
	void flee();
	//Arrive at target AI logic
	void arrive();
	//set the ai state to be different
	void setAIState(AI state);
	//gets new orientation
	float getNewOrientation(float currentOrientation, sf::Vector2f vel);
	sf::Vector2f normalizeVec(sf::Vector2f vec);
	float lengthVec(sf::Vector2f vec);
	void setVectorAngle(sf::Vector2f& vector, float radians);
	Steering getWanderSteering();
private:
	//velocity
	sf::Vector2f m_velocity;
	//sprite
	sf::Sprite m_sprite;
	//position
	sf::Vector2f m_position;
	//max velocity of the sprite
	static const float s_MAX_SPEED;
	//angle in which sprite is facing
	float m_rotation;
	//angle in which sprite is facing
	float m_orientation;
	//speed at which the sprite is moving
	float m_speed;
	//value of pi
	static const float s_PI;
	//maximum rotation amount
	static const float s_MAX_ROTATION;

	static const float s_DEG_TO_RAD;
	static const float s_RAD_TO_DEG;

	//the ai type
	AI m_ai;
	//target position of the AI
	sf::Vector2f m_targetPos;
	//time to reach the target
	float m_timeToTarget;
	float m_distToArrive;
	float m_wanderAngle;

	float m_wanderOffset;
	float m_wanderRadius;
	float m_wanderRate;
	float m_wanderOrientation;
	float m_maxAcc;
	Steering m_steering;
};

