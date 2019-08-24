#ifndef RTCYLINDER_H
#define RTCYLINDER_H

#include "RtPrimitive.h"
class RtCylinder :
	public RtPrimitive
{
	RtFloat radius, zmin, zmax, thetamax;
public:
	RtCylinder();
	RtCylinder(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax);
	void uvToXyz(RtFloat u, RtFloat v, RtPoint p);
	~RtCylinder();
};

#endif