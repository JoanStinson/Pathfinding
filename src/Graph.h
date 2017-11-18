#pragma once
#include "Node.h"
#include "Connection.h"
#include "Vector2D.h"
#include <vector>
using namespace std;

class Graph {
public:
	Graph();
	~Graph();

	// Push back to allConnections
	void AddConnection(Connection c);

	// Returns a list of connections outgoing from the given node
	vector<Vector2D> GetConnections(Vector2D a);

	int GetCost(Vector2D a);

	// Data structure that contains all connections of the graph
	vector<Connection> allConnections;

private:
	// Number of connexions
	int v;
};