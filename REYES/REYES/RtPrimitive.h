#ifndef RT_PRIMITIVE_H
#define RT_PRIMITIVE_H

#include "Ri.h"
#include "Mesh.h"
#include "shaders.h"

#define DICING_SENSITIVITY 100
#define MAX_POLYGON_LENGTH 0.75f // the maximum allowed length of a polygon side (in pixels)

class RtPrimitive
{
public:
	RtMatrix transform; // object to world transformation
	Mesh *mesh;
	RtColor color, opacity;
	ShaderSettings_t settings;
	const void(*surfaceShader)(void) = NULL;
	const void(*displacementShader)(void) = NULL;

	void setTransform(RtMatrix t) { mcopy(this->transform, t); };
	void setColor(RtColor c) { memcpy(this->color, c, sizeof(RtColor)); };
	void setOpacity(RtColor o) { memcpy(this->opacity, o, sizeof(RtColor)); };
	void applyTransform(RtMatrix t) { this->mesh->transform(t); };

	/* Dices itself into a mesh. The level of dicing needed is 
	estimated using the object_to_pixels and the screen resolution transformation */
	void dice(RtMatrix object_to_pixels, RtInt xres, RtInt yres);

	virtual void uvToXyz(RtFloat u, RtFloat v, RtPoint p) = 0;

	void deform(); // deforms its mesh into the appropriate object
	void shade();

	RtPrimitive();
	~RtPrimitive();
};

#endif

