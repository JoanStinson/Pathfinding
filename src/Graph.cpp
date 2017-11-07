#include "Graph.h"
#include <iostream>

Graph::Graph() {
}

Graph::~Graph() {
}

void Graph::AddConnection(Connection c) {
	allConnections.push_back(c);
}

vector<Node> Graph::GetConnections(Node a) { // falla això allConnections està buit, però no tocaria, ja que l'emplenam desde l' initMaze, a ScenePathFinding
	// find a in allConnections
	// guardar totes ses connexions de a
	// retornarles
	vector<Node> neighbors;
	for (int i = 0; i < allConnections.size(); i++) {

		if (allConnections[i].GetFromNode().coord == a.coord) {
			neighbors.push_back(allConnections[i].GetToNode());
		}
		return neighbors;
	}
}
