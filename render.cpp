#include <cstdio>
#include <cmath>
#include <cassert>
#include <iostream>
#include <vector>

#include "render.h"
#include "KDTree.h"
#include "hitPoint.h"
#include "model.h"

const double aperture = 0;

inline bool findHit(const Ray &r, double &mind, int &id, Vector &norm, double &a1, double &a2) {
	int n = model_num;
	double inf = mind = 1e20;
	for (int i = 0; i < n; i++) {
		Vector _norm;
		double _a1, _a2;
		double dist = model[i]->intersect(r, _norm, _a1, _a2);
		if (dist > 0 && dist < mind) {
			mind = dist; id = i; norm = _norm; a1 = _a1; a2 = _a2;
		}
	}
	
	return mind < inf;
}

Vector radiance(const Ray &_r, int depth) {
	Ray r = _r;
	r.o = r.o + r.d * 1e-3;
	double dist;
	int id = 0;
	Vector norm;
	double a1, a2;
	if (!findHit(r, dist, id, norm, a1, a2)) return Vector();
	Shape *obj = model[id];
	Vector x = r.o + r.d * dist;
	Vector n = obj->normal(x, norm), nl = n.dot(r.d) < 0 ? n : n * -1, f = obj->getColor(x, a1, a2);
	double p = std::max(f.x, std::max(f.y, f.z));
	depth++;
	if (depth > 5)
		if (randR() < p && depth < 30) f = f * (1. / p); else return obj->tex.emiss;
	if (obj->tex.ref == DIFF) {
		double r1 = 2 * pi * randR(), r2 = randR(), r2s = sqrt(r2);
		Vector w = nl, u = ((fabs(w.x) > .1 ? Vector(0, 1) : Vector(1)).cross(w)).unit(), v = w.cross(u);
		Vector d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).unit();
		return obj->tex.emiss + f.mul(radiance(Ray(x, d), depth));
	} else if (obj->tex.ref == SPEC)
		return obj->tex.emiss + f.mul(radiance(Ray(x, r.d - n * 2 * n.dot(r.d)), depth));
	Ray reflRay(x, r.d - n * 2 * n.dot(r.d));
	bool into = n.dot(nl) > 0;
	double nc = 1, nt = 1.5, nnt = into ? nc / nt : nt / nc, ddn = r.d.dot(nl), cos2t;
	if ((cos2t = 1 - nnt * nnt * (1 - ddn * ddn)) < 0)
		return obj->tex.emiss + f.mul(radiance(reflRay, depth));
	Vector tdir = (r.d * nnt - n * ((into ? 1 : -1) * (ddn * nnt + sqrt(cos2t)))).unit();
	double a = nt - nc, b = nt + nc, R0 = a * a / (b * b), c = 1 - (into ? -ddn : tdir.dot(n));
	double Re = R0 + (1 - R0) * c * c * c * c * c, Tr = 1 - Re, P = .25 + .5 * Re, RP = Re / P, TP = Tr / (1 - P);
	return obj->tex.emiss + f.mul(depth > 2 ? (randR() < P ? radiance(reflRay, depth) * RP :
		radiance(Ray(x, tdir), depth) * TP) :
		radiance(reflRay, depth) * Re + radiance(Ray(x, tdir), depth) * Tr);
}

void PathTracing(Vector **canvas, int width, int height, int sampling) {
	Ray camera(Vector(50, 52, 295.6), Vector(0, -0.042612, -1).unit());
	Vector cx = Vector(width * .5135 / height), cy = (cx.cross(camera.d)).unit() * .5135, r;
	fprintf(stderr, "\rRendering (%d spp) %5.2f%%", sampling * 4, 0.);
#pragma omp parallel for schedule(dynamic, 1) private(r)
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			//printf("%d %d\n", x, y);
			for (int sy = 0; sy < 2; sy++)
				for (int sx = 0; sx < 2; sx++) {
					Vector r;
					for (int s = 0; s < sampling; s++) {
						double r1 = 2 * randR(), dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1);
						double r2 = 2 * randR(), dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2);
						Vector d = cx * (((sx + .5 + dx) / 2 + x) / width - .5) + cy * (((sy + .5 + dy) / 2 + y) / height - .5) + camera.d;
						Vector pos = camera.o + d * 140, loc = camera.o + Vector(randR() - .5, randR() - .5, 0) * 2 * aperture;
						r = r + radiance(Ray(pos, (pos - loc).unit()), 0) * (1.0 / sampling);
					}
					canvas[y][x] += Vector(clamp(r.x), clamp(r.y), clamp(r.z)) * 0.25;
				}
		}
#pragma omp critical
		fprintf(stderr, "\rRendering (%d spp) %5.2f%%", sampling * 4, 100. * y / (height - 1));
	}
}

