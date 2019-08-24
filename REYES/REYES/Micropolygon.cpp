#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <assert.h>
#include "Micropolygon.h"

RtFloat xyDist(RtPoint p1, RtPoint p2) {
	RtFloat dx = p1[0] - p2[0];
	RtFloat dy = p1[1] - p2[1];
	return sqrtf(dx * dx + dy * dy);
}

RtFloat sign(RtPoint p1, RtPoint p2, RtPoint p3)
{
	return (p1[0] - p3[0]) * (p2[1] - p3[1]) - (p2[0] - p3[0]) * (p1[1] - p3[1]);
}

bool pointInTriangle(RtPoint pt, RtPoint v1, RtPoint v2, RtPoint v3)
{
	bool b1, b2, b3;

	b1 = sign(pt, v1, v2) <= 0.0f;
	b2 = sign(pt, v2, v3) <= 0.0f;
	b3 = sign(pt, v3, v1) <= 0.0f;

	return ((b1 == b2) && (b2 == b3));
}

RtFloat areaTriangle(RtPoint a, RtPoint b, RtPoint c) {
	return (a[0] * (b[1] - c[1]) + b[0] * (c[1] - a[1]) + c[0] * (a[1] - b[1])) / 2.0f;
}

void planeEquation(RtPoint p, RtPoint q, RtPoint r, RtFloat &a, RtFloat &b, RtFloat &c) {
	a = (q[1] - p[1]) * (r[2] - p[2]) - (q[2] - p[2]) * (r[1] - p[1]);
	b = (q[0] - p[0]) * (r[2] - p[2]) - (q[2] - p[2]) * (r[0] - p[0]);
	c = (q[0] - p[0]) * (r[1] - p[1]) - (q[1] - p[1]) * (r[0] - p[0]);
}

Micropolygon::Micropolygon(MeshEntry_t &v1, MeshEntry_t &v2, MeshEntry_t &v3, MeshEntry_t &v4) {

	memcpy(vertices[0], v1.p, sizeof(RtPoint));
	memcpy(vertices[1], v2.p, sizeof(RtPoint));
	memcpy(vertices[2], v3.p, sizeof(RtPoint));
	memcpy(vertices[3], v4.p, sizeof(RtPoint));

	memcpy(colors[0], v1.c, sizeof(RtColor));
	memcpy(colors[1], v2.c, sizeof(RtColor));
	memcpy(colors[2], v3.c, sizeof(RtColor));
	memcpy(colors[3], v4.c, sizeof(RtColor));

	memcpy(opacities[0], v1.o, sizeof(RtColor));
	memcpy(opacities[1], v2.o, sizeof(RtColor));
	memcpy(opacities[2], v3.o, sizeof(RtColor));
	memcpy(opacities[3], v4.o, sizeof(RtColor));

	xmin = 1e10;
	xmax = -1e10;
	ymin = 1e10;
	ymax = -1e10;
	zmin = 1e10;
	zmax = -1e10;

	for (int i = 0; i < 4; i++) {
		RtPoint &p = this->vertices[i];
		if (p[0] < xmin)
			xmin = p[0];
		if (p[0] > xmax)
			xmax = p[0];
		if (p[1] < ymin)
			ymin = p[1];
		if (p[1] > ymax)
			ymax = p[1];
		if (p[2] < zmin)
			zmin = p[2];
		if (p[2] > zmax)
			zmax = p[2];
	}
}

void Micropolygon::pixelBounds(RtInt xres, RtInt yres, RtInt &left, RtInt &right, RtInt &top, RtInt &bottom) {
	left = xmin < 0.0f ? 0 : (int)xmin;
	bottom = ymin < 0.0f ? 0 : (int)ymin;

	right = xmax + 1.0f >= xres ? xres - 1 : (int)xmax + 1.0f;
	top = ymax + 1.0f >= yres ? yres - 1 : (int)ymax + 1.0f;
}

