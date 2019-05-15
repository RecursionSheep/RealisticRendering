#include "KDTree.h"

#include <algorithm>

KDTreeNode::KDTreeNode(hitPoint *_hit): hit(_hit) {
	minBound = _hit->pos - Vector(_hit->rad, _hit->rad, _hit->rad);
	maxBound = _hit->pos + Vector(_hit->rad, _hit->rad, _hit->rad);
	lch = rch = nullptr;
}

void KDTreeNode::update() {
	minBound = hit->pos - Vector(hit->rad, hit->rad, hit->rad);
	maxBound = hit->pos + Vector(hit->rad, hit->rad, hit->rad);
	if (lch != nullptr) {
		minBound = min(minBound, lch->minBound);
		maxBound = max(maxBound, lch->maxBound);
	}
	if (rch != nullptr) {
		minBound = min(minBound, rch->minBound);
		maxBound = max(maxBound, rch->maxBound);
	}
}

KDTreeNode::~KDTreeNode() {
	if (lch != nullptr) delete lch;
	if (rch != nullptr) delete rch;
}

inline bool cmp(hitPoint *a, hitPoint *b) {
	return a->cmpDim < b->cmpDim;
}

KDTree::KDTree() {
	root = nullptr;
}

KDTreeNode* KDTree::build(hitPoint **p, int l, int r, int dim) {
	for (int i = l; i <= r; i++)
		p[i]->cmpDim = (dim == 0) ? (p[i]->pos.x) : ((dim == 1) ? (p[i]->pos.y) : (p[i]->pos.z));
	int mid = (l + r) >> 1;
	std::nth_element(p + l, p + mid, p + r + 1, cmp);
	KDTreeNode *root = new KDTreeNode(p[mid]);
	if (l < mid) root->lch = build(p, l, mid - 1, (dim == 2) ? 0 : (dim + 1));
	if (mid < r) root->rch = build(p, mid + 1, r, (dim == 2) ? 0 : (dim + 1));
	root->update();
	return root;
}

void KDTree::query(KDTreeNode *node, const hitPoint &p, Vector **canvas, double **prob) {
	if (((node->hit->pos - p.pos).len() <= node->hit->rad) && (node->hit->dir.dot(p.dir) >= -1e-5)) {
		canvas[node->hit->indexY][node->hit->indexX] += node->hit->col.dot(p.col);
		prob[node->hit->indexY][node->hit->indexX] += p.prob * node->hit->prob;
	}
	if (node->lch != nullptr) {
		KDTreeNode *child = node->lch;
		if (p.pos > child->minBound - Vector(1e-5, 1e-5, 1e-5) && p.pos < child->maxBound + Vector(1e-5, 1e-5, 1e-5)) query(child, p, canvas, prob);
	}
	if (node->rch != nullptr) {
		KDTreeNode *child = node->rch;
		if (p.pos > child->minBound - Vector(1e-5, 1e-5, 1e-5) && p.pos < child->maxBound + Vector(1e-5, 1e-5, 1e-5)) query(child, p, canvas, prob);
	}
}

void KDTree::buildTree(hitPoint **p, int hitNum) {
	root = build(p, 0, hitNum - 1, 0);
}

void KDTree::queryPoint(const hitPoint &p, Vector **canvas, double **prob) {
	query(root, p, canvas, prob);
}

void KDTree::deleteTree() {
	delete root;
}
