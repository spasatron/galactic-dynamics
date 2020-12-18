#include "QuadTree.h"







int main() {

	Body a(-1, -1, 1, 0, 1, 1), b(1, 1, 5, 0, -1, 2);

	QuadTree* q = new QuadTree(-5, -5, 10);

	q->addBodyToTree(a);
	q->addBodyToTree(b);

	std::cout << q->forceOnBody(a) << " " <<  q->forceOnBody(b) << std::endl;


}