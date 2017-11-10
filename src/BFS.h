#pragma once
#include "Graph.h"
#include <queue>
#include <list>

Vector2D cell2pix(Vector2D cell)
{
	int offset = CELL_SIZE / 2;
	return Vector2D(cell.x*CELL_SIZE + offset, cell.y*CELL_SIZE + offset);
}
bool found = false;
vector<Node> BFS(Node start, Node goal, Graph graph, Path p) {
	queue<Node> frontier;
	frontier.push(start);

	vector<Node> visited, neighbors;
	visited.push_back(start);
	Node current(0, 0), next(0, 0);

	while (!frontier.empty() && current.coord != goal.coord) {

		current = frontier.front();
		frontier.pop();

		neighbors = graph.GetConnections(current);

		for (int i = 0; i < neighbors.size(); i++) {
			next = neighbors[i];
			cout << neighbors[i].coord.x << ' ' << neighbors[i].coord.y << endl;

			for (int j = 0; j < visited.size(); j++) {
				if (visited[j].coord == neighbors[i].coord)
					found = true;
			}
			if (!found) {
				visited.push_back(next);
				frontier.push(next);

			}
			found = false;
		}

	}

	vector<Node> path;
	return path;
}