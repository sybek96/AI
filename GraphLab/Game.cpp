#include "Game.h"

// Updates per milliseconds
static double const S_PER_UPDATE = 0.01;

/// <summary>
/// Default constructor
/// 
/// 
/// </summary>
Game::Game() :
	m_window(sf::VideoMode(750, 850), "AI lab")
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

	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 50; j++)
		{
			auto rect = sf::RectangleShape(sf::Vector2f(14, 14));
			//rect.setOrigin(sf::Vector2f(rect.getGlobalBounds().width / 2, rect.getGlobalBounds().height / 2));
			rect.setPosition(sf::Vector2f(15 * j, 15 * i));
			//gridRects.push_back(rect);
			gameGrid.push_back(GameCell{ GridCell{15 * j ,15 * i}, Game::cellState::FREE, rect, sf::Color::White, static_cast<int>(rect.getPosition().y * WORLD_SIZE + rect.getPosition().x)
				});
		}
	}
	for (int i = 0; i < gameGrid.size(); i++)
	{
		m_pathFinding.addCell(&gameGrid[i].cell); //add address of cell to the pathfinder
	}	
	m_pathFinding.updateStart(&gameGrid.begin()->cell);
	m_pathFinding.updateGoal(&(gameGrid.end() - 1)->cell);
	//m_pathFinding.updateStartAndGoal(&gameGrid.begin()->cell, &(gameGrid.end() - 1)->cell);

	//gameGrid.at(1).rect.setFillColor(sf::Color::Green);
	//gameGrid.at(8).rect.setFillColor(sf::Color::Red);
	//m_pathFinding.SetStartAndGoal(gameGrid.begin()->cell, (gameGrid.end() - 1)->cell);
	//for (int i = 0; i < gameGrid.size(); i++)
	//{
	//	m_pathFinding.m_openList.push_back(&gameGrid.at(i).cell);
	//}
	//m_pathFinding.FindPath(Vector2D(40, 14), Vector2D(60, 60));
	for (auto & grid : gameGrid)
	{
		if (grid.cell.isStart)
		{
			grid.originalColor = sf::Color::Green;
		}
		if (grid.cell.isGoal)
		{
			grid.originalColor = sf::Color::Red;
		}
		grid.rect.setFillColor(grid.originalColor);
	}
	m_pathFinding.createAdjecancySets();
	m_pathFinding.wavefrontAlgorithm();
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
			case sf::Event::MouseButtonReleased:
				if (event.key.code == sf::Mouse::Button::Left)
				{
					m_mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_window));
					for (auto & cell : gameGrid)
					{
						if (m_mousePos.x >= cell.rect.getPosition().x &&         // right of the left edge AND
							m_mousePos.x <= cell.rect.getPosition().x + cell.rect.getGlobalBounds().width &&    // left of the right edge AND
							m_mousePos.y >= cell.rect.getPosition().y &&         // below the top AND
							m_mousePos.y <= cell.rect.getPosition().y + cell.rect.getGlobalBounds().height)
						{    // above the bottom
							m_pathFinding.updateGoal(&cell.cell);
						}
						//for (auto & grid : gameGrid)
						//{
						//	if (grid.cell.isGoal)
						//	{
						//		grid.originalColor = sf::Color::Red;
						//	}
						//	if (grid.cell.isStart)
						//	{
						//		grid.originalColor = sf::Color::Green;
						//	}
						//}
					}
					for (auto & grid : gameGrid)
					{
						if (grid.cell.isGoal)
						{
							grid.originalColor = sf::Color::Red;
						}
						else if (grid.cell.isStart)
						{
							grid.originalColor = sf::Color::Green;
						}
						else
						{
							grid.originalColor = sf::Color::White;
						}
						grid.rect.setFillColor(grid.originalColor);
					}
				}
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
	//m_mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_window));
	//for (auto & grid : gameGrid)
	//{
	//	//if (m_mousePos.x >= cell.rect.getPosition().x &&         // right of the left edge AND
	//	//	m_mousePos.x <= cell.rect.getPosition().x + cell.rect.getGlobalBounds().width &&    // left of the right edge AND
	//	//	m_mousePos.y >= cell.rect.getPosition().y &&         // below the top AND
	//	//	m_mousePos.y <= cell.rect.getPosition().y + cell.rect.getGlobalBounds().height) 
	//	//{    // above the bottom
	//	//	cell.rect.setFillColor(sf::Color( 200,  200,  200,  255));
	//	//}
	//	//else
	//	//{
	//	//	cell.rect.setFillColor(cell.originalColor);
	//	//}
	//	if (grid.cell.isGoal)
	//	{
	//		grid.originalColor = sf::Color::Green;
	//	}
	//	if (grid.cell.isStart)
	//	{
	//		grid.originalColor = sf::Color::Red;
	//	}
	//}
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
	for (auto & cell : gameGrid)
	{
		m_window.draw(cell.rect);
	}
	m_window.display();
}
