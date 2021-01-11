#include "GalacticDynamics.h"
#include <math.h>


/////////////////////////////////////////
////////////////Vec2 Struct/////////////
////////////////////////////////////////


std::ostream& operator<<(std::ostream& os, const Vec2& dt) {
    os << dt.x << ", " << dt.y;
    return os;
}





//////////////////////////////////////////
/////////////Quad Struct/////////////////
/////////////////////////////////////////


bool Quad::contains(double x, double y) {
    return contains({ x, y });
}

bool Quad::contains(Vec2 pos)
{
    if (lowerLeft.x <= pos.x && pos.x < lowerLeft.x + size && lowerLeft.y <= pos.y && pos.y < lowerLeft.y + size) {
        return true;
    }
    else {
        return false;
    }
}


Quad Quad::NW()
{
    return Quad({ lowerLeft.x + size / 2, lowerLeft.y }, size / 2);
}

Quad Quad::NE()
{
    return Quad({ lowerLeft.x + size / 2, lowerLeft.y + size / 2 }, size / 2);
}

Quad Quad::SW()
{
    return Quad({ lowerLeft.x, lowerLeft.y }, size / 2);
}

Quad Quad::SE()
{
    return Quad({ lowerLeft.x, lowerLeft.y + size / 2 }, size / 2);
}











///////////////////////////////////////////////////////
////////////////Body Struct///////////////////////////
//////////////////////////////////////////////////////

void Body::update(double dt)
{
    v.x += dt * f.x / m;
    v.y += dt * f.y / m;
    r.x += dt * v.x;
    r.y += dt * v.y;

}

double Body::distanceTo(Body b)
{
	double dx = r.x - b.r.x;
	double dy = r.y - b.r.y;

	return sqrt(dx * dx + dy * dy);

}

void Body::resetForce()
{
	f = { 0.0, 0.0 };
}

void Body::addForce(Body b)
{
    //const double G = 6.67e-11; 
    const double G = 1;// gravational constant
    const double EPS = 3e-6;      // softening parameter
    double dx = b.r.x - r.x;
    double dy = b.r.x - r.y;
    double dist = sqrt(dx * dx + dy * dy);
    double F = (G * m * b.m) / (dist * dist + EPS * EPS);
    f.x += F * dx / dist;
    f.y += F * dy / dist;
}

bool Body::in(Quad q)
{
    return q.contains(r);
}

Body Body::addBodies(Body a, Body b)
{
    double mass = a.m + b.m;

    double x = (a.m * a.r.x + b.m * b.r.x)/mass;
    double y = (a.m * a.r.y + b.m * b.r.y) / mass;

    return Body({ x, y }, { 0.0, 0.0 }, mass);
}

bool Body::operator==(Body b)
{
    if (r.x == b.r.x && r.y == b.r.y && v.x == b.v.x && v.y == b.v.y && m == b.m) {
        return true;
    }
    return false;
}
    

////////////////////////////////////////////////////////
/////////////////BHTree Class//////////////////////////
//////////////////////////////////////////////////////








BHTree::BHTree()       
{                      
    root = nullptr;    
}

BHTree::BHTree(Quad quad)
{
    root = new BHTreeNode();
    root->quad = quad;
}
                       
BHTree::~BHTree()      
{                      
    destroyTreeFromNode(root);
}

void BHTree::insertBody(Body b)
{
    insertBodyAtNode(b, root);
}

void BHTree::printSomething()
{
    std::cout << "End" << std::endl;
}

void BHTree::updateForce(Body *b)
{

    updateForceAtNode(b, root);
}
                       
void BHTree::destroyTreeFromNode(BHTreeNode *node)
{                      
    if (node == nullptr) {
        return;        
    }                  
    destroyTreeFromNode(node->NE);
    destroyTreeFromNode(node->NW);
    destroyTreeFromNode(node->SE);
    destroyTreeFromNode(node->SW);

    delete node;
}

void BHTree::updateForceAtNode(Body *b, BHTreeNode *node)
{
    const double theta = .5;

    if (node == nullptr) {
        return;
    }


    //if the node is external
    if (!node->internal && node->body.m != 0.0) {
        if (!(*b == node->body)) {
            b->addForce(node->body);
        }
    }

    //if node is external and far enough away
    if (node->quad.size / b->distanceTo(node->body) <= theta) {
        b->addForce(node->body);
    }
    //If none of the above, run recursively over the children.
    else{
        updateForceAtNode(b, node->NE);
        updateForceAtNode(b, node->NW);
        updateForceAtNode(b, node->SE);
        updateForceAtNode(b, node->SW);
    }













}

void BHTree::insertBodyAtNode(Body b, BHTreeNode *node)
{


    //Step one: no bodies are present in the node
    if (node->body.m == 0) {
        node->body = b;
        return;
    }

    //Step two: internal node calculations

    if (node->internal) {
        //Update center of mass
        node->body = Body::addBodies(node->body, b);

        //Recursively add body b to the appropraite quadrant

        if (node->NE->quad.contains(b.r)) {
            insertBodyAtNode(b, node->NE);
        }
        if (node->NW->quad.contains(b.r)) {
            insertBodyAtNode(b, node->NW);
        }
        if (node->SW->quad.contains(b.r)) {
            insertBodyAtNode(b, node->SW);
        }
        if (node->SE->quad.contains(b.r)) {
            insertBodyAtNode(b, node->SE);
        }

    }

    //Step three: external node calculations


    if (!node->internal) {
        //We need to create four more regions
        node->NE = new BHTreeNode;
        node->NE->quad = node->quad.NE();
        node->NW = new BHTreeNode;
        node->NW->quad = node->quad.NW();
        node->SE = new BHTreeNode;
        node->SE->quad = node->quad.SE();
        node->SW = new BHTreeNode;
        node->SW->quad = node->quad.SW();

        node->internal = true;


        //Recursively insert node body into its quadrants
        if (node->NE->quad.contains(node->body.r)) {
            insertBodyAtNode(node->body, node->NE);
        }
        if (node->NW->quad.contains(node->body.r)) {
            insertBodyAtNode(node->body, node->NW);
        }
        if (node->SW->quad.contains(node->body.r)) {
            insertBodyAtNode(node->body, node->SW);
        }
        if (node->SE->quad.contains(node->body.r)) {
            insertBodyAtNode(node->body, node->SE);
        }

        //Recursively insert current body into its quadrants;

        if (node->NE->quad.contains(b.r)) {
            insertBodyAtNode(b, node->NE);
        }
        if (node->NW->quad.contains(b.r)) {
            insertBodyAtNode(b, node->NW);
        }
        if (node->SW->quad.contains(b.r)) {
            insertBodyAtNode(b, node->SW);
        }
        if (node->SE->quad.contains(b.r)) {
            insertBodyAtNode(b, node->SE);
        }
        //Once those are done, combine both bodies
        node->body = Body::addBodies(node->body, b);

    }





}
