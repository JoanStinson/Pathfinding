#pragma once
#include "Scene.h"
#include "Agent.h"
#include "Path.h"
#include "Graph.h"
#include <iostream>
#include <vector>
#include <time.h>
using namespace std;

class SceneAStarRL : public Scene {
public:
	SceneAStarRL();
	~SceneAStarRL();
	// Heuristic to decide which random position to go first
	float Heuristic(Vector2D a, Vector2D b);
	bool allPointsVisited();

	void update(float dtime, SDL_Event *event);
	void draw();
	const char* getTitle();

private:
	std::vector<Agent*> agents;
	Vector2D coinPosition, nPosition, nPosition2;
	Vector2D currentTarget;
	int currentTargetIndex;
	Path path;
	int num_cell_x;
	int num_cell_y;
	bool draw_grid;
	std::vector<SDL_Rect> maze_rects;
	void drawMaze();
	void drawCoinAndStart();
	void drawNPositions();
	SDL_Texture *background_texture;
	SDL_Texture *coin_texture; 
	void initMaze();
	bool loadTextures(char* filename_bg, char* filename_bg2, char* filename_coin, char* filename_coin2, char* start, char* cost1, char* cost2, char* cost3, char* cost4, char* cost5, char* cost6);
	std::vector< std::vector<int> > terrain;
	Vector2D cell2pix(Vector2D cell);
	Vector2D pix2cell(Vector2D pix);
	bool isValidCell(Vector2D cell);

	Graph graph;
	Vector2D start;
	vector<Vector2D> astar;
	SDL_Texture *coin_texture2;
	SDL_Texture *start_texture;

	vector<Vector2D> goalist;

	

	


	Vector2D pointsList[3];
	bool draw_lines, draw_frontier, draw_costs = true, draw_map;
	SDL_Texture *cost1_texture;
	SDL_Texture *cost2_texture;
	SDL_Texture *cost3_texture;
	SDL_Texture *cost4_texture;
	SDL_Texture *cost5_texture;
	SDL_Texture *cost6_texture;
	SDL_Texture *background_texture2;
};
