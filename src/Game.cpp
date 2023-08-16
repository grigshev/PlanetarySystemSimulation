#include "Game.h"

Game::Game()
{
	//walls.reserve(12000);
}

Game::~Game()
{

}

// Game subsystem initialization
void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	is_game_running = false;

	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	// Initializing SDL subsystem
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cerr << "SDL initialization failed!" << std::endl;
		return;
	}
	std::cout << "SDL initialized" << std::endl;

	window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
	if (window == nullptr)
	{
		std::cerr << "SDL window creation failed!" << std::endl;
		return;
	}
	std::cout << "Window created" << std::endl;

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == nullptr)
	{
		std::cerr << "SDL renderer creation failed!" << std::endl;
		return;
	}
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	std::cout << "Renderer created" << std::endl;

	// Initializing SDL Fonts
	if (TTF_Init() == -1)
	{
		std::cerr << "SDL_TTF initialization failed!" << std::endl;
		return;
	}
	std::cout << "SDL_TTF initialized" << std::endl;

	font = TTF_OpenFont("assets/fonts/arial.ttf", fontSize);
	if (font == nullptr)
	{
		std::cerr << "Failed to load font!" << std::endl;
		return;
	}

	// Load Media
	loadMedia();

	// Game initial settings
	is_game_running = true;
}

// Game level setting up and run (during one game session, you can run different levels several times)
void Game::loadLevel()
{
	// Create scene
	stage = 1;
	generateClouds();

	std::cout << "Scene created" << std::endl;

	systemTimer.tick();

	is_level_running = true;
	is_game_paused = false;

	std::cout << "Rendering initial frame..." << std::endl;
	render();

	std::cout << "Game start..." << std::endl;
}

// Stop and clean running level
void Game::resetLevel()
{
	stage = 0;
	dust_clouds.clear();
	gas_clouds.clear();
	planetoids.clear();

	std::cout << "Level cleaned" << std::endl;
}

// Handling game events such as key presses
void Game::handleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	if (event.type == SDL_KEYDOWN /* && !is_level_running */ )
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_q:
			//is_game_paused = !is_game_paused;
			break;
			/*
		case SDLK_w:
			//
			break;
		case SDLK_e:
			//
			break;
			*/
		default:
			break;
		}
	}

	if (event.type == SDL_QUIT)
	{
		is_game_running = false;
	}
}

