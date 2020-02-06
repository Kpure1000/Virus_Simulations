#include"AssetManager.h"

AssetManager* AssetManager::sInstance = nullptr;
AssetManager::AssetManager()
{
	assert(sInstance == nullptr); //  if this object is exited?
	sInstance = this;
}

Texture& AssetManager::GetTexture(std::string const& filename)
{
	auto& texMap = sInstance->m_Textures;

	auto pairFound = texMap.find(filename);

	if (pairFound != texMap.end()) //  if this texture has been loaded?
	{
		return pairFound->second; 
	}
	else
	{
		auto& texture = texMap[filename]; //  load texture to Map
		texture.loadFromFile(filename);
		return texture;
	}
}

Font& AssetManager::GetFont(string const& filename)
{
	auto& fontMap = sInstance->m_Fonts;

	auto pairFound = fontMap.find(filename);

	if (pairFound != fontMap.end())
	{
		return pairFound->second;
	}
	else
	{
		auto& font = fontMap[filename];
		font.loadFromFile(filename);
		return font;
	}
}