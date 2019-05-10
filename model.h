#pragma once

#include "util.h"
#include "bezierSpin.h"
#include "rect.h"
#include "sphere.h"

const int model_num = 9;

Vector2D plist[5] = {Vector2D(6.116, 0.), Vector2D(22.382, 29.124), Vector2D(14.947, 44.816), Vector2D(0.134, 24.495), Vector2D(5.055, 48.989),};
bezierCurve *curve = new bezierCurve(4, plist);
Shape* model[] = {
	new Rect(Vector(1, 81.6, 170), Vector(0, 0, -170), Vector(0, -81.6, 0), Texture("background.png", Vector(), Vector(.75, .25, .25), DIFF)),
	//new Sphere(1e5, Vector(1e5 + 1, 40.8, 81.6), Texture(nullptr, Vector(), Vector(.75, .25, .25), DIFF)),
	new Sphere(1e5, Vector(-1e5 + 99, 40.8, 81.6), Texture(nullptr, Vector(), Vector(.25, .25, .75), DIFF)),
	//new Sphere(1e5, Vector(50, 40.8, 1e5), Texture(nullptr, Vector(), Vector(.75, .75, .75), DIFF)),
	new Rect(Vector(1, 81.6, 0), Vector(98, 0, 0), Vector(0, -81.6, 0), Texture(nullptr, Vector(), Vector(.75, .75, .75), DIFF)),
	new Sphere(1e5, Vector(50, 40.8, -1e5 + 170), Texture(nullptr, Vector(), Vector(), DIFF)),
	new Sphere(1e5, Vector(50, 1e5, 81.6), Texture(nullptr, Vector(), Vector(.75, .75, .75), DIFF)),
	new Sphere(1e5, Vector(50, -1e5 + 81.6, 81.6), Texture(nullptr, Vector(), Vector(.75, .75, .75), DIFF)),
	new Sphere(16.5, Vector(27, 16.5, 47), Texture(nullptr, Vector(), Vector(1, 1, 1) * .999, SPEC)),
	//new Sphere(16.5, Vector(73, 16.5, 78), Texture(nullptr, Vector(), Vector(1, 1, 1) * .999, REFR)),
	new bezierSpin(Vector(73, 0, 88), curve, Texture("vase2.jpg", Vector(), Vector(.75, .25, .25), DIFF)),
	new Sphere(600, Vector(50, 681.6 - .27, 91.6), Texture(nullptr, Vector(12, 12, 12), Vector(), DIFF))
};