// Update game logic
void Game::updateLevel()
{	

	// Update time
	deltaTimer.tick();

	double system_time = systemTimer.getCurrentTime();

	// Counters
	comets_count = 0;
	planetoids_count = 0;
	dwarf_planets_count = 0;
	rocky_earths_count = 0;
	icy_earths_count = 0;
	ocean_earths_count = 0;
	hot_earths_count = 0;
	gas_subgiants_count = 0;
	gas_giants_count = 0;

	max_planetoid_mass = 0.f;
	max_planetoid_radius = 0.f;
	min_earth_temperature = 2500.f;

	if (stage == 1 && system_time > 5)
	{
		stage = 2;
	}
	
	for (auto it = dust_clouds.begin(); it != dust_clouds.end(); ++it)
	{
		it->update(stage);
	}

	if (dust_clouds.size() > 0)
	{
		int dust_condensation_chance = 500 * dust_clouds.size() / DUST_CLOUDS_NUMBER;
		dust_condensation_chance = (dust_condensation_chance > 100) ? dust_condensation_chance : 100;
		if (rand() % dust_condensation_chance == 0)
		{
			int randomIndex = rand() % dust_clouds.size();
			dust_clouds[randomIndex].setStatus(Statuses::FORMED_PLANETOID);
		}
		//int dust_scattering_chance = 
	}

	if (gas_clouds.size() > 0)
	{
		int gas_condensation_chance = 500 * gas_clouds.size() / GAS_CLOUDS_NUMBER;
		gas_condensation_chance = (gas_condensation_chance > 100) ? gas_condensation_chance : 100;
		if (rand() % gas_condensation_chance == 0)
		{
			int randomIndex = rand() % gas_clouds.size();
			gas_clouds[randomIndex].setStatus(Statuses::FORMED_COMET);
		}
		//int dust_scattering_chance = 
	}

	for (auto it = gas_clouds.begin(); it != gas_clouds.end(); ++it)
	{
		it->update(stage);
	}

	for (Planetoid& planetoid : planetoids)
	{
		float distance_to_star = getDistance(planetoid.getPosition(), STAR_POS);

		if (planetoid.getMass() > max_planetoid_mass)
		{
			max_planetoid_mass = planetoid.getMass();
		}

		if (planetoid.getRadius() > max_planetoid_radius)
		{
			max_planetoid_radius = planetoid.getRadius();
		}

		if ((planetoid.getType() == PlanetoidTypes::HOT_EARTH ||
			planetoid.getType() == PlanetoidTypes::ROCKY_EARTH ||
			planetoid.getType() == PlanetoidTypes::OCEAN_EARTH ||
			planetoid.getType() == PlanetoidTypes::ICY_EARTH) && planetoid.getTemperature() < min_earth_temperature)
		{
			min_earth_temperature = planetoid.getTemperature();
		}

		if (planetoid.getType() == PlanetoidTypes::COMET)
		{
			comets_count++;
		}
		else if (planetoid.getType() == PlanetoidTypes::ASTEROID)
		{
			planetoids_count++;
		}
		else if (planetoid.getType() == PlanetoidTypes::DWARF_PLANET)
		{
			dwarf_planets_count++;
		}
		else if (planetoid.getType() == PlanetoidTypes::ROCKY_EARTH)
		{
			rocky_earths_count++;
		}
		else if (planetoid.getType() == PlanetoidTypes::ICY_EARTH)
		{
			icy_earths_count++;
		}
		else if (planetoid.getType() == PlanetoidTypes::OCEAN_EARTH)
		{
			ocean_earths_count++;
		}
		else if (planetoid.getType() == PlanetoidTypes::HOT_EARTH)
		{
			hot_earths_count++;
		}
		else if (planetoid.getType() == PlanetoidTypes::GAS_SUBGIANT_25 ||
			planetoid.getType() == PlanetoidTypes::HOT_GAS_SUBGIANT_25 ||
			planetoid.getType() == PlanetoidTypes::GAS_SUBGIANT_35 || 
			planetoid.getType() == PlanetoidTypes::HOT_GAS_SUBGIANT_35)
		{
			gas_subgiants_count++;
		}
		else if (planetoid.getType() == PlanetoidTypes::GAS_GIANT_45 ||
			planetoid.getType() == PlanetoidTypes::GAS_GIANT_55 ||
			planetoid.getType() == PlanetoidTypes::GAS_GIANT_65 ||
			planetoid.getType() == PlanetoidTypes::GAS_GIANT_75 ||
			planetoid.getType() == PlanetoidTypes::GAS_GIANT_85)
		{
			gas_giants_count++;
		}
		else
		{
			std::cout << "Warning! Unknown planetoid type" << std::endl;
		}

		if (planetoid.destroyed())
		{
			break;
		}

		planetoid.update(stage);
		
		for (Planetoid& inner_planetoid : planetoids)
		{
			if (planetoid.destroyed() || inner_planetoid.destroyed() || planetoid.getId() == inner_planetoid.getId())
			{
				break;
			}

			if (checkCircleToCircleCollision(planetoid.getPosition().x, planetoid.getPosition().y, planetoid.getRadius(),
				inner_planetoid.getPosition().x, inner_planetoid.getPosition().y, inner_planetoid.getRadius()))
			{	
				//Calculate joined object result speed
				float result_speed_x = (planetoid.getMass() * planetoid.getSpeed().x +
					inner_planetoid.getMass() * inner_planetoid.getSpeed().x) /
					(planetoid.getMass() + inner_planetoid.getMass());

				float result_speed_y = (planetoid.getMass() * planetoid.getSpeed().y +
					inner_planetoid.getMass() * inner_planetoid.getSpeed().y) /
					(planetoid.getMass() + inner_planetoid.getMass());

				if (planetoid.getMass() >= inner_planetoid.getMass())
				{
					planetoid.setSpeed({ result_speed_x, result_speed_y });
					planetoid.addMass(inner_planetoid.getGasMass(), inner_planetoid.getDustMass());
					inner_planetoid.setStatus(Statuses::SWALLOWED);
				}
				else
				{
					inner_planetoid.setSpeed({ result_speed_x, result_speed_y });
					inner_planetoid.addMass(planetoid.getGasMass(), planetoid.getDustMass());
					planetoid.setStatus(Statuses::SWALLOWED);
				}
			}
			else if (planetoid.getMass() >= 5.0f || inner_planetoid.getMass() >= 5.0f)
			{
				float distance = getDistance(planetoid.getPosition(), inner_planetoid.getPosition());
				Vector2f pl_pos = planetoid.getPosition();
				Vector2f ipl_pos = inner_planetoid.getPosition();

				if (distance < planetoid.getGravityRadius())
				{
					Vector2f gravity_vector = getGravityVector(ipl_pos, 1.0f, pl_pos, planetoid.getMass());
					inner_planetoid.setSpeed({ inner_planetoid.getSpeed().x + 0.5f * gravity_vector.x, 
						inner_planetoid.getSpeed().y + 0.5f * gravity_vector.y });
				}

				if (distance < inner_planetoid.getGravityRadius())
				{
					Vector2f gravity_vector = getGravityVector(pl_pos, 1.0f, ipl_pos, inner_planetoid.getMass());
					planetoid.setSpeed({ planetoid.getSpeed().x + 0.5f * gravity_vector.x, 
						planetoid.getSpeed().y + 0.5f * gravity_vector.y });
				}

				
				


			}
		}

		if (planetoid.getType() != PlanetoidTypes::COMET)
		{
			for (Cloud& cloud : gas_clouds)
			{
				if (cloud.exists())
				{
					if (checkCircleToCircleCollision(planetoid.getPosition().x, planetoid.getPosition().y,
						planetoid.getSubstanceCollectionRadius(), cloud.getPosition().x, cloud.getPosition().y, cloud.getRadius()))
					{
						if (planetoid.getMass() > 5.0f)
						{

							//Calculate joined object result speed
							float result_speed_x = (planetoid.getMass() * planetoid.getSpeed().x +
								cloud.getMass() * cloud.getSpeed().x) /
								(planetoid.getMass() + cloud.getMass());

							float result_speed_y = (planetoid.getMass() * planetoid.getSpeed().y +
								cloud.getMass() * cloud.getSpeed().y) /
								(planetoid.getMass() + cloud.getMass());

							planetoid.setSpeed({ result_speed_x, result_speed_y });


							cloud.setStatus(Statuses::SWALLOWED);
							planetoid.addMass(0.f, cloud.getMass());
						}

						//planetoid.setSpeed({ CLOUD_SPEED_SLOWDOWN * planetoid.getSpeed().x, CLOUD_SPEED_SLOWDOWN * planetoid.getSpeed().y });
					}
				}


			}

			for (Cloud& cloud : dust_clouds)
			{
				if (cloud.exists())
				{
					if (checkCircleToCircleCollision(planetoid.getPosition().x, planetoid.getPosition().y,
						planetoid.getSubstanceCollectionRadius(), cloud.getPosition().x, cloud.getPosition().y, cloud.getRadius()))
					{
						//Calculate joined object result speed
						float result_speed_x = (planetoid.getMass() * planetoid.getSpeed().x +
							cloud.getMass() * cloud.getSpeed().x) /
							(planetoid.getMass() + cloud.getMass());

						float result_speed_y = (planetoid.getMass() * planetoid.getSpeed().y +
							cloud.getMass() * cloud.getSpeed().y) /
							(planetoid.getMass() + cloud.getMass());

						planetoid.setSpeed({ result_speed_x, result_speed_y });



						cloud.setStatus(Statuses::SWALLOWED);
						planetoid.addMass(0.f, cloud.getMass());
						//planetoid.setSpeed({ CLOUD_SPEED_SLOWDOWN * planetoid.getSpeed().x, CLOUD_SPEED_SLOWDOWN * planetoid.getSpeed().y });
					}
				}
			}
		}

		

	}



	for (auto it = dust_clouds.begin(); it != dust_clouds.end(); ) {
		if (it->destroyed()) {

			if (it->getStatus() == Statuses::FORMED_PLANETOID)
			{
				planetoids.emplace_back(Planetoid(it->getPosition(), it->getSpeed(), PlanetoidTypes::ASTEROID, planetoid_id));
				planetoid_id++;
			}

			it = dust_clouds.erase(it);
		}
		else {
			++it;
		}
	}

	for (auto it = gas_clouds.begin(); it != gas_clouds.end(); ) {
		if (it->destroyed()) {

			if (it->getStatus() == Statuses::FORMED_COMET)
			{
				planetoids.emplace_back(Planetoid(it->getPosition(), it->getSpeed(), PlanetoidTypes::COMET, planetoid_id));
				planetoid_id++;
			}

			it = gas_clouds.erase(it);
		}
		else {
			++it;
		}
	}

	for (auto it = planetoids.begin(); it != planetoids.end(); ) {
		if (it->destroyed()) {
			it = planetoids.erase(it);
		}
		else {
			++it;
		}
	}
}

