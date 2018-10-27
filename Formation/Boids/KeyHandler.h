#pragma once
#include <map>
#include <SFML/Window/Keyboard.hpp>

class KeyHandler
{
public:
	KeyHandler();
	~KeyHandler();
	void update();
	void updateKey(sf::Keyboard::Key, bool isPressed);
	bool isPressed(sf::Keyboard::Key key) const;
	bool isPreviouslyPressed(sf::Keyboard::Key key) const;
private:
	typedef std::map<sf::Keyboard::Key, bool> keyMap;
	keyMap m_keyMap;
	keyMap m_previousKeyMap;
};

