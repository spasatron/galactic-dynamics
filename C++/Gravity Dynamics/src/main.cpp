#define _USE_MATH_DEFINES
#include <random>
#include <math.h>
#include <iostream>
#include "GalacticDynamics.h"
#include <fstream>
#include <sstream>

#define NUMBODIES 10000


//Use the following for a random seed

std::default_random_engine generator;

std::uniform_real_distribution<double> distribution(0.0, 1.0);
Body createRandomUniformDiskBody(Vec2 center, double radius, double velocity);





int main() {
	generator.seed(77);

	
	Body bodies[NUMBODIES];

	for (int i = 0; i < NUMBODIES; i++) {
		bodies[i] = createRandomUniformDiskBody({ 0.0, 0.0 }, 2, .5);
	}

	/*bodies[0].r = { -1, 1 };
	bodies[1].r = { -.5, .5 };
	bodies[0].m = 1;
	bodies[1].m = 1;
	*/

	//For debugging the positions
	/*for (int i = 0; i < NUMBODIES; i++) {
		std::cout << bodies[i].r << " " << bodies[i].v << std::endl;
	}*/

	BHTree q;
	std::ofstream fileStream;
	std::stringstream fileName;

	for (int t = 0; t < 5000; t++) {
		fileName.str(std::string());
		fileName.clear();
		fileName << "data" << t << ".plot";

		fileStream.open(fileName.str());
		if (!fileStream.fail()) {
			for (int i = 0; i < NUMBODIES; i++) {
				fileStream << bodies[i].r << std::endl;
			}
			fileStream.close();
		}

		q.resetToQuad(Quad({ -10.0, -10.0 }, 20));
		

		for (int i = 0; i < NUMBODIES; i++) {
			q.insertBody(bodies[i]);
			bodies[i].resetForce();
		}

		

		for (int i = 0; i < NUMBODIES; i++) {
			q.updateForce(&(bodies[i]));
		}
		for (int i = 0; i < NUMBODIES; i++) {
			bodies[i].update(.01);
		}

		
		
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

	//double xV = 0;
	//double yV = 0;
	return Body(x, y, xV, yV, 1);

}
