#pragma once
#include "Vector2D.h"
#include <vector>
#include <map>
using namespace std;

class Graph {
public:
	Graph();
	~Graph();

	// Adds the from and the to's to allConnections
	void AddConnection(Vector2D from, Vector2D to);

	// Returns a list of neighbors outgoing from the given node
	vector<Vector2D> GetConnections(Vector2D from);

private:
	// Stores all connectios <key = from, value = to>
	// We used multimap to have +1 value per key, because 
	// there can be from 1-4 neighbors (1-4 values per key)
	multimap<Vector2D, Vector2D> allConnections;
};