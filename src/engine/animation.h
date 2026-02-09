#pragma once

#include <BBOP/Graphics.h>

struct Animation {
	std::vector<Texture>
		textures; //<! ensemble de texture qui forme l'animation
	int frame = 0;	  // frame actuelle
	double lastFrameTime = 0.0; // depart de la dernière frame
	// TODO -- pk j'ai utilisé std::pair et pas glm::vec2 ?
	std::pair<int, int> letalRange = {
		-1, -1}; // range de frame letal de l'anim
	std::pair<int, int> knockoutRange = {
		-1, -1}; // range de frame knockout de l'anim
};
