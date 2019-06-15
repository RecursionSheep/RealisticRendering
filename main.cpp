#include <bits/stdc++.h>

#include "render.h"

using namespace std;

int main() {
	int width = 1024, height = 768, sampling = 50;
	Vector **canvas = new Vector*[height];
	for (int i = 0; i < height; i++) canvas[i] = new Vector[width];
	//PathTracing(canvas, width, height, sampling);
	ProgressivePhotonMapping(canvas, width, height, 500000, 100);
	FILE *f = fopen("image.ppm", "w");
	fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
	for (int i = height - 1; i >= 0; i--)
		for (int j = 0; j < width; j++)
			fprintf(f, "%d %d %d ", colorToInt(canvas[i][j].x), colorToInt(canvas[i][j].y), colorToInt(canvas[i][j].z));
	return 0;
}
