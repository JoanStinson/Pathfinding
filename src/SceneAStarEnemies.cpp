#include "SceneAStarEnemies.h"

float SceneAStarEnemies::Heuristic(Vector2D a, Vector2D b) {
	return abs(a.x - b.x) + abs(a.y - b.y);
}

bool SceneAStarEnemies::allPointsVisited() {
	for (int i = 0; i <= 3; i++) {
		if (pointsList[i] != 0) return false;
	}
	return true;
}

SceneAStarEnemies::SceneAStarEnemies() {
	draw_grid = false;

	num_cell_x = SRC_WIDTH / CELL_SIZE;
	num_cell_y = SRC_HEIGHT / CELL_SIZE;
	initMaze();
	loadTextures("../res/maze.png", "../res/coins.png", "../res/coins2.png", "../res/start.png");

	srand((unsigned int)time(NULL));

	Agent *agent = new Agent;
	agent->loadSpriteTexture("../res/soldier.png", 4);
	agents.push_back(agent);

	// Set agent position coords to the center of a random cell
	Vector2D rand_cell(-1, -1);
	while (!isValidCell(rand_cell))
		rand_cell = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));
	agents[0]->setPosition(cell2pix(rand_cell));
	start = Node(agents[0]->getPosition().x, agents[0]->getPosition().y);

	// Set the coin in a random cell (but at least 3 cells far from the agent)
	coinPosition = Vector2D(-1, -1);
	while ((!isValidCell(coinPosition)) || (Vector2D::Distance(coinPosition, rand_cell) < 3))
		coinPosition = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));

	// Set random N locations
	randNum = (rand() % 3) + 1;
	for (unsigned int i = 1; i <= randNum; i++) {
		rList[i] = Vector2D(-1, -1);
		while ((!isValidCell(rList[i])) || (Vector2D::Distance(rList[i], rand_cell) < 3))
			rList[i] = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));
	}

	// PathFollowing next Target
	currentTarget = Vector2D(0, 0);
	currentTargetIndex = -1;

	// A* Algorithm with random positions
	/*pList[0] = pix2cell(start.coord);

	if (randNum == 1) pList[1] = rList[1];

	else if (randNum == 2) {
		if (Heuristic(pList[0], rList[1]) > Heuristic(pList[0], rList[2])) {
			pList[1] = rList[2];
			pList[2] = rList[1];
		}
		else {
			pList[1] = rList[1];
			pList[2] = rList[2];
		}
	}

	else if (randNum >= 3) {
		int i = 1;
		for (unsigned int j = 1; j <= randNum; j++) {

			if (Heuristic(pList[i], rList[j]) > Heuristic(pList[i], rList[j + 1])) {
				pList[i] = rList[j + 1];
				pList[i + 1] = rList[j];
			}
			else {
				pList[i] = rList[j];
				pList[i + 1] = rList[j + 1];
			}
			i++;
		}
	}

	pList[randNum + 1] = coinPosition;*/

	//for (unsigned int i = 0; i <= randNum; i++) {
		astar = agents[0]->AStar(pix2cell(start.coord), coinPosition, graph, false);
		for (unsigned int i = 0; i < astar.size(); i++) {
			path.points.push_back(cell2pix(astar[i]));
		}
	//}

	/*
	generateXRandomGreenPoints->store them to a list
	while(!allGreenPointsVisited())
	{
	int i = WhichIsTheClosestGreenPoint();
	actualGoal=GreenPoint[i];
	}
	GoToGoal()
	*/

	/*
	Exercici num.3 N ubicacions aleatories i las has d'esquivar, com l'anterior pero son ubicacions per les quals no pots passar
	*/
}

