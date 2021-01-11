#define _USE_MATH_DEFINES
#include <random>
#include <math.h>
#include <iostream>
#include "GalacticDynamics.h"

#define NUMBODIES 10000


//Use the following for a random seed

std::default_random_engine generator;

std::uniform_real_distribution<double> distribution(0.0, 1.0);
Body createRandomUniformDiskBody(Vec2 center, double radius, double velocity);





int main() {
	generator.seed(77);

	
	Body bodies[NUMBODIES];

	for (int i = 0; i < NUMBODIES; i++) {
		bodies[i] = createRandomUniformDiskBody({ 0.0, 0.0 }, 2, 3);
	}

	//For debugging the positions
	/*for (int i = 0; i < NUMBODIES; i++) {
		std::cout << bodies[i].r << " " << bodies[i].v << std::endl;
	}*/

	BHTree q = BHTree(Quad({ -3, -3 }, 6));


	for (int i = 0; i < NUMBODIES; i++) {
		q.insertBody(bodies[i]);
	}

	for (int i = 0; i < NUMBODIES; i++) {
		q.updateForce(&(bodies[i]));
	}


}

Body createRandomUniformDiskBody(Vec2 center, double radius, double velocity)
{
	double angle = 2 * M_PI * distribution(generator);
	double r = radius * distribution(generator);

	double x = r * cos(angle);
	double y = r * sin(angle);

	double xV = -velocity * y / r;
	double yV = velocity * x / r;

	return Body(x, y, xV, yV, 1);

}
