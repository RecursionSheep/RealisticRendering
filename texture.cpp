#include "texture.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif // STB_IMAGE_IMPLEMENTATION

Texture::Texture(char *path, Vector _emiss, Vector _color, int _ref):
	emiss(_emiss), color(_color), ref(_ref) {
	if (path == nullptr) {
		buf = nullptr;
		return;
	}
	buf = stbi_load(path, &w, &h, &c, 0);
}
Vector Texture::getColor(double px, double py) {
	if (buf == nullptr) return color;
	int pw = (int(px * w) % w + w) % w, ph = (int(py * h) % h + h) % h;
	int idx = ph * w * c + pw * c;
	int x = buf[idx + 0], y = buf[idx + 1], z = buf[idx + 2];
	return Vector(x, y, z) * (1. / 255);
}
