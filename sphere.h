#pragma once

#include "shape.h"
#include "util.h"

class Sphere: public Shape {
public:
	double r;
	Vector center;
	Sphere(double _r, Vector _c, Texture _tex);
	double intersect(const Ray &r, Vector &norm, double &a1, double &a2);
	Vector normal(const Vector &x, const Vector &norm);
	Vector getColor(const Vector &x, double a1, double a2);
	Ray getPhoton();
};
