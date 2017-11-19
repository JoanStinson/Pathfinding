#include "Connection.h"

Connection::Connection() {
}

Connection::Connection(Vector2D f, Vector2D t, int c) {
	from.coord = f;
	to.coord = t;
	cost = c;
}

Connection::~Connection() {
}

int Connection::GetCost() {
	return cost;
}

Node Connection::GetFromNode() {
	return from;
}

Node Connection::GetToNode() {
	return to;
}
