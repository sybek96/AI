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
	if (!font.loadFromFile("BERNHC.TTF"))
	{
		std::cout << "could not load font" << std::endl;
	}
	if (!m_info.loadFromFile("info.png"))
	{
		std::cout << "could not load image" << std::endl;
	}
	m_infoSprite.setTexture(m_info);
	m_infoSprite.setScale(0.5f, 0.5f);
	m_infoSprite.setPosition(sf::Vector2f(0, 749));

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
	lines.setPrimitiveType(sf::LinesStrip);
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

	createObstacles();

	for (int i = 0; i < gameGrid.size(); i++)
	{
		m_pathFinding.addCell(&gameGrid[i].cell); //add address of cell to the pathfinder
	}	
	m_pathFinding.updateStart(&gameGrid.at(0).cell);
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
		grid.cell.G = 99999;
		//if (grid.cell.isStart)
		//{
		//	grid.originalColor = sf::Color::Green;
		//}
		//if (grid.cell.isGoal)
		//{
		//	grid.originalColor = sf::Color::Red;
		//}
		//if (grid.cell.blocked)
		//{
		//	grid.originalColor = sf::Color::Cyan;
		//}
		//grid.rect.setFillColor(grid.originalColor);
	}
	m_pathFinding.createAdjecancySets();
	m_pathFinding.wavefrontAlgorithm();
	createFieldVectors();
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
		else if (grid.cell.blocked)
		{
			grid.originalColor = sf::Color::Cyan;
		}
		else
		{
			grid.originalColor = sf::Color(0.0f, 0.0f, 50.0f + (grid.cell.G * 3), 255.0f);
			//grid.originalColor = sf::Color::White;
		}
		grid.rect.setFillColor(grid.originalColor);
	}
	for (auto& gameCell : gameGrid)
	{
		gameCell.costLabel.setFont(font);
		if (gameCell.cell.GetF() > 1000)
		{
			gameCell.costLabel.setString("X");
		}
		else
		{
			gameCell.costLabel.setString(std::to_string(static_cast<int>(gameCell.cell.GetF())));
		}
		gameCell.costLabel.setPosition(sf::Vector2f(gameCell.cell.m_xCoord, gameCell.cell.m_yCoord));
		gameCell.costLabel.setCharacterSize(10);
		//gameCell.costLabel.setScale(0.4, 0.4);
		gameCell.costLabel.setFillColor(sf::Color::White);
	}
	
	
	for (int i = 0; i < m_pathFinding.m_pathToGoal.size(); i++)
	{
		lines.append(sf::Vertex(sf::Vector2f(m_pathFinding.m_pathToGoal.at(i)->x, m_pathFinding.m_pathToGoal.at(i)->y), sf::Color::Red));
	}



	//fieldVectors.append(sf::Vertex(directionAsSFVector, sf::Color::Black));
	//fieldVectors.append(sf::Vertex(positionAsSFVector, sf::Color::Black));


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
				if (event.key.code == sf::Keyboard::Space)
				{
					m_mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_window));
					for (auto & cell : gameGrid)
					{
						if (m_mousePos.x >= cell.rect.getPosition().x &&         // right of the left edge AND
							m_mousePos.x <= cell.rect.getPosition().x + cell.rect.getGlobalBounds().width &&    // left of the right edge AND
							m_mousePos.y >= cell.rect.getPosition().y &&         // below the top AND
							m_mousePos.y <= cell.rect.getPosition().y + cell.rect.getGlobalBounds().height)
						{    // above the bottom
							if (cell.cell.blocked)
							{
								recreateAdjecancySet = true; //if player user makes goal on obstacle recreate adjecancy set
								cell.cell.blocked = false;
							}
							m_pathFinding.updateStart(&cell.cell);
							//Check if any obstacles were created since last left click
							//if yes we must recreate the adjecancy set
							if (recreateAdjecancySet)
							{
								m_pathFinding.createAdjecancySets();
								recreateAdjecancySet = false;
								createFieldVectors();

							}
							m_pathFinding.wavefrontAlgorithm();

						}
						lines.clear();
						for (int i = 0; i < m_pathFinding.m_pathToGoal.size(); i++)
						{
							lines.append(sf::Vertex(sf::Vector2f(m_pathFinding.m_pathToGoal.at(i)->x, m_pathFinding.m_pathToGoal.at(i)->y), sf::Color::Red));
						}
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
						else if (grid.cell.blocked)
						{
							grid.originalColor = sf::Color::Cyan;
						}
						else
						{
							grid.originalColor = sf::Color(0.0f, 0.0f, 50.0f + (grid.cell.G * 3), 255.0f);
							//grid.originalColor = sf::Color::White;
						}
						grid.rect.setFillColor(grid.originalColor);
					}
				}
				if (event.key.code == sf::Keyboard::V)
				{
					if (!isCostDisplayed && !clearBoard)
					{
						isCostDisplayed = true;
						isVectorFieldDisplayed = false;
					}
					else if (!isVectorFieldDisplayed)
					{
						isCostDisplayed = false;
						isVectorFieldDisplayed = true;
						clearBoard = true;
					}
					else
					{
						isCostDisplayed = false;
						isVectorFieldDisplayed = false;
						clearBoard = false;
					}
				}
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
							if (cell.cell.blocked)
							{
								recreateAdjecancySet = true; //if player user makes goal on obstacle recreate adjecancy set
								cell.cell.blocked = false;
							}
							m_pathFinding.updateGoal(&cell.cell);
							//Check if any obstacles were created since last left click
							//if yes we must recreate the adjecancy set
							if (recreateAdjecancySet)
							{
								m_pathFinding.createAdjecancySets();
								recreateAdjecancySet = false;
							}
							m_pathFinding.wavefrontAlgorithm();
							createFieldVectors();

							for (auto& gameCell : gameGrid)
							{
								if (gameCell.cell.GetF() > 1000)
								{
									gameCell.costLabel.setString("X");
								}
								else
								{
									gameCell.costLabel.setString(std::to_string(static_cast<int>(gameCell.cell.GetF())));
								}
							}
						}
						lines.clear();
						for (int i = 0; i < m_pathFinding.m_pathToGoal.size(); i++)
						{
							lines.append(sf::Vertex(sf::Vector2f(m_pathFinding.m_pathToGoal.at(i)->x, m_pathFinding.m_pathToGoal.at(i)->y), sf::Color::Red));
						}
					}

				}
				else if (event.key.code == sf::Mouse::Button::Right)
				{
					m_mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_window));
					for (auto & cell : gameGrid)
					{
						if (m_mousePos.x >= cell.rect.getPosition().x &&         // right of the left edge AND
							m_mousePos.x <= cell.rect.getPosition().x + cell.rect.getGlobalBounds().width &&    // left of the right edge AND
							m_mousePos.y >= cell.rect.getPosition().y &&         // below the top AND
							m_mousePos.y <= cell.rect.getPosition().y + cell.rect.getGlobalBounds().height)
						{   // above the bottom
							if (!cell.cell.blocked)
							{
								cell.cell.blocked = true;
								cell.cell.G = 9999;
								//m_pathFinding.updateGoal(&cell.cell);
								if (cell.cell.GetF() > 1000)
								{
									cell.costLabel.setString("X");
								}
								else
								{
									cell.costLabel.setString(std::to_string(static_cast<int>(cell.cell.GetF())));
								}
							}
							recreateAdjecancySet = true;
						}
					}

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
					else if (grid.cell.blocked)
					{
						grid.originalColor = sf::Color::Cyan;
					}
					else
					{
						grid.originalColor = sf::Color(0.0f, 0.0f, 50.0f + (grid.cell.G * 3), 255.0f);
						//grid.originalColor = sf::Color::White;
					}
					grid.rect.setFillColor(grid.originalColor);
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
		if (isCostDisplayed)
		{
			m_window.draw(cell.costLabel);
		}
	}

	m_window.draw(m_infoSprite);
	if (isVectorFieldDisplayed)
	{
		for (auto & strip : fieldVectors)
		{
			m_window.draw(strip);
		}
	}
	m_window.draw(lines);


	m_window.display();
}

