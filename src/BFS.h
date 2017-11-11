#pragma once
#include "Graph.h"
#include <queue>
#include <map>
#include <algorithm>

Vector2D cell2pix(Vector2D cell)
{
	int offset = CELL_SIZE / 2;
	return Vector2D(cell.x*CELL_SIZE + offset, cell.y*CELL_SIZE + offset);
}

bool ContainsValue(Vector2D key, map<Vector2D, Vector2D> internalMap)
{
	//cout << "Key: (" << key.x << ", " << key.y << ")" << endl;
	bool found = false;
	auto it = internalMap.begin(); // internalMap is std::map
	while (it != internalMap.end()) {

		if (key == it->first) {
			found = true;
			break;
		}
		++it;
	}

	return found;
}

vector<Vector2D> BFS(Vector2D start, Vector2D goal, Graph graph) {
	queue<Vector2D> frontier;
	frontier.push(start);

	vector<Vector2D> visited;
	map<Vector2D, Vector2D> came_from; //key next, value current
	came_from[start] = NULL;

	vector<Vector2D> neighbors;
	Vector2D current(0, 0), next(0, 0);
	bool explored;
	while (!frontier.empty()) {

		current = frontier.front();

		if (current == goal) break; // early exit

		neighbors = graph.GetConnections(current);
		//cout << "Current: "<< current.x << ' ' << current.y << endl;

		for (int i = 0; i < neighbors.size(); i++) {
			next = neighbors[i];
			explored = false;
			for (int j = 0; j < came_from.size(); j++) {
				if (!ContainsValue(next, came_from))
					explored = true;
			}
			if (!explored) {
				came_from[next] = current;
				frontier.push(next);
			}

		}

		vector<Vector2D> path;
		current = goal;
		path.push_back(current);

		for (int i = came_from.size() - 1; i > 0; --i) {
			if (current == start) break;
			current = came_from[current];
			path.push_back(current);
		}
		path.push_back(start);
	std:reverse(path.begin(), path.end());

		return path;
	}
}