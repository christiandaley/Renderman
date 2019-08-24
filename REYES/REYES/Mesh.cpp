#include "stdafx.h"
#include <iostream>
#include "Mesh.h"
#include "shaders.h"


Mesh::Mesh(int width, int height)
{
	this->width = width;
	this->height = height;
	this->points = new MeshEntry_t[width * height]();
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			MeshEntry_t &e = this->at(i, j);
		}
	}
}

MeshEntry_t &Mesh::at(int x, int y) {
	return points[y * width + x];
}

void Mesh::normalAt(int x, int y, RtNormal N) {
	if (x == width - 1) {
		normalAt(width - 2, y, N);
		return;
	}
	else if (x == 0) {
		normalAt(1, y, N);
		return;
	}
	else if (y == height - 1) {
		normalAt(x, height - 2, N);
		return;
	}
	else if (y == 0) {
		normalAt(x, 1, N);
		return;
	}

	RtVector du, dv;
	MeshEntry_t &curr = this->at(x, y);
	MeshEntry_t &right = this->at(x + 1, y);
	MeshEntry_t &down = this->at(x, y + 1);

	du[0] = right.p[0] - curr.p[0];
	du[1] = right.p[1] - curr.p[1];
	du[2] = right.p[2] - curr.p[2];

	dv[0] = down.p[0] - curr.p[0];
	dv[1] = down.p[1] - curr.p[1];
	dv[2] = down.p[2] - curr.p[2];

	cross_product(du, dv, N);
	normalize(N);

	/*normalize(du);
	normalize(dv);
	print_point(du);
	print_point(dv);
	print_point(curr.p);*/
}

void Mesh::color(RtColor c) {
	for (int i = 0; i < width * height; i++) {
		memcpy(points[i].c, c, sizeof(RtColor));
	}
}

void Mesh::opacity(RtColor o) {
	for (int i = 0; i < width * height; i++) {
		memcpy(points[i].o, o, sizeof(RtColor));
	}
}

void Mesh::shade(const void(*shader)(void)) {
	if (shader == NULL)
		return;

	RtNormal N;

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			MeshEntry_t &e = this->at(i, j);
			this->normalAt(i, j, N);
			/*if (i == 0 && j == 0) {
				this->normalAt(i, j, N);
				print_point(N);

				std::cout << ".." << std::endl;
			}*/
			shaderInfo.p = &e.p;
			shaderInfo.c = &e.c;
			shaderInfo.o = &e.o;
			shaderInfo.N = &N;
			shaderInfo.u = (RtFloat)i / (width - 1);
			shaderInfo.v = (RtFloat)j / (height - 1);
			shader();
		}
	}

}

void Mesh::transform(RtMatrix transform) {
	RtPoint p;

	for (int i = 0; i < width * height; i++) {
		point_transform(transform, points[i].p, p);
		memcpy(points[i].p, p, sizeof(RtPoint));
	}

}

void Mesh::print() {
	for (int i = 0; i < width * height; i++) {
		print_point(points[i].p);
		print_point(points[i].c);
	}
}

unsigned int Mesh::size() {
	return width * height * sizeof(MeshEntry_t) / 1e6;
}

Mesh::~Mesh()
{
	delete[] points;
}