// Render all objects
void Game::render()
{
	SDL_RenderClear(renderer);

	// Plant color
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	// Plant color
	for (const Planetoid& planetoid : planetoids)
	{
		switch (planetoid.getType())
		{
			case PlanetoidTypes::GAS_GIANT_85:
				gPlanet85GiantTexture.render(renderer, planetoid.getRenderPosition().x, planetoid.getRenderPosition().y);
				break;
			case PlanetoidTypes::GAS_GIANT_75:
				gPlanet75GiantTexture.render(renderer, planetoid.getRenderPosition().x, planetoid.getRenderPosition().y);
				break;
			case PlanetoidTypes::GAS_GIANT_65:
				gPlanet65GiantTexture.render(renderer, planetoid.getRenderPosition().x, planetoid.getRenderPosition().y);
				break;
			case PlanetoidTypes::GAS_GIANT_55:
				gPlanet55GiantTexture.render(renderer, planetoid.getRenderPosition().x, planetoid.getRenderPosition().y);
				break;
			case PlanetoidTypes::GAS_GIANT_45:
				gPlanet45GiantTexture.render(renderer, planetoid.getRenderPosition().x, planetoid.getRenderPosition().y);
				break;
			case PlanetoidTypes::GAS_SUBGIANT_35:
				gPlanet35SubGiantTexture.render(renderer, planetoid.getRenderPosition().x, planetoid.getRenderPosition().y);
				break;
			case PlanetoidTypes::HOT_GAS_SUBGIANT_35:
				gPlanet35SubGiantHotTexture.render(renderer, planetoid.getRenderPosition().x, planetoid.getRenderPosition().y);
				break;
			case PlanetoidTypes::GAS_SUBGIANT_25:
				gPlanet25SubGiantTexture.render(renderer, planetoid.getRenderPosition().x, planetoid.getRenderPosition().y);
				break;
			case PlanetoidTypes::HOT_GAS_SUBGIANT_25:
				gPlanet25SubGiantHotTexture.render(renderer, planetoid.getRenderPosition().x, planetoid.getRenderPosition().y);
				break;
			case PlanetoidTypes::ROCKY_EARTH:
				gPlanet15EarthRockyTexture.render(renderer, planetoid.getRenderPosition().x, planetoid.getRenderPosition().y);
				break;
			case PlanetoidTypes::ICY_EARTH:
				gPlanet15EarthIcyTexture.render(renderer, planetoid.getRenderPosition().x, planetoid.getRenderPosition().y);
				break;
			case PlanetoidTypes::OCEAN_EARTH:
				gPlanet15EarthOceanTexture.render(renderer, planetoid.getRenderPosition().x, planetoid.getRenderPosition().y);
				break;
			case PlanetoidTypes::HOT_EARTH:
				gPlanet15EarthHotTexture.render(renderer, planetoid.getRenderPosition().x, planetoid.getRenderPosition().y);
				break;
			case PlanetoidTypes::DWARF_PLANET:
				gPlanet10DwarfTexture.render(renderer, planetoid.getRenderPosition().x, planetoid.getRenderPosition().y);
				break;
			case PlanetoidTypes::COMET:
				SDL_SetRenderDrawColor(renderer, 255, 250, 228, 255);
				SDL_RenderDrawPoint(renderer, planetoid.getRenderPosition().x, planetoid.getRenderPosition().y);
				break;
			default:
				SDL_SetRenderDrawColor(renderer, 247, 223, 183, 255);
				SDL_RenderDrawPoint(renderer, planetoid.getRenderPosition().x, planetoid.getRenderPosition().y);
				break;
		}
	}

	for (const Cloud& cloud : dust_clouds)
	{
		gDustCloudTexture.render(renderer, cloud.getRenderPosition().x, cloud.getRenderPosition().y);
	}


	if (stage == 1)
	{
		gProtostarTexture.render(renderer, static_cast<int>(STAR_POS.x - PROTOSTAR_RADIUS), static_cast<int>(STAR_POS.y - PROTOSTAR_RADIUS));

		for (const Cloud& cloud : gas_clouds)
		{
			gGasCloudTexture.render(renderer, cloud.getRenderPosition().x, cloud.getRenderPosition().y);
		}
	}
	else
	{
		gStarTexture.render(renderer, static_cast<int>(STAR_POS.x - 24.0f), static_cast<int>(STAR_POS.y - 24.0f));

		for (const Cloud& cloud : gas_clouds)
		{
			gLightenedGasCloudTexture.render(renderer, cloud.getRenderPosition().x, cloud.getRenderPosition().y);
		}
	}

	// UI
	renderUI();

	// Environment color
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	
	SDL_RenderPresent(renderer);
}

