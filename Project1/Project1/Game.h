#pragma once

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Enemy.h"
#include "TextureHolder.h"
#include "Window.h"
#include "KeyHandler.h"
#include <time.h>
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
	//the player object
	std::unique_ptr<Player> m_player;
	//the enemy object
	std::unique_ptr<Enemy> m_enemy;
	//texture holder object
	TextureHolder m_textures;
	//the key handler to handle inputs
	KeyHandler m_keyHandler;


};

