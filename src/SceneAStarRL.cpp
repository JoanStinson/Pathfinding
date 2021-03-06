#include "SceneAStarRL.h"

float SceneAStarRL::Heuristic(Vector2D a, Vector2D b) {
	return abs(a.x - b.x) + abs(a.y - b.y);
}

bool SceneAStarRL::allPointsVisited() {
	for (int i = 0; i <= 3; i++) {
		if (pointsList[i] != 0) return false;
	}
	return true;
}

SceneAStarRL::SceneAStarRL() {
	draw_grid = false;

	num_cell_x = SRC_WIDTH / CELL_SIZE;
	num_cell_y = SRC_HEIGHT / CELL_SIZE;
	initMaze();
	loadTextures("../res/maze.png", "../res/maze2.png", "../res/coins.png", "../res/coins2.png", "../res/start.png", "../res/cost1.png", "../res/cost2.png", "../res/cost3.png", "../res/cost4.png", "../res/cost5.png", "../res/cost6.png");

	srand((unsigned int)time(NULL));

	Agent *agent = new Agent;
	agent->loadSpriteTexture("../res/soldier.png", 4);
	agents.push_back(agent);

	// Set agent position coords to the center of a random cell
	Vector2D rand_cell(-1, -1);
	while (!isValidCell(rand_cell))
		rand_cell = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));
	agents[0]->setPosition(cell2pix(rand_cell));
	start = agents[0]->getPosition();

	// Set the coin in a random cell (but at least 3 cells far from the agent)
	coinPosition = Vector2D(-1, -1);
	while ((!isValidCell(coinPosition)) || (Vector2D::Distance(coinPosition, rand_cell) < 3))
		coinPosition = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));

	// PathFollowing next Target
	currentTarget = Vector2D(0, 0);
	currentTargetIndex = -1;



	//////////////////// A* Algorithm with random positions

	// Create random N locations
	int randNum = (rand() % 10) + 1;
	vector<Vector2D> randomLocations;
	for (unsigned int i = 1; i <= randNum; i++) {
		Vector2D pos1(-1, -1);
		while ((!isValidCell(pos1)) || (Vector2D::Distance(pos1, rand_cell) < 3))
			pos1 = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));
		randomLocations.push_back(pos1);
	}

	// Get their heuristic and store them sorted
	vector<std::pair<float, Vector2D>> heuristicValues;
	for (unsigned int i = 0; i < randomLocations.size(); i++) {
		heuristicValues.push_back(std::make_pair(Heuristic(pix2cell(start), randomLocations[i]), randomLocations[i]));
	}
	std::sort(heuristicValues.begin(), heuristicValues.end());

	// Update goal list
	goalist.push_back(pix2cell(start));
	for (unsigned int i = 0; i < heuristicValues.size(); i++) {
		goalist.push_back(heuristicValues[i].second);
	}
	goalist.push_back(coinPosition);

	// Run A* changing goals
	for (unsigned int i = 0; i < goalist.size()-1; i++) {
		astar = agents[0]->AStar(goalist[i], goalist[i + 1], graph, false);
		for (unsigned int i = 0; i < astar.size(); i++) {
			path.points.push_back(cell2pix(astar[i]));
		}
	}

	// Clear data
	/*randomLocations.clear();
	heuristicValues.clear();
	goalist.clear();*/
	
}

SceneAStarRL::~SceneAStarRL() {
	if (background_texture)
		SDL_DestroyTexture(background_texture);
	if (background_texture2)
		SDL_DestroyTexture(background_texture2);
	if (coin_texture)
		SDL_DestroyTexture(coin_texture);
	if (coin_texture2)
		SDL_DestroyTexture(coin_texture2);
	if (start_texture)
		SDL_DestroyTexture(start_texture);
	if (cost1_texture)
		SDL_DestroyTexture(cost1_texture);
	if (cost2_texture)
		SDL_DestroyTexture(cost2_texture);
	if (cost3_texture)
		SDL_DestroyTexture(cost3_texture);
	if (cost4_texture)
		SDL_DestroyTexture(cost4_texture);
	if (cost5_texture)
		SDL_DestroyTexture(cost5_texture);
	if (cost6_texture)
		SDL_DestroyTexture(cost6_texture);

	for (int i = 0; i < (int)agents.size(); i++) {
		delete agents[i];
	}
}