// Ñalled at the end of each frame
void Game::postUpdate()
{
	frameCap();
	
	if (!is_level_running)
	{
		resetLevel();
	}
}

// Clean game subsystem
void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	window = nullptr;
	renderer = nullptr;

	gDustCloudTexture.free();
	gGasCloudTexture.free();
	gLightenedGasCloudTexture.free();
	gProtostarTexture.free();
	gStarTexture.free();
	gPlanet10DwarfTexture.free();
	gPlanet15EarthRockyTexture.free();
	gPlanet15EarthIcyTexture.free();
	gPlanet15EarthOceanTexture.free();
	gPlanet15EarthHotTexture.free();
	gPlanet25SubGiantTexture.free();
	gPlanet25SubGiantHotTexture.free();
	gPlanet35SubGiantTexture.free();
	gPlanet35SubGiantHotTexture.free();
	gPlanet45GiantTexture.free();
	gPlanet55GiantTexture.free();
	gPlanet65GiantTexture.free();
	gPlanet75GiantTexture.free();
	gPlanet85GiantTexture.free();

	TTF_CloseFont(font);
	font = nullptr;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	std::cout << "Game cleaned" << std::endl;
}

void Game::loadMedia()
{
	//Load static textures
	if (!gDustCloudTexture.loadFromFile(renderer, "assets/textures/celestial_bodies/clouds/dust.png"))
	{
		printf("Failed to load particle texture!\n");
		//success = false;
	}

	if (!gGasCloudTexture.loadFromFile(renderer, "assets/textures/celestial_bodies/clouds/gas.png"))
	{
		printf("Failed to load particle texture!\n");
		//success = false;
	}

	if (!gLightenedGasCloudTexture.loadFromFile(renderer, "assets/textures/celestial_bodies/clouds/lgas.png"))
	{
		printf("Failed to load particle texture!\n");
		//success = false;
	}

	if (!gProtostarTexture.loadFromFile(renderer, "assets/textures/celestial_bodies/stars/protostar.png"))
	{
		printf("Failed to load protostar texture!\n");
		//success = false;
	}

	if (!gStarTexture.loadFromFile(renderer, "assets/textures/celestial_bodies/stars/star.png"))
	{
		printf("Failed to load star texture!\n");
		//success = false;
	}

	if (!gPlanet10DwarfTexture.loadFromFile(renderer, "assets/textures/celestial_bodies/planets/planet10_dwarf.png"))
	{
		printf("Failed to load planet texture!\n");
		//success = false;
	}

	if (!gPlanet15EarthRockyTexture.loadFromFile(renderer, "assets/textures/celestial_bodies/planets/planet15_earth_rocky.png"))
	{
		printf("Failed to load planet texture!\n");
		//success = false;
	}

	if (!gPlanet15EarthIcyTexture.loadFromFile(renderer, "assets/textures/celestial_bodies/planets/planet15_earth_icy.png"))
	{
		printf("Failed to load planet texture!\n");
		//success = false;
	}

	if (!gPlanet15EarthOceanTexture.loadFromFile(renderer, "assets/textures/celestial_bodies/planets/planet15_earth_ocean.png"))
	{
		printf("Failed to load planet texture!\n");
		//success = false;
	}

	if (!gPlanet15EarthHotTexture.loadFromFile(renderer, "assets/textures/celestial_bodies/planets/planet15_earth_hot.png"))
	{
		printf("Failed to load planet texture!\n");
		//success = false;
	}

	if (!gPlanet25SubGiantTexture.loadFromFile(renderer, "assets/textures/celestial_bodies/planets/planet25_subgiant.png"))
	{
		printf("Failed to load planet texture!\n");
		//success = false;
	}

	if (!gPlanet25SubGiantHotTexture.loadFromFile(renderer, "assets/textures/celestial_bodies/planets/planet25_subgiant_hot.png"))
	{
		printf("Failed to load planet texture!\n");
		//success = false;
	}

	if (!gPlanet35SubGiantTexture.loadFromFile(renderer, "assets/textures/celestial_bodies/planets/planet35_subgiant.png"))
	{
		printf("Failed to load planet texture!\n");
		//success = false;
	}

	if (!gPlanet35SubGiantHotTexture.loadFromFile(renderer, "assets/textures/celestial_bodies/planets/planet35_subgiant_hot.png"))
	{
		printf("Failed to load planet texture!\n");
		//success = false;
	}

	if (!gPlanet45GiantTexture.loadFromFile(renderer, "assets/textures/celestial_bodies/planets/planet45_giant.png"))
	{
		printf("Failed to load planet texture!\n");
		//success = false;
	}

	if (!gPlanet55GiantTexture.loadFromFile(renderer, "assets/textures/celestial_bodies/planets/planet55_giant.png"))
	{
		printf("Failed to load planet texture!\n");
		//success = false;
	}

	if (!gPlanet65GiantTexture.loadFromFile(renderer, "assets/textures/celestial_bodies/planets/planet65_giant.png"))
	{
		printf("Failed to load planet texture!\n");
		//success = false;
	}

	if (!gPlanet75GiantTexture.loadFromFile(renderer, "assets/textures/celestial_bodies/planets/planet75_giant.png"))
	{
		printf("Failed to load planet texture!\n");
		//success = false;
	}

	if (!gPlanet85GiantTexture.loadFromFile(renderer, "assets/textures/celestial_bodies/planets/planet85_giant.png"))
	{
		printf("Failed to load planet texture!\n");
		//success = false;
	}

}

