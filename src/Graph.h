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

	void AddConnection(Connection c);

	// Returns a list of connections outgoing from the given node
	list<Connection> GetConnections(Vector2D a);

private:
	int v;
	vector<Connection> adj;
};