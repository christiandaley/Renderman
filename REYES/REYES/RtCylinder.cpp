#include "stdafx.h"
#include <cmath>
#include "RtCylinder.h"


RtCylinder::RtCylinder()
{
}


RtCylinder::RtCylinder(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax) {
	this->radius = radius;
	this->zmin = zmin;
	this->zmax = zmax;
	this->thetamax = thetamax;
}

void RtCylinder::uvToXyz(RtFloat u, RtFloat v, RtPoint p) {
	RtFloat theta;

	theta = u * thetamax;
	p[0] = radius * cos(theta * MY_PI / 180.0f);
	p[1] = radius * sin(theta * MY_PI / 180.0f);
	p[2] = zmin + v * (zmax - zmin);
}

RtCylinder::~RtCylinder()
{
}
