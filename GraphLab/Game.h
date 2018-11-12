#pragma once

#include <SFML/Graphics.hpp>
#include <time.h>
#include "KeyHandler.h"
#include "GridCell.h"
#include "PathFinding.h"

///
/// Game class is the main loop of the game that updates and draws the game
///
class Game
{
	enum class cellState
	{
		FREE,
		BLOCKED
	};

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
	struct GameCell
	{
		GridCell cell;
		cellState state = Game::cellState::FREE;
		sf::RectangleShape rect;
		sf::Color originalColor = sf::Color::White;
		int m_id;
	};
	std::vector<GameCell> gameGrid;
	std::vector<sf::RectangleShape> gridRects;
	PathFinding m_pathFinding;
};

