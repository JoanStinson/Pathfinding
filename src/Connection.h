#pragma once
#include "Node.h"

class Connection {
public:
	Connection(Vector2D f, Vector2D t, float c);
	~Connection();

	// Returns the non-negative cost of the connection
	float GetCost();

	// Returns the node that this connection came from
	Vector2D GetFromNode();

	// Returns the node that this connection leads to
	Vector2D GetToNode();

private:
	Vector2D from, to;
	float cost;
};