void radiance1(const Ray &_r, int depth, double prob, Vector col, int indexX, int indexY, std::vector<hitPoint> *hitpoints) {
	Ray r = _r;
	r.o = r.o + r.d * 1e-3;
	double dist;
	int id = 0;
	Vector norm;
	double a1, a2;
	if (!findHit(r, dist, id, norm, a1, a2)) return;
	Shape *obj = model[id];
	Vector x = r.o + r.d * dist;
	Vector n = obj->normal(x, norm), nl = n.dot(r.d) < 0 ? n : n * -1, f = obj->getColor(x, a1, a2);
	double p = std::max(f.x, std::max(f.y, f.z));
	if (p < 1e-4 || prob < 1e-4) return;
	depth++;
	if (depth > 5)
		if (randR() < p && depth < 30) f = f * (1. / p); else return;
	if (obj->tex.ref == DIFF) {
#pragma omp critical
		hitpoints->push_back(hitPoint(x, nl, col.mul(f), indexX, indexY, prob));
		return;
	} else if (obj->tex.ref == SPEC) {
		radiance1(Ray(x, r.d - n * 2 * n.dot(r.d)), depth, prob, col.mul(f), indexX, indexY, hitpoints);
		return;
	}
	Ray reflRay(x, r.d - n * 2 * n.dot(r.d));
	bool into = n.dot(nl) > 0;
	double nc = 1, nt = 1.5, nnt = into ? nc / nt : nt / nc, ddn = r.d.dot(nl), cos2t;
	if ((cos2t = 1 - nnt * nnt * (1 - ddn * ddn)) < 0) {
		radiance1(reflRay, depth, prob, col.mul(f), indexX, indexY, hitpoints);
		return;
	}
	Vector tdir = (r.d * nnt - n * ((into ? 1 : -1) * (ddn * nnt + sqrt(cos2t)))).unit();
	double a = nt - nc, b = nt + nc, R0 = a * a / (b * b), c = 1 - (into ? -ddn : tdir.dot(n));
	double Re = R0 + (1 - R0) * c * c * c * c * c, Tr = 1 - Re, P = .25 + .5 * Re, RP = Re / P, TP = Tr / (1 - P);
	if (depth <= 2) {
		radiance1(reflRay, depth, prob * Re, col.mul(f), indexX, indexY, hitpoints);
		radiance1(Ray(x, tdir), depth, prob * Tr, col.mul(f), indexX, indexY, hitpoints);
		return;
	} else {
		if (randR() < P)
			radiance1(reflRay, depth, prob * RP, col.mul(f), indexX, indexY, hitpoints);
		else
			radiance1(Ray(x, tdir), depth, prob * TP, col.mul(f), indexX, indexY, hitpoints);
		return;
	}
}

void radiance2(const Ray &_r, int depth, double prob, Vector col, Vector **canvas, double **cnt, KDTree *hitTree) {
	Ray r = _r;
	r.o = r.o + r.d * 1e-3;
	double dist;
	int id = 0;
	Vector norm;
	double a1, a2;
	if (!findHit(r, dist, id, norm, a1, a2)) return;
	Shape *obj = model[id];
	Vector x = r.o + r.d * dist;
	Vector n = obj->normal(x, norm), nl = n.dot(r.d) < 0 ? n : n * -1, f = obj->getColor(x, a1, a2);
	double p = std::max(f.x, std::max(f.y, f.z));
	if (p < 1e-4 || prob < 1e-4) return;
	depth++;
	if (depth > 5)
		if (randR() < p && depth < 30) f = f * (1. / p); else return;
	if (obj->tex.ref == DIFF) {
		hitTree->queryPoint(hitPoint(x, nl, col, -1, -1, prob), canvas, cnt);
		double r1 = 2 * pi * randR(), r2 = randR(), r2s = sqrt(r2);
		Vector w = nl, u = ((fabs(w.x) > .1 ? Vector(0, 1) : Vector(1)).cross(w)).unit(), v = w.cross(u);
		Vector d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).unit();
		radiance2(Ray(x, d), depth, prob, f.mul(col), canvas, cnt, hitTree);
		return;
	} else if (obj->tex.ref == SPEC) {
		radiance2(Ray(x, r.d - n * 2 * n.dot(r.d)), depth, prob, col.mul(f), canvas, cnt, hitTree);
		return;
	}
	Ray reflRay(x, r.d - n * 2 * n.dot(r.d));
	bool into = n.dot(nl) > 0;
	double nc = 1, nt = 1.5, nnt = into ? nc / nt : nt / nc, ddn = r.d.dot(nl), cos2t;
	if ((cos2t = 1 - nnt * nnt * (1 - ddn * ddn)) < 0) {
		radiance2(reflRay, depth, prob, col.mul(f), canvas, cnt, hitTree);
		return;
	}
	Vector tdir = (r.d * nnt - n * ((into ? 1 : -1) * (ddn * nnt + sqrt(cos2t)))).unit();
	double a = nt - nc, b = nt + nc, R0 = a * a / (b * b), c = 1 - (into ? -ddn : tdir.dot(n));
	double Re = R0 + (1 - R0) * c * c * c * c * c, Tr = 1 - Re, P = .25 + .5 * Re, RP = Re / P, TP = Tr / (1 - P);
	if (depth <= 2) {
		radiance2(reflRay, depth, prob * Re, col.mul(f), canvas, cnt, hitTree);
		radiance2(Ray(x, tdir), depth, prob * Tr, col.mul(f), canvas, cnt, hitTree);
		return;
	} else {
		if (randR() < P)
			radiance2(reflRay, depth, prob * RP, col.mul(f), canvas, cnt, hitTree);
		else
			radiance2(Ray(x, tdir), depth, prob * TP, col.mul(f), canvas, cnt, hitTree);
		return;
	}
}

