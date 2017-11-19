#include "Graph.h"

Graph::Graph() {
}

Graph::~Graph() {
}

void Graph::AddConnection(Vector2D from, Vector2D to) {
	mymultimap.emplace(from, to);
}

vector<Vector2D> Graph::GetConnections(Vector2D a) {
	vector<Vector2D> neighbors;

	for (auto it = mymultimap.begin(); it != mymultimap.end(); ++it) {
		if ((*it).first == a) 
			neighbors.push_back((*it).second);
		
		if (neighbors.size() == 4) 
			break;
	}
		
	return neighbors;
}