SceneAStarEnemies::~SceneAStarEnemies() {
	if (background_texture)
		SDL_DestroyTexture(background_texture);
	if (coin_texture)
		SDL_DestroyTexture(coin_texture);
	if (coin_texture2)
		SDL_DestroyTexture(coin_texture2);
	if (start_texture)
		SDL_DestroyTexture(start_texture);

	for (int i = 0; i < (int)agents.size(); i++) {
		delete agents[i];
	}
}

void SceneAStarEnemies::update(float dtime, SDL_Event *event) {
	/* Keyboard & Mouse events */
	switch (event->type) {
	case SDL_KEYDOWN:
		if (event->key.keysym.scancode == SDL_SCANCODE_F)
			draw_frontier = !draw_frontier;
		else if (event->key.keysym.scancode == SDL_SCANCODE_L)
			draw_lines = !draw_lines;
		else if (event->key.keysym.scancode == SDL_SCANCODE_SPACE)
			draw_grid = !draw_grid;
		break;
	default:
		break;
	}
	if ((currentTargetIndex == -1) && (path.points.size() > 0))
		currentTargetIndex = 0;

	if (currentTargetIndex >= 0) {
		float dist = Vector2D::Distance(agents[0]->getPosition(), path.points[currentTargetIndex]);
		if (dist < path.ARRIVAL_DISTANCE) {
			if (currentTargetIndex == path.points.size() - 1) {
				if (dist < 3) {
					currentTargetIndex = -1;
					agents[0]->setVelocity(Vector2D(0, 0));
					// if we have arrived to the coin, replace it ina random cell!
					if (pix2cell(agents[0]->getPosition()) == coinPosition) {

						coinPosition = Vector2D(-1, -1);
						while ((!isValidCell(coinPosition)) || (Vector2D::Distance(coinPosition, pix2cell(agents[0]->getPosition())) < 3))
							coinPosition = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));

						// Set random N locations
						randNum = (rand() % 3) + 1;
						for (unsigned int i = 1; i <= randNum; i++) {
							rList[i] = Vector2D(-1, -1);
							while ((!isValidCell(rList[i])) || (Vector2D::Distance(rList[i], pix2cell(agents[0]->getPosition())) < 3))
								rList[i] = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));
						}

						agents[0]->setPosition(path.points.back());
						start = Node(agents[0]->getPosition());
						path.points.clear();

						// A* Algorithm with random positions
						/*pList[0] = pix2cell(start.coord);

						if (randNum == 1) pList[1] = rList[1];

						else if (randNum == 2) {
							if (Heuristic(pList[0], rList[1]) > Heuristic(pList[0], rList[2])) {
								pList[1] = rList[2];
								pList[2] = rList[1];
							}
							else {
								pList[1] = rList[1];
								pList[2] = rList[2];
							}
						}

						else if (randNum >= 3) {
							int i = 1;
							for (unsigned int j = 1; j <= randNum; j++) {

								if (Heuristic(pList[i], rList[j]) > Heuristic(pList[i], rList[j + 1])) {
									pList[i] = rList[j + 1];
									pList[i + 1] = rList[j];
								}
								else {
									pList[i] = rList[j];
									pList[i + 1] = rList[j + 1];
								}
								i++;
							}
						}

						pList[randNum + 1] = coinPosition;*/

						//for (unsigned int i = 0; i <= randNum; i++) {
							astar = agents[0]->AStar(pix2cell(start.coord), coinPosition, graph, false);
							for (unsigned int i = 0; i < astar.size(); i++) {
								path.points.push_back(cell2pix(astar[i]));
							}
						//}
					}
				}
				else {
					Vector2D steering_force = agents[0]->Behavior()->Arrive(agents[0], currentTarget, path.ARRIVAL_DISTANCE, dtime);
					agents[0]->update(steering_force, dtime, event);
				}
				return;
			}
			currentTargetIndex++;
		}

		currentTarget = path.points[currentTargetIndex];
		Vector2D steering_force = agents[0]->Behavior()->Seek(agents[0], currentTarget, dtime);
		agents[0]->update(steering_force, dtime, event);
	}
	else {
		agents[0]->update(Vector2D(0, 0), dtime, event);
	}
}

