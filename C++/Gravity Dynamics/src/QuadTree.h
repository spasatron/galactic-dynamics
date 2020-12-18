#pragma once
#include "Body.h"
#include <iostream>

struct QuadNode {
	bool external;
	double mass;
	int id;
	double centerOfMassPosX, centerOfMassPosY, bottomLeftCornerPosX, bottomLeftCornerPosY, length;
	QuadNode* NW;
	QuadNode* NE;
	QuadNode* SW;
	QuadNode* SE;

	//TO DO: Write this function
	bool contains(Body b) {
		return true;
	}

	QuadNode(double cornerX, double cornerY, double l, Body b) : bottomLeftCornerPosX(cornerX), bottomLeftCornerPosY(cornerY), length(l) {
		centerOfMassPosX = b.xPos;
		centerOfMassPosY = b.yPos;
		mass = b.mass;
		external = false;
		id = b.id;
		NW = nullptr;
		NE = nullptr;
		SW = nullptr;
		SE = nullptr;
	}
	QuadNode(double cornerX, double cornerY, double l) : bottomLeftCornerPosX(cornerX), bottomLeftCornerPosY(cornerY), length(l) {
		centerOfMassPosX = 0;
		centerOfMassPosY = 0;
		mass = 0;
		external = false;
		id = -1;
		NW = nullptr;
		NE = nullptr;
		SW = nullptr;
		SE = nullptr;
	}

};

struct Vec2 {
public:
	double x = 0.0, y = 0.0;
	friend Vec2 operator+(const Vec2& a, const Vec2& b);
	friend Vec2 operator+(const Vec2& a, double b);
	friend Vec2 operator*(const Vec2& a, double b);
	friend Vec2 operator+(double b, const Vec2& a);
	friend Vec2 operator*(double b, const Vec2& a);
	friend Vec2 operator/(const Vec2& a, double b);
	friend std::ostream& operator<<(std::ostream& os, const Vec2& dt);
	

	double norm2squared() {
		return x * x + y * y;
	}


	Vec2() {}
	Vec2(double a, double b) : x(a), y(b) {}


};




class QuadTree {
public:
	QuadTree(double lowerLeftX, double lowerLeftY, double length);
	~QuadTree();

	void addBodyToTree(Body b);
	Vec2 forceOnBody(Body b);
	void destroyTree();

private:
	void updateNodeCenterOfMass(QuadNode* node, Body b);
	void destroyNode(QuadNode* node);
	void addBodyToNode(QuadNode* node, Body b);
	Vec2 forceOnBodyByNode(Body b, QuadNode* node);
	QuadNode* root;
};