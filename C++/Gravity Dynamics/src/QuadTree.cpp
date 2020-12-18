#pragma once
#include "QuadTree.h"


static double epsilon = 1e-14;
static double G = 1;
static double theta = .5;
QuadTree::QuadTree(double lowerLeftX, double lowerLeftY, double length)
{
	root = new QuadNode(lowerLeftX, lowerLeftY, length);
}
QuadTree::~QuadTree()
{
	destroyTree();
}

void QuadTree::addBodyToTree(Body b)
{
	addBodyToNode(root, b);
}

Vec2 QuadTree::forceOnBody(Body b)
{
	return forceOnBodyByNode(b, root);
}



void QuadTree::destroyTree()
{
	destroyNode(root);
}

Vec2 operator +(const Vec2& a, const Vec2& b)
{
	return { a.x + b.x, a.y + b.y };
}

Vec2 operator +(const Vec2& a, double b)
{
	return { a.x + b, a.y + b };
}

Vec2 operator *(const Vec2& a, double b)
{
	return { a.x * b, a.y * b };
}

Vec2 operator+(double b, const Vec2& a)
{
	return { a.x + b, a.y + b };
}

Vec2 operator*(double b, const Vec2& a)
{
	return { a.x * b, a.y * b };
}

Vec2 operator /(const Vec2& a, double b)
{
	return { a.x / b, a.y / b };
}

std::ostream& operator<<(std::ostream& os, const Vec2& dt) {
	os << dt.x << ", " << dt.y;
	return os;
}

void QuadTree::updateNodeCenterOfMass(QuadNode* node, Body b)
{
	node->external = true;
	node->centerOfMassPosY = (node->mass * node->centerOfMassPosY + b.yPos * b.mass) / (node->mass + b.mass);
	node->centerOfMassPosX = (node->mass * node->centerOfMassPosX + b.xPos * b.mass) / (node->mass + b.mass);
	node->mass += b.mass;
}

void QuadTree::destroyNode(QuadNode* node)
{
	if (node == nullptr) return;
	destroyNode(node->NE);
	destroyNode(node->NW);
	destroyNode(node->SE);
	destroyNode(node->SW);

	delete node;
}

void QuadTree::addBodyToNode(QuadNode* node, Body b)
{
	updateNodeCenterOfMass(node, b);
	
	//Create New Nodes
	

	//NW
	if (b.xPos <= node->bottomLeftCornerPosX + node->length / 2 && b.yPos >= node->bottomLeftCornerPosY + node->length / 2) {
		if (node->NW == nullptr) {
			node->NW = new QuadNode(node->bottomLeftCornerPosX, node->bottomLeftCornerPosY + node->length / 2, node->length / 2, b);
		}
		else {
			addBodyToNode(node->NW, b);
		}
	}
	//NE
	else if (b.xPos > node->bottomLeftCornerPosX + node->length / 2 && b.yPos >= node->bottomLeftCornerPosY + node->length / 2) {
		if (node->NE == nullptr) {
			node->NE = new QuadNode(node->bottomLeftCornerPosX + node->length/2, node->bottomLeftCornerPosY + node->length / 2, node->length / 2, b);
		}
		else {
			addBodyToNode(node->NE, b);
		}
	}
	//SW
	else if (b.xPos <= node->bottomLeftCornerPosX + node->length / 2 && b.yPos < node->bottomLeftCornerPosY + node->length / 2) {
		if (node->SW == nullptr) {
			node->SW = new QuadNode(node->bottomLeftCornerPosX, node->bottomLeftCornerPosY, node->length / 2, b);
		}
		else {
			addBodyToNode(node->SW, b);
		}
	}
	//SE
	else if (b.xPos > node->bottomLeftCornerPosX + node->length / 2 && b.yPos < node->bottomLeftCornerPosY + node->length / 2) {
		if (node->SE == nullptr) {
			node->SE = new QuadNode(node->bottomLeftCornerPosX + node->length/2, node->bottomLeftCornerPosY, node->length / 2, b);
		}
		else {
			addBodyToNode(node->SE, b);
		}
	}



}

Vec2 QuadTree::forceOnBodyByNode(Body b, QuadNode* node)
{
	Vec2 returnVec(0, 0);
	if (node == nullptr)
		return returnVec;

	returnVec.x = node->centerOfMassPosX - b.xPos;
	returnVec.y = node->centerOfMassPosY - b.yPos;
	if (!(node->external)) {
		if (node->id != b.id) {
			return G * node->mass * b.mass*returnVec/ pow((node->centerOfMassPosX - b.xPos) * (node->centerOfMassPosX - b.xPos) + (node->centerOfMassPosY - b.yPos) * (node->centerOfMassPosY - b.yPos) + epsilon * epsilon, 1.5);
		}
		return { 0, 0 };
	}
	else if (node->length/pow(pow(node->centerOfMassPosX - b.xPos, 2) + pow(node->centerOfMassPosY - b.yPos, 2), .5) <= theta) {
		return G * node->mass * b.mass * returnVec / pow((node->centerOfMassPosX - b.xPos) * (node->centerOfMassPosX - b.xPos) + (node->centerOfMassPosY - b.yPos) * (node->centerOfMassPosY - b.yPos) + epsilon * epsilon, 1.5);
	}
	else {
		//std::cout << "NW: " << forceOnBodyByNode(b, node->NW) << " " << forceOnBodyByNode(b, node->NE) << " " << forceOnBodyByNode(b, node->SW) << " " << forceOnBodyByNode(b, node->SE) << std::endl;
			return forceOnBodyByNode(b, node->NW) + forceOnBodyByNode(b, node->NE) + forceOnBodyByNode(b, node->SW) + forceOnBodyByNode(b, node->SE);
	}
}
