#ifndef MICROPOLYGON_H
#define MICROPOLYGON_H

#include "Ri.h"
#include "Mesh.h"


class Micropolygon
{
private:
	RtFloat xmin, xmax, ymin, ymax, zmin, zmax; // bounding box
public:
	RtPoint vertices[4];
	RtColor colors[4];
	RtColor opacities[4];
	Micropolygon(MeshEntry_t &v1, MeshEntry_t &v2, MeshEntry_t &v3, MeshEntry_t &v4);

	// computes the bounds of the micropolygon in whole pixels
	void pixelBounds(RtInt xres, RtInt yres, RtInt &left, RtInt &right, RtInt &top, RtInt &bottom);

	// returns true if the given point is within the micropolygon.
	// also returns the interpolated z-coordinate, color, and opacity of that point
	bool contains(RtFloat x, RtFloat y, RtFloat &z, RtColor c, RtColor o);

	// returns true if any part of the micropolygon is within the scree bounds
	bool onScreen(RtInt xres, RtInt yres);
	RtFloat zCoord(RtFloat x, RtFloat y);
	RtFloat width();
	RtFloat height();
	void getColorAndOpacity(RtColor c, RtColor o, RtPoint p);
	
	void print();
	~Micropolygon();
};

#endif