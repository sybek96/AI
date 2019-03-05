#pragma once

#include <SFML/Graphics.hpp>
#include <time.h>
#include "KeyHandler.h"
#include <iostream>
#include "FuzzyMembership.h"
#include "FuzzyLogic.h"
#include "Character.h"
#include "Enemy.h"
#include "Ally.h"

///
/// Game class is the main loop of the game that updates and draws the game
///
class Game
{

public:
	//Default constructor
	Game();
	//Default destructor
	~Game();
	//function ran when the game starts
	void init();
	//the game loop method
	void loop();
	//update game logic
	void update(float dt);
	//draw the game elements
	void draw();

private:
	//the render window object
	sf::RenderWindow m_window;
	//the key handler to handle inputs
	KeyHandler m_keyHandler;
	sf::Vector2f m_mousePos;
	sf::Font font;
	std::vector<Character*> m_characters;
	sf::Text m_allyText;
	sf::Text m_enemyAmountText;
	sf::Text m_enemyDistanceText;
	sf::Text m_beginText;
};

