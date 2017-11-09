#pragma once
#include "Graph.h"
#include <queue>
#include <list>

Vector2D cell2pix(Vector2D cell)
{
	int offset = CELL_SIZE / 2;
	return Vector2D(cell.x*CELL_SIZE + offset, cell.y*CELL_SIZE + offset);
}

vector<Node> BFS(Node start, Node goal, Graph graph, Path p) {
	queue <Node> frontier;
	frontier.push(start);

	vector<pair<Node, Node>> came_from;
	came_from.push_back(pair<Node, Node>(start, NULL));

	vector<Node> neighbors, path;
	Node current(0, 0), next(0, 0);
	int j = 0;

	while (!frontier.empty()) { //TODO solucionar algoritme perque la frontier sempre està plena = bucle infinit :)
		current = frontier.front();
		frontier.pop();

		if (current.coord == goal.coord) break; //early exit

		neighbors = graph.GetConnections(current);
		//cout << neighbors.size() << endl; // se pasen com a posicions de cel·les. nice
		cout << endl << "Node start (" << current.coord.x << ", " << current.coord.y << ") i Node final (" << goal.coord.x << ", " << goal.coord.y << ")" << endl;

		for (int i = 0; i < neighbors.size(); i++) {
			next = neighbors[i];
			cout << "	Node vei " <<  i << " (" << next.coord.x << ", " << next.coord.y << ")" << endl; 
			
			for (j = 0; j < came_from.size(); j++) { 
				cout << "		Node " << j << " (" << came_from[j].first.coord.x << ", " << came_from[j].first.coord.y << ") comes from Node (" << came_from[j].second.coord.x << ", " << came_from[j].second.coord.y << ")" << endl;
				
				// Si next, que és veí de current no està dins l'arrai de nodes explorats "came_from"
				if (next.coord != came_from[j].first.coord) { //es el mateix que dir found == false, però més simple
					// Afagir next a la frontera i afagir a l arrai came_from juntament amb el node del que procedeix
					frontier.push(next);
					came_from.push_back(pair<Node, Node>(next, current));
				}
			}

			cout << "		Came from " << j << " node(s) from start" << endl;
		}
	}

	return path;
}