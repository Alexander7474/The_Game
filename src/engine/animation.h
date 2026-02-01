#pragma once 

#include <BBOP/Graphics.h>

struct Animation {
        std::vector<Texture>
            textures;               //<! ensemble de texture qui forme l'animation
        int frame = 0;              // frame actuelle
        double lastFrameTime = 0.0; // depart de la dernière frame
};
