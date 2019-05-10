#include <cmath>
#include <iostream>

#include "sphere.h"

Sphere::Sphere(double _r, Vector _c, Texture _tex):
	Shape(_tex), r(_r), center(_c) {}

double Sphere::intersect(const Ray &r, Vector &norm, double &a1, double &a2) {
	Vector l = center - r.o;
	double t_p = l.dot(r.d);
	double d = l.dot(l) - t_p * t_p;
	if (d > this->r * this->r + 1e-4) return -1;
	double t_prime = sqrt(this->r * this->r - d);
	if (t_p > t_prime + 1e-4) return t_p - t_prime; else return std::max(-1., t_p + t_prime);
	//if (l.len() + 1e-4 < this->r) return std::max(-1., t_p + t_prime); else return std::max(-1., t_p - t_prime);
}
/*double intersect(const Ray &r, Vector &norm, double &a1, double &a2) {
	Vector op = center-r.o; // Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
	double t, eps=1e-4, b=op.dot(r.d), det=b*b-op.dot(op)+this->r*this->r;
	if (det<0) return 0; else det=sqrt(det);
	return (t=b-det)>eps ? t : ((t=b+det)>eps ? t : 0);
}*/

Vector Sphere::normal(const Vector &x, const Vector &norm) {
	return (x - center).unit();
}

Vector Sphere::getColor(const Vector &x, double a1, double a2) {
	return tex.color;
}
