#include "HazardPath.h"

using namespace std;

void HazardPath::decideRegion(string str)
{
	int posPts;
	string pts;
	posPts = str.find("pts");
	pts = str.substr(posPts, str.size()).c_str();
}
