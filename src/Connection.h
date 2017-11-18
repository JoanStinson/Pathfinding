#pragma once
#include "Node.h"

class Connection {
public:
	Connection();
	Connection(Vector2D f, Vector2D t, int c);
	~Connection();

	// Returns the non-negative cost of the connection
	int GetCost();

	// Returns the node that this connection came from
	Node GetFromNode();

	// Returns the node that this connection leads to
	Node GetToNode();

private:
	Node from, to;
	int cost;
};