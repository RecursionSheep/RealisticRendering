#include "util.h"

Vector::Vector(double _x, double _y, double _z): x(_x), y(_y), z(_z) {}

Vector Vector::operator +(const Vector &v) const {
	return Vector(x + v.x, y + v.y, z + v.z);
}
Vector Vector::operator -(const Vector &v) const {
	return Vector(x - v.x, y - v.y, z - v.z);
}
Vector Vector::operator *(double k) const {
	return Vector(k * x, k * y, k * z);
}
Vector& Vector::operator +=(const Vector &v) {
	return *this = *this + v;
}
bool Vector::operator <(const Vector &v) const {
	return x < v.x && y < v.y && z < v.z;
}
bool Vector::operator >(const Vector &v) const {
	return x > v.x && y > v.y && z > v.z;
}
Vector Vector::mul(const Vector &v) const {
	return Vector(x * v.x, y * v.y, z * v.z);
}
double Vector::dot(const Vector &v) const {
	return x * v.x + y * v.y + z * v.z;
}
Vector Vector::cross(const Vector &v) const {
	return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}
double Vector::len() const {
	return std::sqrt(x * x + y * y + z * z);
}
Vector& Vector::unit() {
	return *this = *this * (1 / sqrt(x * x + y * y + z * z));
}

Vector min(const Vector &a, const Vector &b) {
	return Vector(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
}
Vector max(const Vector &a, const Vector &b) {
	return Vector(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}

Ray::Ray(Vector _o, Vector _d): o(_o), d(_d) {}

double clamp(double x) {
	return x < 0 ? 0 : x > 1 ? 1 : x;
}
int colorToInt(double x) {
	return int(pow(clamp(x), 1 / 2.2) * 255 + .5);
}
double randR() {
	return (double)rand() / RAND_MAX;
}
