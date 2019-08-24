#include "stdafx.h"
#include "RtSphere.h"
#include <iostream>
#include <cmath>


RtSphere::RtSphere() {
	this->radius = 1.0f;
	this->zmin = -1.0f;
	this->zmax = 1.0f;
	this->thetamax = 360.0f;
	this->phimin = -90.0f;
	this->phimax = 90.0f;
}

RtSphere::RtSphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax) {
	this->radius = radius;
	this->zmin = zmin;
	this->zmax = zmax;
	this->thetamax = thetamax;

	if (zmin <= -radius)
		phimin = -90.0f;
	else
		phimin = asin(zmin / radius) * 180.0f / MY_PI;

	if (zmax >= radius)
		phimax = 90.0f;
	else
		phimax = asin(zmax / radius) * 180.0f / MY_PI;

}

void RtSphere::uvToXyz(RtFloat u, RtFloat v, RtPoint p) {
	RtFloat phi, theta;

	phi = this->phimin + v * (this->phimax - this->phimin);
	theta = u * this->thetamax;

	p[0] = this->radius * cos(theta * MY_PI / 180.0f) * cos(phi * MY_PI / 180.0f);
	p[1] = this->radius * sin(theta * MY_PI / 180.0f) * cos(phi * MY_PI / 180.0f);
	p[2] = this->radius * sin(phi * MY_PI / 180.0f);

}

RtSphere::~RtSphere()
{

	std::cout << "Deleted a Sphere" << std::endl;
}
