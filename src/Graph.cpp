#include "Graph.h"

Graph::Graph() {
}

Graph::~Graph() {
}

void Graph::AddConnection(Connection c) {
	adj.push_back(c);
}

list<Connection> Graph::GetConnections(Vector2D a) {
	return list<Connection>();
}
