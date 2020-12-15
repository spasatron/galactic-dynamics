#pragma once

struct Quadrant{
	double lowerLeftPoint;
	double length;
	double centerOfMass;
	double mass;
	Quadrant *NW, *NE, *SW, *SE;






	bool contains(double x, double y) {
		if((lowerLeftPoint <= x)  && (x <= lowerLeftPoint + length) && (lowerLeftPoint <= y) && (y <= lowerLeftPoint + length))
			return true;
		return false;
	}

};

struct Body
{

};



class BHTree {

	BHTree();
	~BHTree();

};




