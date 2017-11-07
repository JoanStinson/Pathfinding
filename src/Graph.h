#pragma once
#include "Node.h"
#include "Connection.h"
#include "Vector2D.h"
#include <vector>
#include <list>
using namespace std;

class Graph {
public:
	Graph();
	~Graph();

	// Push back to allConnections
	void AddConnection(Connection c);

	// Returns a list of connections outgoing from the given node
	vector<Node> GetConnections(Node a);
	
	// Prints allConnections
	void PrintConnections();

	int v; // num connexions

private:
	vector<Connection> allConnections;
};