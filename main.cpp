#include <bits/stdc++.h>

#include "rt.h"

using namespace std;

int main() {
	int width = 1024, height = 768, sampling = 80;
	Vector **canvas = new Vector*[height];
	for (int i = 0; i < height; i++) canvas[i] = new Vector[width];
	RayTracing(canvas, width, height, sampling);
	FILE *f = fopen("image.ppm", "w");
	fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
	for (int i = height - 1; i >= 0; i--)
		for (int j = 0; j < width; j++)
			fprintf(f, "%d %d %d ", colorToInt(canvas[i][j].x), colorToInt(canvas[i][j].y), colorToInt(canvas[i][j].z));
	return 0;
}