void SceneAStarEnemies::draw() {

	drawMaze();

	// Draw grid
	if (draw_grid) {
		SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 255, 255, 255, 127);
		for (int i = 0; i < SRC_WIDTH; i += CELL_SIZE) {
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), i, 0, i, SRC_HEIGHT);
		}
		for (int j = 0; j < SRC_HEIGHT; j = j += CELL_SIZE) {
			SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), 0, j, SRC_WIDTH, j);
		}
	}

	// Draw frontier
	if (draw_frontier) {
		for (unsigned int i = 0; i < agents[0]->frontierCount.size(); i++) {
			draw_circle(TheApp::Instance()->getRenderer(), cell2pix(agents[0]->frontierCount[i]).x, cell2pix(agents[0]->frontierCount[i]).y, 15, 0, 191, 255, 255);
		}
	}

	// Draw path
	for (int i = 2; i < (int)path.points.size() - 1; i++) {
		draw_circle(TheApp::Instance()->getRenderer(), (int)(path.points[i].x), (int)(path.points[i].y), 15, 255, 255, 0, 255);
		if (draw_lines) {
			if (i > 2)
				SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), (int)(path.points[i - 1].x), (int)(path.points[i - 1].y), (int)(path.points[i].x), (int)(path.points[i].y));
		}
	}

	drawCoinAndStart();
	drawNPositions();
	draw_circle(TheApp::Instance()->getRenderer(), (int)currentTarget.x, (int)currentTarget.y, 15, 255, 0, 0, 255);
	agents[0]->draw();
}

const char* SceneAStarEnemies::getTitle() {
	return "SDL Pathfinding Algorithms :: A* Algorithm with N Enemies";
}

void SceneAStarEnemies::drawMaze() {
	if (draw_grid) {
		SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 0, 0, 255, 255);
		for (unsigned int i = 0; i < maze_rects.size(); i++)
			SDL_RenderFillRect(TheApp::Instance()->getRenderer(), &maze_rects[i]);
	}
	else SDL_RenderCopy(TheApp::Instance()->getRenderer(), background_texture, NULL, NULL);
}

void SceneAStarEnemies::drawCoinAndStart() {
	Vector2D coin_coords = cell2pix(coinPosition);
	int offset = CELL_SIZE / 2;
	Uint32 sprite = (int)(SDL_GetTicks() / (150)) % 10;
	int coin_w = 290 / 10;
	int sprite_height = 30;
	SDL_Rect srcrect = { (int)sprite * coin_w, 0, coin_w, sprite_height };
	SDL_Rect dstrect = { (int)coin_coords.x - (coin_w / 2), (int)coin_coords.y - (sprite_height / 2), coin_w, sprite_height };
	SDL_Point center = { coin_w / 2, sprite_height / 2 };
	SDL_RenderCopyEx(TheApp::Instance()->getRenderer(), coin_texture, &srcrect, &dstrect, 0, &center, SDL_FLIP_NONE);

	SDL_Rect dstrect2 = { (int)start.coord.x - offset, (int)start.coord.y - offset, CELL_SIZE, CELL_SIZE };
	SDL_RenderCopy(TheApp::Instance()->getRenderer(), start_texture, NULL, &dstrect2);
}

