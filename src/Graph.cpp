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
	
	vector<Node> neighbors;
	// Només explorem connexions si el vector està ple
	if (!allConnections.empty()) {
		int index = 0;
		for (std::vector<Connection>::iterator i = allConnections.begin(); i != allConnections.end(); ++i) {
			cout << "Connexio numero " << index << ": " << allConnections[index].GetCost() << endl;
			// find a in allConnections
			if (allConnections[index].GetFromNode().coord == a.coord) 
				neighbors.push_back(allConnections[index].GetToNode()); // guardar totes ses connexions de a 
			++index;
		}
	} else cout << "El vector de conneccions allConnections esta buit :)" << endl;
	
	return neighbors; // retornarles
}

void Graph::PrintConnections() {
	for (int i = 0; i < 10; i++) {
		Connection a(Vector2D(1.f, 1.f), Vector2D(2.f, 2.f), 1.f);
		allConnections.push_back(a);
	}
	if (!allConnections.empty()) {
		/*for (std::vector<Connection>::iterator i = allConnections.begin(); i != allConnections.end() - 1; ++i) {
			cout << &i << endl;
		}*/
	} else cout << "El vector de conneccions allConnections esta buit :)" << endl;
}
