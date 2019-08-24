#ifndef RTTORUS_H
#define RTTORUS_H

#include "RtPrimitive.h"
class RtTorus :
	public RtPrimitive
{
	RtFloat majorradius, minorradius;
	RtFloat thetamax, phimin, phimax;
public:
	RtTorus();
	RtTorus(RtFloat majorradius, RtFloat minorradius, RtFloat phimin, RtFloat phimax, RtFloat thetamax);
	void uvToXyz(RtFloat u, RtFloat v, RtPoint p);
	~RtTorus();
};

#endif