void Game::frameCap()
{
	//Calculate and correct fps
	/*
	avgFPS = countedFrames / (systemTimer.getTicks() / 1000.f);
	if (avgFPS > 2000000)
	{
		avgFPS = 0;
	}
	*/
	++countedFrames;

	//If frame finished early
	frameTicks = deltaTimer.getTicks();

	if (frameTicks < SCREEN_TICK_PER_FRAME)
	{
		//Wait remaining time
		SDL_Delay(SCREEN_TICK_PER_FRAME - frameTicks);
		FPS = SCREEN_FPS;
	}
	else
	{
		FPS = 1000 / frameTicks;
	}
}

void Game::generateClouds()
{
	
	for (uint16_t i = 0; i < GAS_CLOUDS_NUMBER; i++)
	{
		Vector2f pos = getStartObjectPosition();
		Vector2f speed = getStartObjectSpeed(pos, CLOUD_START_SPEED_VARIATION_RANGE);

		gas_clouds.emplace_back(Cloud(pos, speed, CloudTypes::GAS));
	}

	
	for (uint16_t i = 0; i < DUST_CLOUDS_NUMBER; i++)
	{
		Vector2f pos = getStartObjectPosition();
		Vector2f speed = getStartObjectSpeed(pos, CLOUD_START_SPEED_VARIATION_RANGE);

		dust_clouds.emplace_back(Cloud(pos, speed, CloudTypes::DUST));
	}
	

}

