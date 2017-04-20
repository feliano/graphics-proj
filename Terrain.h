#pragma once
#include "Grid.h"

class Terrain: public Grid{

public:
	Terrain(): Grid(){
		attachShaders("shaders/terrain.vshader","shaders/terrain.fshader");
		//addTexture("textures/ground.png");
	}

};