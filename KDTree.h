#pragma once

#include "hitPoint.h"

const double alpha = 0.8;

class KDTreeNode {
public:
	hitPoint *hit;
	Vector minBound, maxBound;
	int photonTot, photonCnt;
	KDTreeNode *lch, *rch;
	KDTreeNode(hitPoint *_hit);
	void update();
	~KDTreeNode();
};

class KDTree {
	KDTreeNode* build(hitPoint **p, int l, int r, int dim);
	void query(KDTreeNode *node, const hitPoint &p, Vector **canvas, double **cnt);
	void modify(KDTreeNode *node);
public:
	KDTreeNode *root;
	KDTree();
	void buildTree(hitPoint **p, int hitNum);
	void queryPoint(const hitPoint &p, Vector **canvas, double **cnt);
	void modifyRadius();
	void deleteTree();
};
