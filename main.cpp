#include "src/Game.h"
#include "src/Engine/Global.h"

Game* game = nullptr;

int main(int argc, char* argv[])
{
	//Random seed
	srand((unsigned)time(0));
	game = new Game();

	game->init("Planetary System Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, false);

	game->loadLevel();

	while (game->running())
	{
		game->handleEvents();

		if (game->levelRunning() && !game->paused())
		{
			game->updateLevel();
			game->render();
			game->postUpdate();
		}
	}

	game->clean();
	delete game;

	return 0;
}