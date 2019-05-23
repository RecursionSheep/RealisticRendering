#include <cmath>

#include "rect.h"

Rect::Rect(Vector _p, Vector _w, Vector _h, Texture _tex):
	Shape(_tex), p(_p), w(_w), h(_h) {}

double Rect::intersect(const Ray &r, Vector &norm, double &a1, double &a2) {
	//r.o+r.d*t=p.p+p.w*alpha+p.h*beta
	//                t
	//(r.d -p.w -p.h)(alpha)=p.p-r.o
	//                beta
	double dom = r.d.cross(w * -1.).dot(h * -1.);
	if (fabs(dom) < 1e-4) return -1;
	double t = (p - r.o).cross(w * -1.).dot(h * -1.) / dom;
	if (t < 1e-4) return -1;
	double alpha = r.d.cross(p - r.o).dot(h * -1.) / dom;
	if (alpha < 1e-4 || alpha > 1 - 1e-4) return -1;
	double beta = r.d.cross(w * -1.).dot(p - r.o) / dom;
	if (beta < 1e-4 || beta > 1 - 1e-4) return -1;
	return t;
}

Vector Rect::normal(const Vector &x, const Vector &norm) {
	return h.cross(w).unit();
}

Vector Rect::getColor(const Vector &x, double a1, double a2) {
	double alpha = (x - p).dot(w) / w.len() / w.len(), beta = (x - p).dot(h) / h.len() / h.len();
	return tex.getColor(alpha, beta);
}

Ray Rect::getPhoton() {
	Vector o = p + w * randR() + h * randR();
	Vector nl = h.cross(w).unit();
	double r1 = 2 * pi * randR(), r2 = randR(), r2s = sqrt(r2);
	Vector u = ((fabs(nl.x) > .1 ? Vector(0, 1) : Vector(1)).cross(nl)).unit(), v = nl.cross(u);
	Vector d = (u * cos(r1) * r2s + v * sin(r1) * r2s + nl * sqrt(1 - r2)).unit();
	return Ray(o, d);
}
