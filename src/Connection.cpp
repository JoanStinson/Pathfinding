#include "Connection.h"

Connection::Connection() {
}

Connection::Connection(Vector2D f, Vector2D t) {
	from.coord = f;
	to.coord = t;
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
