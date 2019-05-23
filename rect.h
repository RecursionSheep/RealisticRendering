#pragma once

#include "util.h"
#include "shape.h"

class Rect: public Shape {
public:
	Vector p;
	Vector w, h;
	Rect(Vector _p, Vector _w, Vector _h, Texture _tex);
	double intersect(const Ray &r, Vector &norm, double &a1, double &a2);
	Vector normal(const Vector &x, const Vector &norm);
	Vector getColor(const Vector &x, double a1, double a2);
	Ray getPhoton();
};