bool Game::renderUI()
{
	bool success = true;

	std::vector<std::string> messages;

	if (is_level_running)
	{
		std::string fps_message = (FPS <= SCREEN_FPS) ? "FPS: " + std::to_string(FPS) : "FPS: " + std::to_string(SCREEN_FPS) + " (max)";
		int earths_total = rocky_earths_count + icy_earths_count + ocean_earths_count + hot_earths_count;

		messages.push_back("Q - START SIMULATION");
		messages.push_back("------------------------------------");
		messages.push_back(fps_message);
		messages.push_back("Gas clouds: " + std::to_string(gas_clouds.size()));
		messages.push_back("Dust clouds: " + std::to_string(dust_clouds.size()));
		messages.push_back("Planetoids (total): " + std::to_string(planetoids.size()));
		messages.push_back("  Comets: " + std::to_string(comets_count));
		messages.push_back("  Asteroids: " + std::to_string(planetoids_count));
		messages.push_back("  Dwarf planets: " + std::to_string(dwarf_planets_count));
		messages.push_back("  Earth-like (total): " + std::to_string(earths_total));
		messages.push_back("    hot: " + std::to_string(hot_earths_count));
		messages.push_back("    rocky: " + std::to_string(rocky_earths_count));
		messages.push_back("    icy: " + std::to_string(icy_earths_count));
		messages.push_back("    ocean: " + std::to_string(ocean_earths_count));
		messages.push_back(" Gas subgiants: " + std::to_string(gas_subgiants_count));
		messages.push_back(" Gas giants: " + std::to_string(gas_giants_count));

		messages.push_back("Mass (max): " + std::to_string(max_planetoid_mass));
		messages.push_back("Radius (max): " + std::to_string(max_planetoid_radius));
		messages.push_back("Temp (min): " + std::to_string(min_earth_temperature));
	}
	
	int line = 0;

	for (std::string message : messages)
	{
		LTexture textTexture;
		if (!textTexture.loadFromRenderedText(renderer, font, message, textColor))
		{
			std::cout << "Failed to render text texture!" << std::endl;
			success = false;
		}
		textTexture.render(renderer, VIEWPORT_WIDTH + margin, margin + line * line_spacing);
		line++;

		textTexture.free();
	}

	return success;
}
