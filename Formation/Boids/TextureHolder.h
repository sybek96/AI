#pragma once
#include <assert.h>
#include<SFML/Graphics.hpp>
namespace Textures
{
	enum ID {Player,Enemy};
}

class TextureHolder
{
public:
	TextureHolder();
	~TextureHolder();
	void load(Textures::ID id, const std::string& filepath);
	sf::Texture& get(Textures::ID id);
	const sf::Texture& get(Textures::ID id) const;

private:
	//A map that maps the id to the corresponding texture
	std::map<Textures::ID, std::unique_ptr<sf::Texture>> m_textureMap;
};

