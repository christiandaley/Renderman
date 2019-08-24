#include "stdafx.h"
#include <cmath>
#include <iostream>
#include "RtCone.h"


RtCone::RtCone()
{
}

RtCone::RtCone(RtFloat height, RtFloat radius, RtFloat thetamax) {
	this->height = height;
	this->radius = radius;
	this->thetamax = thetamax;
}

void RtCone::uvToXyz(RtFloat u, RtFloat v, RtPoint p) {

	RtFloat theta;

	theta = u * thetamax;
	p[0] = radius * (1.0f - v) * cos(theta * MY_PI / 180.0f);
	p[1] = radius * (1.0f - v) * sin(theta * MY_PI / 180.0f);
	p[2] = v * height;
}


RtCone::~RtCone()
{
}
