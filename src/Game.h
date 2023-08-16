#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include "Engine/Graphics/LTexture.h"
#include "Engine/LTimer.h"
#include "Engine/Global.h"
#include "GameObjects/BaseObject.h"
#include "GameObjects/Cloud.h"
#include "GameObjects/Planetoid.h"

class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void loadLevel();
	void resetLevel();
	void handleEvents();
	void updateLevel();
	void render();
	void postUpdate();
	void clean();

	bool running() { return is_game_running; };
	bool levelRunning() { return is_level_running; };
	bool paused() { return is_game_paused; };
	

private:
	bool is_game_running = false;
	bool is_level_running = false;
	bool is_game_paused = true;
	int current_level_id = 1;
	SDL_Window* window;
	SDL_Renderer* renderer;

	// Environment
	SDL_Color environmentColor = { 0, 153, 153 };

	// Particles
	std::vector<Cloud> dust_clouds;
	std::vector<Cloud> gas_clouds;
	std::vector<Planetoid> planetoids;
	int planetoid_id = 0;
	int stage = 0;
	float max_planetoid_mass = 0;
	float max_planetoid_radius = 0;
	float min_earth_temperature = 0;

	//Types
	int comets_count = 0;
	int planetoids_count = 0;
	int dwarf_planets_count = 0;
	int rocky_earths_count = 0;
	int icy_earths_count = 0;
	int ocean_earths_count = 0;
	int hot_earths_count = 0;
	int gas_subgiants_count = 0;
	int gas_giants_count = 0;

	//Frame rate
	LTimer systemTimer; //The frames per second timer
	LTimer deltaTimer; //The frames per second cap timer
	int countedFrames = 0; //total frames
	float avgFPS = 0;
	int FPS = 0;
	int frameTicks = 0;

	// Info panel
	TTF_Font* font = nullptr;
	int fontSize = 24;
	SDL_Color textColor = { 255, 255, 255 };
	int margin = 20;
	int line_spacing = 36;

	//Textures
	LTexture gDustCloudTexture;
	LTexture gGasCloudTexture;
	LTexture gLightenedGasCloudTexture;
	LTexture gProtostarTexture;
	LTexture gStarTexture;
	LTexture gPlanet10DwarfTexture;
	LTexture gPlanet15EarthRockyTexture;
	LTexture gPlanet15EarthIcyTexture;
	LTexture gPlanet15EarthOceanTexture;
	LTexture gPlanet15EarthHotTexture;
	LTexture gPlanet25SubGiantTexture;
	LTexture gPlanet25SubGiantHotTexture;
	LTexture gPlanet35SubGiantTexture;
	LTexture gPlanet35SubGiantHotTexture;
	LTexture gPlanet45GiantTexture;
	LTexture gPlanet55GiantTexture;
	LTexture gPlanet65GiantTexture;
	LTexture gPlanet75GiantTexture;
	LTexture gPlanet85GiantTexture;
	
	// Functions
	void loadMedia();
	void frameCap();

	// Scene
	void generateClouds();

	// Helpers
	

	// UI
	bool renderUI();
};