void Game::createObstacles()
{
	//top obstacle
	gameGrid.at(1121).cell.blocked = true;
	gameGrid.at(1122).cell.blocked = true;
	gameGrid.at(1123).cell.blocked = true;
	gameGrid.at(1124).cell.blocked = true;
	gameGrid.at(1125).cell.blocked = true;
	gameGrid.at(1124).cell.blocked = true;
	gameGrid.at(1123).cell.blocked = true;
	gameGrid.at(1174).cell.blocked = true;
	gameGrid.at(1224).cell.blocked = true;
	gameGrid.at(1274).cell.blocked = true;
	gameGrid.at(1324).cell.blocked = true;
	gameGrid.at(1374).cell.blocked = true;
	gameGrid.at(1424).cell.blocked = true;

	//middle obstacle
	gameGrid.at(123).cell.blocked = true;
	gameGrid.at(124).cell.blocked = true;
	gameGrid.at(125).cell.blocked = true;
	gameGrid.at(126).cell.blocked = true;
	gameGrid.at(127).cell.blocked = true;
	gameGrid.at(128).cell.blocked = true;
	gameGrid.at(129).cell.blocked = true;
	gameGrid.at(130).cell.blocked = true;
	gameGrid.at(131).cell.blocked = true;
	gameGrid.at(174).cell.blocked = true;
	gameGrid.at(224).cell.blocked = true;
	gameGrid.at(274).cell.blocked = true;
	gameGrid.at(324).cell.blocked = true;

	//bottom obstacle
	gameGrid.at(2124).cell.blocked = true;
	gameGrid.at(2125).cell.blocked = true;
	gameGrid.at(2126).cell.blocked = true;
	gameGrid.at(2127).cell.blocked = true;
	gameGrid.at(2128).cell.blocked = true;
	gameGrid.at(2129).cell.blocked = true;
	gameGrid.at(2130).cell.blocked = true;
	gameGrid.at(2131).cell.blocked = true;
	gameGrid.at(2174).cell.blocked = true;
	gameGrid.at(2224).cell.blocked = true;
	gameGrid.at(2274).cell.blocked = true;
	gameGrid.at(2324).cell.blocked = true;
	gameGrid.at(2325).cell.blocked = true;
	gameGrid.at(2326).cell.blocked = true;
	gameGrid.at(2327).cell.blocked = true;
	gameGrid.at(2328).cell.blocked = true;	
	gameGrid.at(2278).cell.blocked = true;
	gameGrid.at(2228).cell.blocked = true;
	gameGrid.at(2178).cell.blocked = true;


}

void Game::createFieldVectors()
{
	fieldVectors.clear();
	for (int i = 0; i < m_pathFinding.m_openList.size(); i++)
	{
		sf::Vector2f directionAsSFVector = sf::Vector2f(m_pathFinding.m_openList.at(i)->m_direction.x, m_pathFinding.m_openList.at(i)->m_direction.y);
		sf::Vector2f positionAsSFVector = sf::Vector2f(m_pathFinding.m_openList.at(i)->m_xCoord + 7.5, m_pathFinding.m_openList.at(i)->m_yCoord + 7.5);
		fieldVectors.push_back(sf::VertexArray(sf::LineStrip));
		fieldVectors.at(i).append(sf::Vertex(directionAsSFVector, sf::Color::White));
		fieldVectors.at(i).append(sf::Vertex(positionAsSFVector, sf::Color::White));
	}
}
