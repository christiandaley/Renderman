#ifndef MESH_H
#define MESH_H

#include "Ri.h"

struct MeshEntry_t {
	RtPoint p;
	RtColor c, o;
};

class Mesh
{
public:
	int width, height;
	MeshEntry_t *points;
	MeshEntry_t &at(int x, int y);
	void normalAt(int x, int y, RtNormal N);
	void color(RtColor c);
	void opacity(RtColor o);
	void shade(const void(*shader)(void));
	void transform(RtMatrix transform);
	void print();
	unsigned int size(); // returns the size in Mb
	Mesh(int width, int height);
	~Mesh();
};

#endif