#include "Graph.h"
#include <iostream>

Graph::Graph() {
}

Graph::~Graph() {
}

void Graph::AddConnection(Connection c) {
	allConnections.push_back(c);
}

vector<Node> Graph::GetConnections(Node a) { 
		// find a in allConnections
			// guardar totes ses connexions de a
			// retornarles
	vector<Node> neighbors;
	for (int i = 0; i < allConnections.size(); i++) {

		if (allConnections[i].GetFromNode().coord == a.coord) { // TODO ESTO I JA ESTA, crec
			neighbors.push_back(allConnections[i].GetToNode());

		}
		cout << neighbors.size() << endl;

	}
	return neighbors;

}

void Graph::PrintConnections() {
	/*for (int i = 0; i < 10; i++) {
		Connection a(Vector2D(1.f, 1.f), Vector2D(2.f, 2.f), 1.f);
		allConnections.push_back(a);
	}
	if (!allConnections.empty()) {
		for (std::vector<Connection>::iterator i = allConnections.begin(); i != allConnections.end() - 1; ++i) {
			cout << &i << endl;
		}
	}
	else cout << "El vector de conneccions allConnections esta buit :)" << endl;*/
}
