#pragma once
#include "Scene.h"
#include "Agent.h"
#include "Path.h"
#include "Graph.h"
#include <iostream>
#include <vector>
#include <time.h>
using namespace std;

class SceneDijkstra : public Scene {
public:
	SceneDijkstra();
	~SceneDijkstra();
	void update(float dtime, SDL_Event *event);
	void draw();
	const char* getTitle();

private:
	std::vector<Agent*> agents;
	Vector2D coinPosition;
	Vector2D currentTarget;
	int currentTargetIndex;
	Path path;
	int num_cell_x;
	int num_cell_y;
	bool draw_grid;
	std::vector<SDL_Rect> maze_rects;
	void drawMaze();
	void drawCoinAndStart();
	SDL_Texture *background_texture;
	SDL_Texture *coin_texture;
	void initMaze();
	bool loadTextures(char* filename_bg, char* filename_coin, char* start);
	std::vector< std::vector<int> > terrain;
	Vector2D cell2pix(Vector2D cell);
	Vector2D pix2cell(Vector2D pix);
	bool isValidCell(Vector2D cell);

	Graph graph;
	Node start, end;
	vector<Vector2D> dijkstra;
	SDL_Texture *start_texture;
	int coin_w;
	bool draw_lines;
};
