#include "ressourceManager.h"

#include <filesystem>

std::unordered_map<std::string, Texture *> RessourceManager::textures;
std::unordered_map<std::string, Mix_Chunk *> RessourceManager::sounds;

void RessourceManager::loadAll(std::string racinePath)
{
	std::string assetsPath = racinePath + "assets/";
	for (const auto &dirEntry :
		std::filesystem::recursive_directory_iterator(assetsPath)) {
		std::string file = dirEntry.path().string();
		std::string extention = file.substr(file.size() - 4);
		if (extention == ".png") {
			textures[file] = new Texture(file.c_str());
		} else if (extention == ".wav") {
			sounds[file] = Mix_LoadWAV(file.c_str());
		} else {
			std::cerr << "Erreur: '" << extention
				  << "' non reconnue pour " << file << '\n';
		}
	}
}

Texture *RessourceManager::getTexture(std::string name)
{
	if (textures.find(name) == textures.end())
		return textures.at("default");

	return textures.at(name);
}

Mix_Chunk *RessourceManager::getSound(std::string name)
{
	if (sounds.find(name) == sounds.end())
		return sounds.at("default");

	return sounds.at(name);
}
