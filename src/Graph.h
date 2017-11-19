#pragma once
#include "Node.h"
#include "Connection.h"
#include "Vector2D.h"
#include <vector>
#include <iostream>
#include <map>
using namespace std;

class Graph {
public:
	Graph();
	~Graph();

	// Push back to allConnections
	void AddConnection(Vector2D from, Vector2D to);

	// Returns a list of connections outgoing from the given node
	vector<Vector2D> GetConnections(Vector2D a);

private:
	multimap<Vector2D, Vector2D> mymultimap;
};