void SceneAStarRL::update(float dtime, SDL_Event *event) {
	/* Keyboard & Mouse events */
	switch (event->type) {
	case SDL_KEYDOWN:
		if (event->key.keysym.scancode == SDL_SCANCODE_C)
			draw_costs = !draw_costs;
		else if (event->key.keysym.scancode == SDL_SCANCODE_F)
			draw_frontier = !draw_frontier;
		else if (event->key.keysym.scancode == SDL_SCANCODE_L)
			draw_lines = !draw_lines;
		else if (event->key.keysym.scancode == SDL_SCANCODE_M)
			draw_map = !draw_map;
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


						agents[0]->setPosition(path.points.back());
						start = agents[0]->getPosition();
						path.points.clear();

						//////////////////// A* Algorithm with random positions
						agents[0]->vector_costs.clear();
						agents[0]->frontierCount.clear();
						// Create random N locations
						goalist.clear();
						int randNum = (rand() % 10) + 1;
						vector<Vector2D> randomLocations;
						for (unsigned int i = 1; i <= randNum; i++) {
							Vector2D pos1(-1, -1);
							while ((!isValidCell(pos1)) || (Vector2D::Distance(pos1, pix2cell(agents[0]->getPosition())) < 3))
								pos1 = Vector2D((float)(rand() % num_cell_x), (float)(rand() % num_cell_y));
							randomLocations.push_back(pos1);
						}

						// Get their heuristic and store them sorted
						vector<std::pair<float, Vector2D>> heuristicValues;
						for (unsigned int i = 0; i < randomLocations.size(); i++) {
							heuristicValues.push_back(std::make_pair(Heuristic(pix2cell(start), randomLocations[i]), randomLocations[i]));
						}
						std::sort(heuristicValues.begin(), heuristicValues.end());

						// Update goal list
						goalist.push_back(pix2cell(start));
						for (unsigned int i = 0; i < heuristicValues.size(); i++) {
							goalist.push_back(heuristicValues[i].second);
						}
						goalist.push_back(coinPosition);

						// Run A* changing goals
						for (unsigned int i = 0; i < goalist.size() - 1; i++) {
							astar = agents[0]->AStar(goalist[i], goalist[i + 1], graph, false);
							for (unsigned int i = 0; i < astar.size(); i++) {
								path.points.push_back(cell2pix(astar[i]));
							}
						}

						// Clear data
						/*randomLocations.clear();
						heuristicValues.clear();
						goalist.clear();*/




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

		// Clear data
		/*randomLocations.clear();
		heuristicValues.clear();
		goalist.clear();
		agents[0]->vector_costs.clear();
		agents[0]->frontierCount.clear();*/
		currentTarget = path.points[currentTargetIndex];
		Vector2D steering_force = agents[0]->Behavior()->Seek(agents[0], currentTarget, dtime);
		agents[0]->update(steering_force, dtime, event);
	}
	else {
		agents[0]->update(Vector2D(0, 0), dtime, event);
	}

}

void SceneAStarRL::draw() {

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

	// Draw costs
	int offset = CELL_SIZE / 2;
	SDL_Texture *terrain = NULL;
	if (draw_costs) {
		for (unsigned int i = 0; i < agents[0]->vector_costs.size(); i++) {
			if (agents[0]->vector_costs[i].second > 5) terrain = cost6_texture;
			else if (agents[0]->vector_costs[i].second > 4) terrain = cost5_texture;
			else if (agents[0]->vector_costs[i].second > 3) terrain = cost4_texture;
			else if (agents[0]->vector_costs[i].second > 2) terrain = cost3_texture;
			else if (agents[0]->vector_costs[i].second > 1) terrain = cost2_texture;
			else if (agents[0]->vector_costs[i].second > 0) terrain = cost1_texture;

			SDL_Rect dstrect = { (int)cell2pix(agents[0]->vector_costs[i].first).x - offset, (int)cell2pix(agents[0]->vector_costs[i].first).y - offset, CELL_SIZE, CELL_SIZE };
			SDL_RenderCopy(TheApp::Instance()->getRenderer(), terrain, NULL, &dstrect);
		}
	}

	// Draw frontier
	if (draw_frontier) {
		for (unsigned int i = 0; i < agents[0]->frontierCount.size(); i++) {
			draw_circle(TheApp::Instance()->getRenderer(), cell2pix(agents[0]->frontierCount[i]).x, cell2pix(agents[0]->frontierCount[i]).y, 15, 0, 191, 255, 255);
		}
	}

	drawNPositions();

	// Draw path
	for (int i = 2; i < (int)path.points.size() - 1; i++) {
		draw_circle(TheApp::Instance()->getRenderer(), (int)(path.points[i].x), (int)(path.points[i].y), 15, 255, 255, 0, 255);
		if (draw_lines) {
			if (i > 2)
				SDL_RenderDrawLine(TheApp::Instance()->getRenderer(), (int)(path.points[i - 1].x), (int)(path.points[i - 1].y), (int)(path.points[i].x), (int)(path.points[i].y));
		}
	}

	
	
	drawCoinAndStart();
	draw_circle(TheApp::Instance()->getRenderer(), (int)currentTarget.x, (int)currentTarget.y, 15, 255, 0, 0, 255);
	agents[0]->draw();
}

const char* SceneAStarRL::getTitle() {
	return "SDL Pathfinding Algorithms :: A* Algorithm with N Locations";
}

