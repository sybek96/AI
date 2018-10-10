#include "Game.h"

// Updates per milliseconds
static double const S_PER_UPDATE = 0.01;

/// <summary>
/// Default constructor
/// 
/// 
/// </summary>
Game::Game() :
	m_window(sf::VideoMode(Window::WIDTH, Window::HEIGHT), "AI lab")
{
}

/// <summary>
/// Default destructor
/// 
/// 
/// </summary>
Game::~Game()
{
}

void Game::init()
{
	srand(time(NULL));
	m_textures.load(Textures::Player, "assets/player.png");
	m_textures.load(Textures::Enemy, "assets/enemy.png");
	m_player = std::unique_ptr<Player>(new Player(m_textures.get(Textures::Player),m_keyHandler));
	//set up the 3 AI's to be in the vector and give each a different state
	//m_enemies.push_back(std::unique_ptr<Enemy>(new Enemy(m_textures.get(Textures::Enemy))));
	//m_enemies.back()->setAIState(Enemy::AI::Flee);
	m_enemies.push_back(std::unique_ptr<Enemy>(new Enemy(m_textures.get(Textures::Enemy))));
	m_enemies.back()->setAIState(Enemy::AI::Pursue);
	m_enemies.back()->setMaxSpeed(20.0f);
	m_enemies.back()->setMaxAcc(20.0f);
	//m_enemies.push_back(std::unique_ptr<Enemy>(new Enemy(m_textures.get(Textures::Enemy))));
	//m_enemies.back()->setAIState(Enemy::AI::Arrive);
	loop();
}

/// <summary>
/// The game loop, listens for key press events and handles window closing,
/// calls the update and draw methods
/// </summary>
void Game::loop()
{
	sf::Clock clock;
	sf::Int32 lag = 0;

	while (m_window.isOpen())
	{
		sf::Time dt = clock.restart();

		lag += dt.asMilliseconds();

		sf::Event event;
		while (m_window.pollEvent(event))
		{
			switch (event.type)
			{
				// window closed
			case sf::Event::Closed:
				m_window.close();
				break;

				// key pressed
			case sf::Event::KeyPressed:
				m_keyHandler.updateKey(event.key.code, true);
				break;
			case sf::Event::KeyReleased:
				m_keyHandler.updateKey(event.key.code, false);
			default:
				break;
			}
		}
		while (lag > S_PER_UPDATE)
		{
			update(S_PER_UPDATE);
			lag -= S_PER_UPDATE;
		}

		update(S_PER_UPDATE);
		draw();
	}
}

/// <summary>
/// Update the game also update the collision with the screen and wrap entities around
/// TODO: make sure the collision off the screen is handled INSIDE the object class, create
/// a static window variable public to all objects
/// 
/// 
/// </summary>
void Game::update(float dt)
{
	m_player->update(dt);
	//m_enemy->setTargetPos(m_player->getPos());
	//m_enemy->update(dt);
	for (const auto & enemy : m_enemies)
	{
		enemy->setTarget(m_player->getPos(), m_player->getVel(), m_player->getOrientation());
		enemy->update(dt);
	}
	m_keyHandler.update();
}

/// <summary>
/// Draw the game entities
/// 
/// 
/// </summary>
void Game::draw()
{
		m_window.clear();

		m_player->draw(m_window);
		//m_enemy->draw(m_window);
		for (const auto & enemy : m_enemies)
		{
			enemy->draw(m_window);
		}
		m_window.display();
}