void ProgressivePhotonMapping(Vector **canvas, int width, int height, int photonNum, int roundNum) {
	Ray camera(Vector(50, 52, 295.6), Vector(0, -0.042612, -1).unit());
	Vector cx = Vector(width * .5135 / height), cy = (cx.cross(camera.d)).unit() * .5135, r;
	fprintf(stderr, "Ray tracing phase %5.2f%%", 0.);
	std::vector<hitPoint> points;
#pragma omp parallel for schedule(dynamic, 1)
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			//printf("%d %d\n", x, y);
			for (int sy = 0; sy < 2; sy++)
				for (int sx = 0; sx < 2; sx++) {
					double r1 = 2 * randR(), dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1);
					double r2 = 2 * randR(), dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2);
					Vector d = cx * (((sx + .5 + dx) / 2 + x) / width - .5) + cy * (((sy + .5 + dy) / 2 + y) / height - .5) + camera.d;
					Vector pos = camera.o + d * 140, loc = camera.o + Vector(randR() - .5, randR() - .5, 0) * 2 * aperture;
					radiance1(Ray(pos, (pos - loc).unit()), 0, .25, Vector(1., 1., 1.), x, y, &points);
				}
		}
#pragma omp critical
		fprintf(stderr, "\rRay tracing phase %5.2f%%", 100. * y / (height - 1));
	}
	KDTree hitTree;
	double **cnt = new double*[height];
	for (int i = 0; i < height; i++) {
		cnt[i] = new double[width];
		for (int j = 0; j < width; j++) cnt[i][j] = 0.;
	}
	int hitNum = points.size();
	hitPoint **hitpoints = new hitPoint*[hitNum];
	for (int i = 0; i < hitNum; i++) hitpoints[i] = &points[i];
	hitTree.buildTree(hitpoints, hitNum);
	fprintf(stderr, "\nPhoton mapping phase %5.2f%%", 0.);
	for (int round = 0; round < roundNum; round++) {
		for (int photonCnt = 0; photonCnt < photonNum; photonCnt++) {
			for (int i = 0; i < model_num; i++) if (model[i]->tex.emiss > Vector(1e-4, 1e-4, 1e-4)) {
				Ray ray = model[i]->getPhoton();
				//printf("%.2f %.2f %.2f\n", ray.o.x, ray.o.y, ray.o.z);
				hitTree.queryPoint(hitPoint(ray.o, model[i]->normal(Vector(), Vector()), model[i]->tex.emiss), canvas, cnt);
				radiance2(ray, 0, 1., model[i]->tex.emiss, canvas, cnt, &hitTree);
			}
		}
		hitTree.modifyRadius();
		fprintf(stderr, "\rPhoton mapping phase %5.2f%%", 100. * (double)(round + 1) / roundNum);
		char *filename = new char[13];
		sprintf(filename, "temp%d.ppm", round);
		FILE *f = fopen(filename, "w");
		fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
		for (int i = height - 1; i >= 0; i--)
			for (int j = 0; j < width; j++) {
				Vector pixel = canvas[i][j];
				if (cnt[i][j] >= 1e-5) pixel = pixel * (1. / cnt[i][j]);
				else pixel = Vector(0, 0, 0);
				fprintf(f, "%d %d %d ", colorToInt(pixel.x), colorToInt(pixel.y), colorToInt(pixel.z));
			}
	}
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) {
			if (cnt[i][j] >= 1e-5)
				canvas[i][j] = canvas[i][j] * (1. / cnt[i][j]);
			else
				canvas[i][j] = Vector(0., 0., 0.);
			//printf("%.2f %.2f %.2f\n", canvas[i][j].x, canvas[i][j].y, canvas[i][j].z);
		}
	hitTree.deleteTree();
}
