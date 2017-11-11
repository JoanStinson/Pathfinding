#pragma once
#include "Graph.h"
#include <queue>
#include <list>
#include <algorithm>

Vector2D cell2pix(Vector2D cell)
{
	int offset = CELL_SIZE / 2;
	return Vector2D(cell.x*CELL_SIZE + offset, cell.y*CELL_SIZE + offset);
}

vector<Vector2D> BFS(Vector2D start, Vector2D goal, Graph graph, Path p) {
	queue<Vector2D> frontier;
	frontier.push(start);

	vector<Vector2D> came_from;
	came_from.push_back(start);

	vector<Vector2D> neighbors;
	Vector2D current(0, 0), next(0, 0);

	while (!frontier.empty()) {

		current = frontier.front();
		frontier.pop();

		if (current == goal) break; // early exit

		neighbors = graph.GetConnections(current);

		for (int i = 0; i < neighbors.size(); i++) {
			next = neighbors[i];

			// If next not in came_from
			if (!(std::find(came_from.begin(), came_from.end(), next) != came_from.end())) {
				cout << "NOT" << endl;
				frontier.push(next);
				came_from.push_back(next);
			}
			else cout << "IN" << endl;
		}

	}

	vector<Vector2D> path;
	current = goal;
	path.push_back(current);

	while (current != start) {
		current = came_from.back();
		came_from.pop_back();
		path.push_back(current);
	}
	path.push_back(start);
std:reverse(path.begin(), path.end());

	return path;
}