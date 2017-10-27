#include "Connection.h"

Connection::Connection(Vector2D f, Vector2D t, float c) {
	from = f;
	to = t;
	cost = c;
}

Connection::~Connection() {
}

float Connection::GetCost() {
	return 0.0f;
}

Vector2D Connection::GetFromNode() {
	return from;
}

Vector2D Connection::GetToNode() {
	return to;
}
