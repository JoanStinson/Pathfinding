#include "Graph.h"
#include <iostream>
//#include "BFS.h"

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
		//cout << "Conn " << i << ": " << "x: " << allConnections[i].GetFromNode().coord.x<< " y: " << allConnections[i].GetFromNode().coord.y<< " Node on cercam: " << a.coord.x << ' ' << a.coord.y << endl;

			if (allConnections[i].GetFromNode().coord == a.coord) { 
				neighbors.push_back(allConnections[i].GetToNode());
		}

	}
	return neighbors;

}
void Graph::PrintConnections() {
	
}
