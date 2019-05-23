#include <cmath>
#include <iostream>

#include "bezierSpin.h"

bezierSpin::bezierSpin(Vector _p, bezierCurve *_curve, Texture _tex):
	Shape(_tex), p(_p), curve(_curve) {
	double maxy = 0;
	for (int i = 0; i <= _curve->n; i++) maxy = std::max(maxy, _curve->p[i].y);
	Vector center = _p + Vector(0, maxy / 2, 0);
	double r = 0;
	for (int i = 0; i <= _curve->n; i++) r = std::max(r, (Vector(_curve->p[i].x, _curve->p[i].y, 0) + _p - center).len() + 1e-3);
	for (int it = 0; it < 500; it++) {
		Vector _center = _p + Vector(0, maxy * randR(), 0);
		double _r = 0;
		for (int i = 0; i <= _curve->n; i++) _r = std::max(_r, (Vector(_curve->p[i].x, _curve->p[i].y, 0) + _p - _center).len() + 1e-3);
		if (_r < r) {
			r = _r;
			center = _center;
		}
	}
	bound = new Sphere(r, center, Texture(nullptr, Vector(), Vector(), 0));
}

Vector bezierSpin::getPos(double t, double theta) {
	Vector2D p2 = curve->getPos(t);
	return p + Vector(p2.x * cos(theta), p2.y, p2.x * sin(theta));
}

double bezierSpin::intersect(const Ray &r, Vector &norm, double &w1, double &w2) {
	double t1 = bound->intersect(r, norm, w1, w2);
	if (t1 <= 0) return -1;
	bool converge = false;
	Vector sol(1e100);
	int it_num = 5;
	for (int it = 0; it < it_num; it++) {
		Vector x(t1, (double)it / it_num, (double)it / it_num * pi * 2);
		for (int k = 0; k < 20; k++) {
			Vector2D pos = curve->getPos(x.y);
			Vector2D dir = curve->getJacobian(x.y);
			Vector f = r.o + r.d * x.x - getPos(x.y, x.z);
			Vector a1 = r.d, a2 = Vector(-dir.x * cos(x.z), -dir.y, -dir.x * sin(x.z)), a3 = Vector(pos.x * sin(x.z), 0, -pos.x * cos(x.z));
			if (fabs(f.x) < 1e-4 && fabs(f.y) < 1e-4 && fabs(f.z) < 1e-4) {
				if (x.y < 0 || x.y > 1 || x.x < 0) break;
				if (x.x < sol.x - 1e-3) {
					w2 = x.y; w1 = x.z / 2 / pi;
					sol = x; norm = a3.cross(a2).unit();
					if (converge) return sol.x;
					converge = true; break;
				} else {
					if (converge) return sol.x;
				}
			}
			double dom = a1.cross(a2).dot(a3);
			if (fabs(dom) < 1e-4) break;
			Vector delta(f.cross(a2).dot(a3), a1.cross(f).dot(a3), a1.cross(a2).dot(f));
			delta = delta * (1. / dom);
			x = x - delta;
			if (x.x < -10 && delta.x > 0) break;
		}
	}
	if (converge) return sol.x;
	return -1;
}

Vector bezierSpin::normal(const Vector &x, const Vector &norm) {
	return norm;
}

Vector bezierSpin::getColor(const Vector &x, double a1, double a2) {
	return tex.getColor(a1, a2);
}

Ray bezierSpin::getPhoton() {
	return Ray(Vector(0, 0, 0), Vector(0, 0, 0));
}
