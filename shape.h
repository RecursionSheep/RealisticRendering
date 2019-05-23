#pragma once

#include "texture.h"
#include "util.h"

class Shape {
public:
	Texture tex;
	Shape(Texture _tex): tex(_tex) {}
	virtual double intersect(const Ray &r, Vector &norm, double &a1, double &a2) = 0;
	virtual Vector normal(const Vector &x, const Vector &norm) = 0;
	virtual Vector getColor(const Vector &x, double a1, double a2) = 0;
	virtual Ray getPhoton() = 0;
};
