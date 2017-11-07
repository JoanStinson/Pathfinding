#pragma once
#include "Vector2D.h"

class Node {
public:
	int info;
	Vector2D coord;
	//vector<Node*> neighbors;
	Node *came_from;
	Node() {

	}
	Node(float x, float y){
		coord.x = x;
		coord.y = y;
	}
};


