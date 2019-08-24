#include "stdafx.h"
#include <cmath>
#include "RtTorus.h"


RtTorus::RtTorus()
{
}

RtTorus::RtTorus(RtFloat majorradius, RtFloat minorradius, RtFloat phimin, RtFloat phimax, RtFloat thetamax) {
	this->majorradius = majorradius;
	this->minorradius = minorradius;
	this->phimin = phimin;
	this->phimax = phimax;
	this->thetamax = thetamax;


}

void RtTorus::uvToXyz(RtFloat u, RtFloat v, RtPoint p) {
	RtFloat phi, theta, r;
	theta = u * thetamax;
	phi = phimin + v * (phimax - phimin);
	r = minorradius * cos(phi * MY_PI / 180.0f);

	p[0] = (majorradius + r) * cos(theta * MY_PI / 180.0f);
	p[1] = (majorradius + r) * sin(theta * MY_PI / 180.0f);
	p[2] = minorradius * sin(phi * MY_PI / 180.0f);

}

RtTorus::~RtTorus()
{
}
