#pragma once

#include "util.h"

#define DIFF 1
#define SPEC 2
#define REFR 3

class Texture {
public:
	unsigned char *buf;
	Vector emiss, color;
	int ref;
	int w, h, c;
	Texture(char *path, Vector _emiss, Vector _color, int _ref);
	Vector getColor(double px, double py);
};
