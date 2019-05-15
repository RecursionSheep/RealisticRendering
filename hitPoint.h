#pragma once

#include "util.h"

class hitPoint {
public:
	Vector pos, dir;
	Vector col;
	int indexX, indexY;
	double prob, rad;
	double cmpDim;
	hitPoint(Vector _pos = Vector(), Vector _dir = Vector(), Vector _col = Vector(), int _indexX = -1, int _indexY = -1, double _prob = 1.0, double _rad = 1.0);
};
