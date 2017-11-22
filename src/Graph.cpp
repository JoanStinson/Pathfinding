#include "Graph.h"

Graph::Graph() {
}

Graph::~Graph() {
}

void Graph::AddConnection(Vector2D from, Vector2D to) {
	allConnections.emplace(from, to);
}

vector<Vector2D> Graph::GetConnections(Vector2D from) {
	vector<Vector2D> neighbors;

	// Search for from inside the list of all connections
	for (auto it = allConnections.begin(); it != allConnections.end(); ++it) {
		// If we find it pushback the to (to == value of multimap, every to is a neighbor)
		if ((*it).first == from)
			neighbors.push_back((*it).second);
		
		// If we already have 4 neighbors we can't add more because it's the max, so break
		if (neighbors.size() == 4) 
			break;
	}
		
	return neighbors;
}