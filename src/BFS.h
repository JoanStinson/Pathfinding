#pragma once
#include "Node.h"
#include "Connection.h"
#include "Graph.h"
#include <queue>
#include <list>
#include <map>
#include "Agent.h"

Vector2D cell2pix(Vector2D cell)
{
	int offset = CELL_SIZE / 2;
	return Vector2D(cell.x*CELL_SIZE + offset, cell.y*CELL_SIZE + offset);
}

Vector2D pix2cell(Vector2D pix)
{
	return Vector2D((float)((int)pix.x / CELL_SIZE), (float)((int)pix.y / CELL_SIZE));
}

vector<Node> BFS(Node start, Node goal, Graph graph, Path p) { 
	// Inicialitzar la frontera amb el node de la posició inicial
	queue <Node> frontier;
	frontier.push(start);

	//map <Node, bool> came_from;
	//came_from.insert(pair<Node, bool>(start, true));

	vector <Node> neighbors, path, reversed;
	
	Node current(0, 0);
	Node next(0, 0);
	
	bool found;

	while (!frontier.empty()) {
		current = frontier.front();
		frontier.pop();
		Node temp(current.coord); 
		
		neighbors = graph.GetConnections(Node(pix2cell(temp.coord))); //TODO aixo funciona WIIIIIIIIIIIIIIIIIIIIIIIIIIIII, però només si està plena
		found = false;

		if (current.coord == goal.coord)
			break;

		for (int i = 0; i < neighbors.size() || found; i++) {
			if (neighbors[i].coord == next.coord) {
				frontier.push(next);
				//came_from.insert(pair<Node, bool>(next, true));
				next.came_from = &current;	
				current = next;
				found = true;

			}
		}
	}

	while (current.coord != start.coord) {
		path.push_back(*current.came_from);
		current = *current.came_from;
	}
	//cout << neighbors.size() << endl;
	for (int i = 0; i < path.size(); i++) {
		reversed[i] = path[path.size() - 1];
	}
	for (int i = 0; i < reversed.size(); i++) {
		p.points.push_back(path[i].coord);
	}

	return path;
}