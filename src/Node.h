#pragma once
#include "Vector2D.h"

class Node {
public:
	Node() {
	}

	Node(float x, float y) {
		coord.x = x;
		coord.y = y;
	}

	Node(Vector2D v) {
		coord.x = v.x;
		coord.y = v.y;
	}

	int info;
	Vector2D coord;
	Node *came_from;
};






