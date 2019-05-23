#pragma once

#include "bezierCurve.h"
#include "sphere.h"
#include "shape.h"

class bezierSpin: public Shape {
public:
	Vector p;
	bezierCurve *curve;
	Shape *bound;
	bezierSpin(Vector _p, bezierCurve *_curve, Texture _tex);
	Vector getPos(double t, double theta);
	double intersect(const Ray &r, Vector &norm, double &a1, double &a2);
	Vector normal(const Vector &x, const Vector &norm);
	Vector getColor(const Vector &x, double a1, double a2);
	Ray getPhoton();
};
