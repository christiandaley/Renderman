#ifndef RTCONE_H
#define RTCONE_H

#include "RtPrimitive.h"
class RtCone :
	public RtPrimitive
{
	RtFloat height, radius, thetamax;
public:
	RtCone();
	RtCone(RtFloat height, RtFloat radius, RtFloat thetamax);
	void uvToXyz(RtFloat u, RtFloat v, RtPoint p);
	~RtCone();
};

#endif