void SceneAStarEnemies::drawNPositions() {
	for (unsigned int i = 1; i <= randNum; i++) {
		/*Vector2D coin_coords = cell2pix(rList[i]);
		int offset = CELL_SIZE / 2;
		SDL_Rect dstrect = { (int)coin_coords.x - offset, (int)coin_coords.y - offset, CELL_SIZE, CELL_SIZE };
		SDL_RenderCopy(TheApp::Instance()->getRenderer(), coin_texture2, NULL, &dstrect);*/

		Vector2D coin_coords = cell2pix(rList[i]);
		int offset = CELL_SIZE / 2;
		Uint32 sprite = (int)(SDL_GetTicks() / (150)) % 10;
		int coin_w = 290 / 10;
		int sprite_height = 30;
		SDL_Rect srcrect = { (int)sprite * coin_w, 0, coin_w, sprite_height };
		SDL_Rect dstrect = { (int)coin_coords.x - (coin_w / 2), (int)coin_coords.y - (sprite_height / 2), coin_w, sprite_height };
		SDL_Point center = { coin_w / 2, sprite_height / 2 };
		SDL_RenderCopyEx(TheApp::Instance()->getRenderer(), coin_texture2, &srcrect, &dstrect, 0, &center, SDL_FLIP_NONE);
	}
}

