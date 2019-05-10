#pragma once

class Vector2D {
public:
	double x, y;
	Vector2D(double _x = 0, double _y = 0);
	Vector2D operator +(const Vector2D &v) const;
	Vector2D operator -(const Vector2D &v) const;
	Vector2D operator *(double k) const;
};

class bezierCurve {
	Vector2D ***poly;
	//Vector2D deCasteljau(int i, int k, double t, Vector2D **pos);
public:
	int n;
	Vector2D *p;
	bezierCurve(int _n = 0, Vector2D *_p = nullptr);
	Vector2D getPos(double t);
	Vector2D getJacobian(double t);
};
