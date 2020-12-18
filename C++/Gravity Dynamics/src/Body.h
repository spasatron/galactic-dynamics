#pragma once
struct Body {
	double xPos, yPos, mass, xVel, yVel;
	int id;

	Body(double x, double y, double m, double xV, double yV, int i) : xPos(x), yPos(y),
		mass(m), xVel(xV), yVel(yV), id(i) {}
};