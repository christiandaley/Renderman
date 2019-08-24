#ifndef RTSPHERE_H
#define RTSPHERE_H

#include "RtPrimitive.h"

class RtSphere :
	public RtPrimitive
{
	RtFloat zmin, zmax;
	RtFloat radius;
	RtFloat thetamax, phimin, phimax;
public:
	void uvToXyz(RtFloat u, RtFloat v, RtPoint p);
	RtSphere();
	RtSphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax);
	~RtSphere();
};

#endif