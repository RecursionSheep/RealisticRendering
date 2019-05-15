#pragma once

#include <cmath>
#include <algorithm>

const double pi = acos(-1.);

class Vector {
public:
	double x, y, z;
	Vector(double _x = 0, double _y = 0, double _z = 0);
	Vector operator +(const Vector &v) const;
	Vector operator -(const Vector &v) const;
	Vector operator *(double k) const;
	Vector& operator +=(const Vector &v);
	bool operator <(const Vector &v) const;
	bool operator >(const Vector &v) const;
	Vector mul(const Vector &v) const;
	double dot(const Vector &v) const;
	Vector cross(const Vector &v) const;
	double len() const;
	Vector& unit();
};

Vector min(const Vector &a, const Vector &b);
Vector max(const Vector &a, const Vector &b);

class Ray {
public:
	Vector o, d;
	Ray(Vector _o, Vector _d);
};

double clamp(double x);
int colorToInt(double x);
double randR();
