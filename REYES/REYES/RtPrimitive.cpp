#include "stdafx.h"
#include <iostream>
#include "RtPrimitive.h"
#include "Micropolygon.h"


RtPrimitive::RtPrimitive()
{

}

void RtPrimitive::shade() {
	shaderSettings = this->settings;

	init_shader();

	this->mesh->shade(this->surfaceShader);
	this->mesh->shade(this->displacementShader);
#if DO_LIGHT_SHADING
	this->mesh->shade(LIGHTING);
#endif
}

void RtPrimitive::dice(RtMatrix object_to_pixels, RtInt xres, RtInt yres) {
	// the maximum width and height of a polygon in our mesh
	RtFloat width_max = 0;
	RtFloat height_max = 0;
	this->mesh = new Mesh(DICING_SENSITIVITY, DICING_SENSITIVITY);
	this->deform();
	mesh->transform(object_to_pixels);

	for (int j = 0; j < mesh->height - 1; j++) {
		for (int i = 0; i < mesh->width - 1; i++) {
			MeshEntry_t &v1 = this->mesh->at(i, j);
			MeshEntry_t &v2 = this->mesh->at(i + 1, j);
			MeshEntry_t &v3 = this->mesh->at(i + 1, j + 1);
			MeshEntry_t &v4 = this->mesh->at(i, j + 1);

			Micropolygon mp = Micropolygon(v1, v2, v3, v4);

			if (!mp.onScreen(xres, yres))
				continue; // dont consider polygons that wont be on the screen

			if (mp.width() > width_max)
				width_max = mp.width();

			if (mp.height() > height_max)
				height_max = mp.height();
			
		}
	}

	delete this->mesh;

	if (width_max == 0 && height_max == 0) {
		// this object is completely off the screen and should not be rendered
		this->mesh = NULL;
		return;
	}

	this->mesh = new Mesh(30 + DICING_SENSITIVITY * width_max / MAX_POLYGON_LENGTH,
						30 + DICING_SENSITIVITY * height_max / MAX_POLYGON_LENGTH);

	this->deform();

	this->mesh->color(this->color);
	this->mesh->opacity(this->opacity);
}

void RtPrimitive::deform() {
	RtFloat u, v;
	for (int j = 0; j < mesh->height; j++) {
		for (int i = 0; i < mesh->width; i++) {
			MeshEntry_t &e = mesh->at(i, j);
			// u & v are normalized coordinates each in the range [0, 1]
			u = (RtFloat)i / (mesh->width - 1);
			v = (RtFloat)j / (mesh->height - 1);

			this->uvToXyz(u, v, e.p); 

		}
	}
}

RtPrimitive::~RtPrimitive()
{
	//std::cout << "Deleted a primitive" << std::endl;
	delete mesh;
}
