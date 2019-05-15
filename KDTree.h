#pragma once

#include "hitPoint.h"

class KDTreeNode {
public:
	hitPoint *hit;
	Vector minBound, maxBound;
	KDTreeNode *lch, *rch;
	KDTreeNode(hitPoint *_hit);
	void update();
	~KDTreeNode();
};

class KDTree {
	KDTreeNode* build(hitPoint **p, int l, int r, int dim);
	void query(KDTreeNode *node, const hitPoint &p, Vector **canvas, double **prob);
public:
	KDTreeNode *root;
	KDTree();
	void buildTree(hitPoint **p, int hitNum);
	void queryPoint(const hitPoint &p, Vector **canvas, double **prob);
	void deleteTree();
};