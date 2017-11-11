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

bool ContainsValue(Vector2D value, map<Vector2D, Vector2D> internalMap)
{
	bool found = false;
	auto it = internalMap.begin(); // internalMap is std::map
	while (it != internalMap.end())
	{
		found = (it->second == value);
		if (found)
			break;
		++it;
	}
	return found;
}

vector<Vector2D> BFS(Vector2D start, Vector2D goal, Graph graph, Path p) {
	queue<Vector2D> frontier;
	frontier.push(start);

	vector<Vector2D> visited;
	map<Vector2D, Vector2D> came_from; //key next, value current
	//came_from.push_back(start);

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
			if (!(ContainsValue(next, came_from))) {
				cout << "push" << endl;
				frontier.push(next);
				came_from[next] = current;
			}
			else cout << "caca" << endl;
		}

	}

	vector<Vector2D> path;
	current = goal;
	path.push_back(current);

	/*for (int i = came_from.size()-1; i > 0; --i) {
		if (current == start) break;
		current = came_from[current];
		path.push_back(current);
	}*/
	path.push_back(start);
	std:reverse(path.begin(), path.end());

	return path;
}