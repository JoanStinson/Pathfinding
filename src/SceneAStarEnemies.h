#pragma once
#include "Scene.h"
#include "Agent.h"
#include "Path.h"
#include "Graph.h"
#include <iostream>
#include <vector>
#include <time.h>
using namespace std;

class SceneAStarEnemies : public Scene {
public:
	SceneAStarEnemies();
	~SceneAStarEnemies();
	// Heuristic to decide which random position to go first
	float Heuristic(Vector2D a, Vector2D b);
	bool allPointsVisited();

	void update(float dtime, SDL_Event *event);
	void draw();
	const char* getTitle();

private:
	vector<Agent*> agents;
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
	bool loadTextures(char* filename_bg, char* filename_bg2, char* filename_coin, char* filename_coin2, char* start, char* cost1, char* cost2, char* cost3, char* cost4, char* cost5, char* cost6,
		char* num1, char* num2, char* num3, char* num4, char* num5, char* num6, char* num9);
	std::vector< std::vector<int> > terrain;
	Vector2D cell2pix(Vector2D cell);
	Vector2D pix2cell(Vector2D pix);
	bool isValidCell(Vector2D cell);

	Graph graph;
	Vector2D start;
	vector<Vector2D> astar;
	SDL_Texture *coin_texture2;
	SDL_Texture *start_texture;

	int randNum;
	Vector2D pList[12];
	vector<Vector2D> randomList;
	Vector2D pointsList[3];
	bool draw_lines, draw_frontier, draw_costs, draw_map, draw_numbers = true;
	SDL_Texture *cost1_texture;
	SDL_Texture *cost2_texture;
	SDL_Texture *cost3_texture;
	SDL_Texture *cost4_texture;
	SDL_Texture *cost5_texture;
	SDL_Texture *cost6_texture;
	SDL_Texture *background_texture2;
	SDL_Texture *num1_texture;
	SDL_Texture *num2_texture;
	SDL_Texture *num3_texture;
	SDL_Texture *num4_texture;
	SDL_Texture *num5_texture;
	SDL_Texture *num6_texture;
	SDL_Texture *num9_texture;
};