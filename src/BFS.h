#pragma once
#include "Node.h"
#include "Connection.h"
#include "Graph.h"
#include <queue>
#include <list>
#include <map>

void BFS(Vector2D start, Graph graph) { // TODO Passarli la posicio inicial del agent = Agent.GetPosition()
	// Inicialitzar la frontera amb el node de la posició inicial
	std::queue<Vector2D> frontier;
	frontier.push(start);

	std::map <Vector2D, bool> visited;
	visited.insert(std::pair<Vector2D, bool>(start, true));

	// Mentre la frontera no estigui buida
	while (!frontier.empty) {
		// Agafem el seguent node
		Vector2D current = frontier.front();
		// Mirem els seus veins 
		// TODO fer un loop 
		graph.GetConnections(current);

		// Si no esta visitat el node current
		for (std::map<Vector2D, bool>::iterator i = visited.begin(); i != visited.end(); ++i) {
			if (/*Current no l'hem visitat*/ false) {
				frontier.push(current);
				visited.insert(std::pair<Vector2D, bool>(current, true));
			}
		}

	}
}