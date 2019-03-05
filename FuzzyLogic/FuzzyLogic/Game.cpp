#include "Game.h"

// Updates per milliseconds
static double const S_PER_UPDATE = 0.01;

/// <summary>
/// Default constructor
/// 
/// 
/// </summary>
Game::Game() :
	m_window(sf::VideoMode(800, 600), "AI lab fuzzy logic")
{
	if (!font.loadFromFile("BERNHC.TTF"))
	{
		std::cout << "could not load font" << std::endl;
	}
	m_allyText.setFont(font);
	m_enemyAmountText.setFont(font);
	m_enemyDistanceText.setFont(font);
	m_beginText.setFont(font);


	m_beginText.setString("Press spacebar to generate enemy forces");
	m_beginText.setPosition(sf::Vector2f(100.0f, 250.0f));

	m_allyText.setPosition(sf::Vector2f(30, 320));
	m_allyText.setColor(sf::Color::Green);

	m_enemyAmountText.setPosition(sf::Vector2f(500, 320));
	m_enemyAmountText.setColor(sf::Color::Red);
	m_enemyDistanceText.setPosition(sf::Vector2f(500, 350));
	m_enemyDistanceText.setColor(sf::Color::Red);

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
	if (m_keyHandler.isPressed(sf::Keyboard::Space) && !m_keyHandler.isPreviouslyPressed(sf::Keyboard::Space))
	{
		m_beginText.setString("");
		// 1 - FUZZY VARIABLES
		//generate size between 0 and 45
		int sizeOfForce = std::rand() % 46;
		//generate distance between 0 and 100
		int distance = std::rand() % 101;
		std::cout << "size of force is: " << sizeOfForce << std::endl;
		std::cout << "force is: " << distance << "m away" << std::endl;

		// 2 - FUZZIFICATION
		//THREAT ASSESSMENT - SIZE
		double mTiny = FuzzyMembership::FuzzyGradeDown(sizeOfForce, 0.0, 10.0);
		double mSmall = FuzzyMembership::FuzzyTrapezoid(sizeOfForce, 2.0, 10.0, 15.0, 20.0);
		double mModerate = FuzzyMembership::FuzzyTrapezoid(sizeOfForce, 15.0, 20.0, 25.0, 30.0);
		double mLarge = FuzzyMembership::FuzzyGradeUp(sizeOfForce, 25.0, 30.0);

		//THREAT ASSESSMENT - DISTANCE
		double mClose = FuzzyMembership::FuzzyGradeDown(distance, 0.0, 30.0);
		double mMedium = FuzzyMembership::FuzzyTrapezoid(distance, 10.0, 30.0, 50.0, 70.0);
		double mFar = FuzzyMembership::FuzzyGradeUp(distance, 50.0, 70.0);

		// 3/4 - APPLY RULE MATRIX / RULES APPLICATION
		//APPLY RULE MATRIX

		//low - (medium AND tiny) OR (mediu and small) OR (far AND (NOT large)) 
		double lowThreat = FuzzyLogic::FuzzyOR(
			FuzzyLogic::FuzzyAND(mMedium, mTiny),
			FuzzyLogic::FuzzyAND(mMedium, mSmall)
		);
		lowThreat = FuzzyLogic::FuzzyOR(
			lowThreat,
			FuzzyLogic::FuzzyAND(mFar, FuzzyLogic::FuzzyNOT(mLarge))
		);


		//medium - (close AND tiny) OR (medium AND moderate) OR (far AND large)
		double mediumThreat = FuzzyLogic::FuzzyOR(
			FuzzyLogic::FuzzyAND(mClose, mTiny),
			FuzzyLogic::FuzzyAND(mMedium, mModerate)
		);
		mediumThreat = FuzzyLogic::FuzzyOR(
			mediumThreat,
			FuzzyLogic::FuzzyAND(mFar, mLarge)
		);

		//high - (close AND (NOT tiny)) OR (large AND medium)
		double highThreat = FuzzyLogic::FuzzyOR(
			FuzzyLogic::FuzzyAND(mClose, FuzzyLogic::FuzzyNOT(mTiny)),
			FuzzyLogic::FuzzyAND(mLarge, mMedium)
		);

		// 5 - DEFUZZIFICATION
		//DEFUZZIFICATION PROCESS (where low units = 3, medium is 30 and high is 50)

		int numberToDeploy = (lowThreat * 3 + mediumThreat * 30 + highThreat * 50) / (lowThreat + mediumThreat + highThreat);
		std::cout << "nuber of units to deploy is: " << numberToDeploy << std::endl;


		//DEAL WITH DISPLAY
		m_characters.clear();
		for (int i = 0; i < sizeOfForce; i++)
		{
			m_characters.push_back(new Enemy(
				sf::Vector2f((rand() % 700) + 50, (rand() % 200) + 50),
				 sf::Vector2f(1.0f - (static_cast<float>(distance) / 100.0f), 1.0f - (static_cast<float>(distance) / 100.0f))
			));
		}
		//15 per row
		float xPos = 50;
		float yPos = 550;
		
		for (int j = 0; j < numberToDeploy; j++)
		{
			m_characters.push_back(new Ally(sf::Vector2f(xPos, yPos)));
			xPos += 50;
			if (xPos > 750)
			{
				xPos = 50;
				yPos -= 50;
			}
		}
		m_allyText.setString("Ally forces: " + std::to_string(numberToDeploy));
		m_enemyAmountText.setString("Enemies: " + std::to_string(sizeOfForce));
		m_enemyDistanceText.setString("Enemy distance: " + std::to_string(distance) + "m");

	}
	for (auto & character : m_characters)
	{
		character->update(dt);
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
	for (auto & character : m_characters)
	{
		character->draw(m_window);
	}
	m_window.draw(m_allyText);
	m_window.draw(m_enemyAmountText);
	m_window.draw(m_enemyDistanceText);
	m_window.draw(m_beginText);
	m_window.display();
}