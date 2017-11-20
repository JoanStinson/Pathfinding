#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "SDL_SimpleApp.h"
#include "ScenePathFinding.h"
#include "SceneBFS.h"
#include "SceneDijkstra.h"
#include "SceneGBFS.h"
#include "SceneAStar.h"
#include "SceneAStarRL.h"
#include "SceneAStarEnemies.h"

using namespace std;
#define FRAMES_PER_SEC 30

int main(int argc, char ** argv) {
	// Print Welcome Message
	cout << endl << "  Welcome to our SDL Pathfinding Algorithms Implementation!" << endl;
	cout << endl;
	cout << "  -Controls- " << endl;
	cout << "  Key 0 - Default Scene" << endl;
	cout << "  Key 1 - Breadth First Search" << endl;
	cout << "  Key 2 - Dijkstra's Algorithm" << endl;
	cout << "  Key 3 - Greedy Breadth First Search" << endl;
	cout << "  Key 4 - A* Algorithm" << endl;
	cout << "  Key 5 - A* Algorithm with N Locations" << endl;
	cout << "  Key 6 - A* Algorithm with N Enemies" << endl;
	cout << "  Key C - Draw Costs" << endl;
	cout << "  Key F - Draw Frontier" << endl;
	cout << "  Key L - Draw Lines" << endl;
	cout << "  Key M - Draw Map" << endl;
	cout << "  Key N - Draw Cost Numbers" << endl;
	cout << "  Key Space - Draw Grid" << endl;
	cout << "  Key Enter - Fullscreen" << endl;
	cout << "  Key Q or ESC - Exit" << endl;
	cout << endl;
	cout << "  -Explored Nodes- " << endl;

	bool quit = false;
	SDL_Event event;
	int this_tick = 0;
	int next_tick = 1000/FRAMES_PER_SEC;

	SDL_SimpleApp *app = SDL_SimpleApp::Instance();

	Scene *curr_scene = new ScenePathFinding;
	app->setWindowTitle(curr_scene->getTitle());

	while (!quit) {
		this_tick = SDL_GetTicks();
		/*if (this_tick < next_tick) {
			SDL_Delay(next_tick - this_tick);
		}*/
		next_tick = this_tick + (1000 / FRAMES_PER_SEC);

		// run app frame by frame
		event = app->run(curr_scene);

		/* Keyboard events */
		switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.scancode == SDL_SCANCODE_0) {
					delete(curr_scene);
					curr_scene = new ScenePathFinding;
					app->setWindowTitle(curr_scene->getTitle());
					cout << "\r" << "           " << "    " << "      " << "    " << "      " << "    " << "          " << "    " << "            ";
				}

				if (event.key.keysym.scancode == SDL_SCANCODE_1) {
					delete(curr_scene);
					curr_scene = new SceneBFS;
					app->setWindowTitle(curr_scene->getTitle());
				}

				if (event.key.keysym.scancode == SDL_SCANCODE_2) {
					delete(curr_scene);
					curr_scene = new SceneDijkstra;
					app->setWindowTitle(curr_scene->getTitle());
				}

				if (event.key.keysym.scancode == SDL_SCANCODE_3) {
					delete(curr_scene);
					curr_scene = new SceneGBFS;
					app->setWindowTitle(curr_scene->getTitle());
				}

				if (event.key.keysym.scancode == SDL_SCANCODE_4) {
					delete(curr_scene);
					curr_scene = new SceneAStar;
					app->setWindowTitle(curr_scene->getTitle());
				}

				if (event.key.keysym.scancode == SDL_SCANCODE_5) {
					delete(curr_scene);
					curr_scene = new SceneAStarRL;
					app->setWindowTitle(curr_scene->getTitle());
				}

				if (event.key.keysym.scancode == SDL_SCANCODE_6) {
					delete(curr_scene);
					curr_scene = new SceneAStarEnemies;
					app->setWindowTitle(curr_scene->getTitle());
				}

				if ((event.key.keysym.scancode == SDL_SCANCODE_Q) || (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) {
					quit = true;
				}

				if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
					app->setFullScreen();
				}
			break;

			case SDL_QUIT:
				quit = true;
				break;
		}

	}

	return 0;
}