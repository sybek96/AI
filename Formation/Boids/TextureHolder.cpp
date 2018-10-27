#include "TextureHolder.h"


/// <summary>
/// Default constructor
/// </summary>
TextureHolder::TextureHolder()
{
}


TextureHolder::~TextureHolder()
{
}
/// <summary>
/// This function will  load the texture from file and move it into the map
/// with the ID provided
/// </summary>
/// <param name="id">id of the texture</param>
/// <param name="filepath">filepath to the texture</param>
void TextureHolder::load(Textures::ID id, const std::string & filepath)
{
	//load a texture into the local texture variable
	std::unique_ptr<sf::Texture> texture(new sf::Texture);
	if (!texture->loadFromFile(filepath))
	{
		throw std::runtime_error("TextureHolder::load - Failed to Load from" + filepath);
	}
	//move the texture into the map and assert that it inserted correctly into the map
	auto inserted = m_textureMap.insert(std::make_pair(id, std::move(texture)));
	assert(inserted.second);
}

/// <summary>
/// Gets the texture from the map and returns a reference to it
/// </summary>
/// <param name="id">id of the texture in the map</param>
/// <returns>reference to the texture requested</returns>
sf::Texture& TextureHolder::get(Textures::ID id)
{
	auto found = m_textureMap.find(id);
	assert(found != m_textureMap.end());
	return *found->second;
}

/// <summary>
/// A constant function for the texture getter
/// </summary>
/// <param name="id">id of the texture to get in the map</param>
/// <returns>constant reference to the texture object</returns>
const sf::Texture& TextureHolder::get(Textures::ID id) const
{
	auto found = m_textureMap.find(id);
	assert(found != m_textureMap.end());
	return *found->second;
}