void SceneAStarEnemies::initMaze() {

	// Initialize a list of Rectagles describing the maze geometry (useful for collision avoidance)
	SDL_Rect rect = { 0, 0, 1280, 32 };
	maze_rects.push_back(rect);
	rect = { 608, 32, 64, 32 };
	maze_rects.push_back(rect);
	rect = { 0, 736, 1280, 32 };
	maze_rects.push_back(rect);
	rect = { 608, 512, 64, 224 };
	maze_rects.push_back(rect);
	rect = { 0,32,32,288 };
	maze_rects.push_back(rect);
	rect = { 0,416,32,320 };
	maze_rects.push_back(rect);
	rect = { 1248,32,32,288 };
	maze_rects.push_back(rect);
	rect = { 1248,416,32,320 };
	maze_rects.push_back(rect);
	rect = { 128,128,64,32 };
	maze_rects.push_back(rect);
	rect = { 288,128,96,32 };
	maze_rects.push_back(rect);
	rect = { 480,128,64,32 };
	maze_rects.push_back(rect);
	rect = { 736,128,64,32 };
	maze_rects.push_back(rect);
	rect = { 896,128,96,32 };
	maze_rects.push_back(rect);
	rect = { 1088,128,64,32 };
	maze_rects.push_back(rect);
	rect = { 128,256,64,32 };
	maze_rects.push_back(rect);
	rect = { 288,256,96,32 };
	maze_rects.push_back(rect);
	rect = { 480, 256, 320, 32 };
	maze_rects.push_back(rect);
	rect = { 608, 224, 64, 32 };
	maze_rects.push_back(rect);
	rect = { 896,256,96,32 };
	maze_rects.push_back(rect);
	rect = { 1088,256,64,32 };
	maze_rects.push_back(rect);
	rect = { 128,384,32,256 };
	maze_rects.push_back(rect);
	rect = { 160,512,352,32 };
	maze_rects.push_back(rect);
	rect = { 1120,384,32,256 };
	maze_rects.push_back(rect);
	rect = { 768,512,352,32 };
	maze_rects.push_back(rect);
	rect = { 256,640,32,96 };
	maze_rects.push_back(rect);
	rect = { 992,640,32,96 };
	maze_rects.push_back(rect);
	rect = { 384,544,32,96 };
	maze_rects.push_back(rect);
	rect = { 480,704,32,32 };
	maze_rects.push_back(rect);
	rect = { 768,704,32,32 };
	maze_rects.push_back(rect);
	rect = { 864,544,32,96 };
	maze_rects.push_back(rect);
	rect = { 320,288,32,128 };
	maze_rects.push_back(rect);
	rect = { 352,384,224,32 };
	maze_rects.push_back(rect);
	rect = { 704,384,224,32 };
	maze_rects.push_back(rect);
	rect = { 928,288,32,128 };
	maze_rects.push_back(rect);

	// Initialize the terrain matrix (for each cell a zero value indicates it's a wall)
	// (1st) initialize all cells to 1 by default
	for (int i = 0; i < num_cell_x; i++)
	{
		vector<int> terrain_col(num_cell_y, 1);
		terrain.push_back(terrain_col);
	}
	// (2nd) set to zero all cells that belong to a wall
	int offset = CELL_SIZE / 2;
	for (int i = 0; i < num_cell_x; i++) {
		for (int j = 0; j < num_cell_y; j++) {
			Vector2D cell_center((float)(i*CELL_SIZE + offset), (float)(j*CELL_SIZE + offset));
			for (unsigned int b = 0; b < maze_rects.size(); b++) {
				if (Vector2DUtils::IsInsideRect(cell_center, (float)maze_rects[b].x, (float)maze_rects[b].y, (float)maze_rects[b].w, (float)maze_rects[b].h)) {
					terrain[i][j] = 0;
					break;
				}
			}
		}
	}

	// Add connections to all cells of the game (that are not walls)
	//40 X CELLS 24 Y CELLS
	for (int i = 0; i < num_cell_x; i++) {
		for (int j = 0; j < num_cell_y; j++) {

			if (terrain[i][j] == 1) {

				if (j < num_cell_y - 1 && terrain[i][j + 1] != 0) {
					Connection c(Vector2D(i, j), Vector2D(i, j + 1));
					graph.AddConnection(c);
				}

				if (i < num_cell_x - 1 && terrain[i + 1][j] != 0) {
					Connection c(Vector2D(i, j), Vector2D(i + 1, j));
					graph.AddConnection(c);
				}

				if (j > 0 && terrain[i][j - 1] != 0) {
					Connection c(Vector2D(i, j), Vector2D(i, j - 1));
					graph.AddConnection(c);
				}

				if (i > 0 && terrain[i - 1][j] != 0) {
					Connection c(Vector2D(i, j), Vector2D(i - 1, j));
					graph.AddConnection(c);
				}

				if (i == 10 && j == 0) {
					Connection c(Vector2D(i, j), Vector2D(10, 39));
					graph.AddConnection(c);
				}

				if (i == 10 && j == 39) {
					Connection c(Vector2D(i, j), Vector2D(10, 0));
					graph.AddConnection(c);
				}

				if (i == 11 && j == 0) {
					Connection c(Vector2D(i, j), Vector2D(11, 39));
					graph.AddConnection(c);
				}

				if (i == 11 && j == 39) {
					Connection c(Vector2D(i, j), Vector2D(11, 0));
					graph.AddConnection(c);
				}

				if (i == 12 && j == 0) {
					Connection c(Vector2D(i, j), Vector2D(12, 39));
					graph.AddConnection(c);
				}

				if (i == 12 && j == 39) {
					Connection c(Vector2D(i, j), Vector2D(12, 0));
					graph.AddConnection(c);
				}
			}
		}
	}
}

bool SceneAStarEnemies::loadTextures(char* filename_bg, char* filename_coin, char* filename_coin2, char* start) {
	// Bg
	SDL_Surface *image = IMG_Load(filename_bg);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	background_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	// Coin
	image = IMG_Load(filename_coin);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	coin_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	// Coin2
	image = IMG_Load(filename_coin2);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	coin_texture2 = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	// Start
	image = IMG_Load(start);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	start_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	return true;
}

Vector2D SceneAStarEnemies::cell2pix(Vector2D cell) {
	int offset = CELL_SIZE / 2;
	return Vector2D(cell.x*CELL_SIZE + offset, cell.y*CELL_SIZE + offset);
}

Vector2D SceneAStarEnemies::pix2cell(Vector2D pix) {
	return Vector2D((float)((int)pix.x / CELL_SIZE), (float)((int)pix.y / CELL_SIZE));
}

bool SceneAStarEnemies::isValidCell(Vector2D cell) {
	if ((cell.x < 0) || (cell.y < 0) || (cell.x >= terrain.size()) || (cell.y >= terrain[0].size()))
		return false;
	return !(terrain[(unsigned int)cell.x][(unsigned int)cell.y] == 0);
}