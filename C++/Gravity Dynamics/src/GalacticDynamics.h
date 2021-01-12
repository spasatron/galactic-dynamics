#pragma once
#include <iostream>

struct Vec2 {
	double x, y;
	Vec2(double a, double b):x(a), y(b) {}
	Vec2() { x = 0.0; y = 0.0; }

	friend std::ostream& operator<<(std::ostream& os, const Vec2& dt);

};


struct Quad {
	Vec2 lowerLeft;
	double size;

	Quad(Vec2 corner, double length) : lowerLeft(corner), size(length) {}
	Quad() { size = 0; }
	bool contains(double x, double y);
	bool contains(Vec2 pos);

	Quad NW();
	Quad NE();
	Quad SW();
	Quad SE();

};


struct Body {
	Vec2 r;
	Vec2 v;
	Vec2 f;
	double m;

	Body(double rx, double ry, double vx, double vy, double mass) : r(rx, ry), v(vx, vy), f(0, 0), m(mass) {};
	Body(Vec2 pos, Vec2 vel, double mass) : r(pos), v(vel), f(0, 0), m(mass) {}
	Body(): m(0.0) {}

	void update(double dt);
	double distanceTo(Body b);
	void resetForce();
	void addForce(Body b);
	bool in(Quad q);

	static Body addBodies(Body a, Body b);

	bool operator==(Body b);


};





struct BHTreeNode {
	Body body;
	Quad quad;
	BHTreeNode *NW, *NE, *SW, *SE;
	bool internal;
	BHTreeNode() { NW = nullptr; NE = nullptr; SW = nullptr; SE = nullptr; internal = false; }
};


class BHTree {
public:

	BHTree();
	BHTree(Quad quad);
	~BHTree();

	void insertBody(Body b);
	void printSomething();
	void updateForce(Body *b);
	void resetToQuad(Quad q);


private:

	void destroyTreeFromNode(BHTreeNode* node);
	void updateForceAtNode(Body *b, BHTreeNode *node);
	void insertBodyAtNode(Body b, BHTreeNode* node);
	BHTreeNode* root;

};




