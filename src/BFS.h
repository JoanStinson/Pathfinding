#pragma once
#include "Node.h"
#include "Connection.h"
#include "Graph.h"
#include <queue>
#include <list>
#include <map>
#include "Agent.h"

void BFS(Node start, Node goal, Graph graph, Agent *agent) { // TODO Passarli la posicio inicial del agent = Agent.GetPosition()
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
		cout << "xd";
		neighbors = graph.GetConnections(current);
		found = false;

		/*if (current.coord == goal.coord)
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
	for (int i = 0; i < path.size(); i++) {
		reversed[i] = path[path.size() - 1];
	}
	for (int i = 0; i < reversed.size(); i++) {
		agent->setTarget(reversed[i].coord);*/
	}
}