void SceneAStarRL::drawMaze() {
	if (draw_grid) {
		SDL_SetRenderDrawColor(TheApp::Instance()->getRenderer(), 0, 0, 255, 255);
		for (unsigned int i = 0; i < maze_rects.size(); i++)
			SDL_RenderFillRect(TheApp::Instance()->getRenderer(), &maze_rects[i]);
	}
	else {
		if (draw_map) SDL_RenderCopy(TheApp::Instance()->getRenderer(), background_texture2, NULL, NULL);
		else SDL_RenderCopy(TheApp::Instance()->getRenderer(), background_texture, NULL, NULL);
	}
}

void SceneAStarRL::drawCoinAndStart() {
	Vector2D coin_coords = cell2pix(coinPosition);
	int offset = CELL_SIZE / 2;
	Uint32 sprite = (int)(SDL_GetTicks() / (150)) % 10;
	int coin_w = 290 / 10;
	int sprite_height = 30;
	SDL_Rect srcrect = { (int)sprite * coin_w, 0, coin_w, sprite_height };
	SDL_Rect dstrect = { (int)coin_coords.x - (coin_w / 2), (int)coin_coords.y - (sprite_height / 2), coin_w, sprite_height };
	SDL_Point center = { coin_w / 2, sprite_height / 2 };
	SDL_RenderCopyEx(TheApp::Instance()->getRenderer(), coin_texture, &srcrect, &dstrect, 0, &center, SDL_FLIP_NONE);

	SDL_Rect dstrect2 = { (int)start.x - offset, (int)start.y - offset, CELL_SIZE, CELL_SIZE };
	SDL_RenderCopy(TheApp::Instance()->getRenderer(), start_texture, NULL, &dstrect2);
}

void SceneAStarRL::drawNPositions() {
	for (unsigned int i = 1; i < goalist.size()-1; i++) {
		Vector2D coin_coords = cell2pix(goalist[i]);
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

void SceneAStarRL::initMaze() {

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
					graph.AddConnection(Vector2D(i, j), Vector2D(i, j + 1));
				}

				if (i < num_cell_x - 1 && terrain[i + 1][j] != 0) {
					graph.AddConnection(Vector2D(i, j), Vector2D(i + 1, j));
				}

				if (j > 0 && terrain[i][j - 1] != 0) {
					graph.AddConnection(Vector2D(i, j), Vector2D(i, j - 1));
				}

				if (i > 0 && terrain[i - 1][j] != 0) {
					graph.AddConnection(Vector2D(i, j), Vector2D(i - 1, j));
				}

				if (i == 10 && j == 0) {
					graph.AddConnection(Vector2D(i, j), Vector2D(10, 39));
				}

				if (i == 10 && j == 39) {
					graph.AddConnection(Vector2D(i, j), Vector2D(10, 0));
				}

				if (i == 11 && j == 0) {
					graph.AddConnection(Vector2D(i, j), Vector2D(11, 39));
				}

				if (i == 11 && j == 39) {
					graph.AddConnection(Vector2D(i, j), Vector2D(11, 0));
				}

				if (i == 12 && j == 0) {
					graph.AddConnection(Vector2D(i, j), Vector2D(12, 39));
				}

				if (i == 12 && j == 39) {
					graph.AddConnection(Vector2D(i, j), Vector2D(12, 0));
				}
			}
		}
	}
}

bool SceneAStarRL::loadTextures(char* filename_bg, char* filename_bg2, char* filename_coin, char* filename_coin2, char* start, char* cost1, char* cost2, char* cost3, char* cost4, char* cost5, char* cost6) {
	// Bg
	SDL_Surface *image = IMG_Load(filename_bg);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	background_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);
	// Bg 2
	image = IMG_Load(filename_bg2);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	background_texture2 = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

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

	// Cost1
	image = IMG_Load(cost1);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	cost1_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	// Cost2
	image = IMG_Load(cost2);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	cost2_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	// Cost3
	image = IMG_Load(cost3);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	cost3_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	// Cost4
	image = IMG_Load(cost4);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	cost4_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	// Cost5
	image = IMG_Load(cost5);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	cost5_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	// Cost6
	image = IMG_Load(cost6);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	cost6_texture = SDL_CreateTextureFromSurface(TheApp::Instance()->getRenderer(), image);

	if (image)
		SDL_FreeSurface(image);

	return true;
}

Vector2D SceneAStarRL::cell2pix(Vector2D cell) {
	int offset = CELL_SIZE / 2;
	return Vector2D(cell.x*CELL_SIZE + offset, cell.y*CELL_SIZE + offset);
}

Vector2D SceneAStarRL::pix2cell(Vector2D pix) {
	return Vector2D((float)((int)pix.x / CELL_SIZE), (float)((int)pix.y / CELL_SIZE));
}

bool SceneAStarRL::isValidCell(Vector2D cell) {
	if ((cell.x < 0) || (cell.y < 0) || (cell.x >= terrain.size()) || (cell.y >= terrain[0].size()))
		return false;
	return !(terrain[(unsigned int)cell.x][(unsigned int)cell.y] == 0);
}