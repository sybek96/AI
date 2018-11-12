#include "KeyHandler.h"



KeyHandler::KeyHandler()
	: m_keyMap()
	, m_previousKeyMap()
{
}


KeyHandler::~KeyHandler()
{
}

void KeyHandler::update()
{
	if (!m_keyMap.empty())
	{
		m_previousKeyMap = m_keyMap;
	}
}

void KeyHandler::updateKey(sf::Keyboard::Key key, bool isPressed)
{
	//get iterator to the key in the map
	keyMap::const_iterator it = m_keyMap.find(key);
	if (it != m_keyMap.end())
	{
		m_previousKeyMap[key] = m_keyMap[key];
	}
	else
	{
		m_previousKeyMap[key] = !isPressed;
	}
	m_keyMap[key] = isPressed;
}

bool KeyHandler::isPressed(sf::Keyboard::Key key) const
{
	keyMap::const_iterator it = m_keyMap.find(key);
	if(it != m_keyMap.end())
	{
		return it->second;
	}

	return false;
}

bool KeyHandler::isPreviouslyPressed(sf::Keyboard::Key key) const
{
	keyMap::const_iterator it = m_previousKeyMap.find(key);
	if (it != m_previousKeyMap.end())
	{
		return it->second;
	}
	return false;
}