bool Micropolygon::contains(RtFloat x, RtFloat y, RtFloat &z, RtColor c, RtColor o) {

	RtFloat a1, a2, a3, total;
	RtPoint p;
	p[0] = x;
	p[1] = y;
	p[2] = 0.0f;


	if (pointInTriangle(p, vertices[0], vertices[1], vertices[2])) {
		a1 = areaTriangle(vertices[1], vertices[2], p);
		a2 = areaTriangle(vertices[2], vertices[0], p);
		a3 = areaTriangle(vertices[0], vertices[1], p);
		total = a1 + a2 + a3;
		z = total / (a1 * (1.0f / vertices[0][2]) 
			+ a2 * (1.0f / vertices[1][2]) 
			+ a3 * (1.0f / vertices[2][2]));

		for (int i = 0; i < 3; i++) {
			c[i] = (a1 * colors[0][i] + a2 * colors[1][i] + a3 * colors[2][i]) / total;
			o[i] = (a1 * opacities[0][i] + a2 * opacities[1][i] + a3 * opacities[2][i]) / total;
		}

		return true;
	}
	else if (pointInTriangle(p, vertices[2], vertices[3], vertices[0])) {
		a1 = areaTriangle(vertices[3], vertices[0], p);
		a2 = areaTriangle(vertices[0], vertices[2], p);
		a3 = areaTriangle(vertices[2], vertices[3], p);
		total = a1 + a2 + a3;

		z = total / (a1 * (1.0f / vertices[2][2])
			+ a2 * (1.0f / vertices[3][2])
			+ a3 * (1.0f / vertices[0][2]));

		for (int i = 0; i < 3; i++) {
			c[i] = (a1 * colors[2][i] + a2 * colors[3][i] + a3 * colors[0][i]) / total;
			o[i] = (a1 * opacities[2][i] + a2 * opacities[3][i] + a3 * opacities[0][i]) / total;
		}

		return true;
	}

	return false;
}

bool Micropolygon::onScreen(RtInt xres, RtInt yres) {
	if (zmin < 0.0f || zmax > 1.0f)
		return false;

	if (xmax < 0.0f)
		return false;
	if (xmin > (RtFloat)xres)
		return false;

	if (ymax < 0.0f)
		return false;
	if (ymin > (RtFloat)yres)
		return false;

	return true;
}

RtFloat Micropolygon::zCoord(RtFloat x, RtFloat y) {
	RtPoint p;
	p[0] = x;
	p[1] = y;
	p[2] = 0.0f;

	RtFloat a, b, c;

	if (pointInTriangle(p, vertices[0], vertices[1], vertices[2])) {
		planeEquation(vertices[0], vertices[1], vertices[2], a, b, c);
		return (a*(x - vertices[0][0]) + b*(y - vertices[0][1]) - c*vertices[0][2]) / -c;
	}
	else if (pointInTriangle(p, vertices[2], vertices[3], vertices[0])) {
		planeEquation(vertices[2], vertices[3], vertices[0], a, b, c);
		return (a*(x - vertices[2][0]) + b*(y - vertices[2][1]) - c*vertices[2][2]) / -c;
	}

	// should never reach here...
	assert(false);

	return 1.0f;
}

RtFloat Micropolygon::width() {
	return xmax - xmin;
}
RtFloat Micropolygon::height() {
	return ymax - ymin;
}

void Micropolygon::getColorAndOpacity(RtColor c, RtColor o, RtPoint p) {

	RtFloat a1, a2, a3, total;
	if (pointInTriangle(p, vertices[0], vertices[1], vertices[2])) {
		a1 = areaTriangle(vertices[1], vertices[2], p);
		a2 = areaTriangle(vertices[2], vertices[0], p);
		a3 = areaTriangle(vertices[0], vertices[1], p);
		total = a1 + a2 + a3;
		for (int i = 0; i < 3; i++) {
			c[i] = (a1 * colors[0][i] + a2 * colors[1][i] + a3 * colors[2][i]) / total;
			o[i] = (a1 * opacities[0][i] + a2 * opacities[1][i] + a3 * opacities[2][i]) / total;
		}

		return;
	}
	else if (pointInTriangle(p, vertices[2], vertices[3], vertices[0])) {
		a1 = areaTriangle(vertices[3], vertices[0], p);
		a2 = areaTriangle(vertices[0], vertices[2], p);
		a3 = areaTriangle(vertices[2], vertices[3], p);
		total = a1 + a2 + a3;
		for (int i = 0; i < 3; i++) {
			c[i] = (a1 * colors[2][i] + a2 * colors[3][i] + a3 * colors[0][i]) / total;
			o[i] = (a1 * opacities[2][i] + a2 * opacities[3][i] + a3 * opacities[0][i]) / total;
		}

		return;
	}

	assert(false);
	// Should never reach here...
}


void Micropolygon::print() {
	std::cout << "[" << std::endl;
	print_point(vertices[0]);
	print_point(vertices[1]);
	print_point(vertices[2]);
	print_point(vertices[3]);

	std::cout << "xmin: " << xmin << ", xmax: " << xmax << ", ymin: " << ymin << ", ymax: " << ymax << std::endl;

	std::cout << "]" << std::endl;
}


Micropolygon::~Micropolygon()
{
}
