#include "bezierCurve.h"

Vector2D::Vector2D(double _x, double _y): x(_x), y(_y) {}

Vector2D Vector2D::operator +(const Vector2D &v) const {
	return Vector2D(x + v.x, y + v.y);
}
Vector2D Vector2D::operator -(const Vector2D &v) const {
	return Vector2D(x - v.x, y - v.y);
}
Vector2D Vector2D::operator *(double k) const {
	return Vector2D(k * x, k * y);
}

bezierCurve::bezierCurve(int _n, Vector2D *_p): n(_n), p(_p) {
	poly = new Vector2D**[n + 1];
	for (int i = 0; i <= n; i++) {
		poly[i] = new Vector2D*[n + 1 - i];
		for (int k = 0; k <= n - i; k++) poly[i][k] = new Vector2D[k + 1];
	}
	for (int i = 0; i <= n; i++) poly[i][0][0] = _p[i];
	for (int k = 1; k <= n; k++) {
		for (int i = 0; i + k <= n; i++) {
			for (int t = 0; t < k; t++)
				poly[i][k][t] = poly[i][k - 1][t];
			for (int t = 1; t <= k; t++)
				poly[i][k][t] = poly[i][k][t] + poly[i + 1][k - 1][t - 1] - poly[i][k - 1][t - 1];
		}
	}
}

/*Vector2D bezierCurve::deCasteljau(int i, int k, double t, Vector2D **pos) {
	if (k == 0) return p[i];
	if (pos[i][k].x < 1e90) return pos[i][k];
	return pos[i][k] = deCasteljau(i, k - 1, t, pos) * (1 - t) + deCasteljau(i + 1, k - 1, t, pos) * t;
}*/
Vector2D bezierCurve::getPos(double t) {
	/*Vector2D **pos = new Vector2D*[n + 1];
	for (int i = 0; i <= n; i++) {
		pos[i] = new Vector2D[n + 1];
		for (int j = 0; j <= n; j++) pos[i][j] = Vector2D(1e100, 1e100);
	}
	Vector2D ans = deCasteljau(0, n, t, pos);
	for (int i = 0; i <= n; i++) delete [] pos[i];
	delete [] pos;
	return ans;*/
	Vector2D ans = poly[0][n][n];
	for (int i = n - 1; i >= 0; i--)
		ans = ans * t + poly[0][n][i];
	return ans;
}
Vector2D bezierCurve::getJacobian(double t) {
	/*Vector2D **pos = new Vector2D*[n + 1];
	for (int i = 0; i <= n; i++) {
		pos[i] = new Vector2D[n + 1];
		for (int j = 0; j <= n; j++) pos[i][j] = Vector2D(1e100, 1e100);
	}
	Vector2D ans = (deCasteljau(1, n - 1, t, pos) - deCasteljau(0, n - 1, t, pos)) * (double)n;
	for (int i = 0; i <= n; i++) delete [] pos[i];
	delete [] pos;
	return ans;*/
	Vector2D ans = poly[1][n - 1][n - 1] - poly[0][n - 1][n - 1];
	for (int i = n - 2; i >= 0; i--)
		ans = ans * t + poly[1][n - 1][i] - poly[0][n - 1][i];
	return ans * (double)n;
}
