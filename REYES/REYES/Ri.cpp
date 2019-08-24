#include "stdafx.h"
#include <iostream>
#include <cstdarg>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <atlimage.h>
#include <Gdiplusimaging.h>
#include <Gdiplus.h>
#include <time.h>
#include "Ri.h"
#include "RtSphere.h"
#include "RtTorus.h"
#include "RtCone.h"
#include "RtCylinder.h"
#include "FrameBuffer.h"
#include "Micropolygon.h"
#include "shaders.h"

RtToken RI_PERSPECTIVE = "perspective";
RtToken RI_ORTHOGRAPHIC = "orthographic";

RtMatrix current_transform;

Screen_t Screen;
Camera_t Camera;
RtMatrix world_to_camera;
RtMatrix camera_to_screen;
RtMatrix screen_to_pixels;
Settings_t settings;

wchar_t filename[64];
char file_extension[16];
RtToken filetype, filemode;
RtInt currentframe;

RtColor currentColor;
RtColor currentOpacity= { 1.0f, 1.0f, 1.0f };

std::vector<RtPrimitive *> objects;
FrameBuffer *fb = NULL;

const void(*surfaceShader)(void) = NULL;
const void(*displacementShader)(void) = NULL;
// stores the result of AB in C
void matrix_mul(RtMatrix A, RtMatrix B, RtMatrix C);
void zero_matrix(RtMatrix A);
void identity_matrix(RtMatrix A);
void p2h(RtPoint p, RtHpoint h);
void h2p(RtHpoint h, RtPoint p);


RtVoid RiBegin(RtToken name) {
	surfaceShader = NULL;
	displacementShader = NULL;
	Screen = Screen_t();
	Camera = Camera_t();
	shaderInfo = ShaderInfo_t();
	shaderSettings = ShaderSettings_t();

	currentOpacity[0] = 1.0f;
	currentOpacity[1] = 1.0f;
	currentOpacity[2] = 1.0f;

	RiIdentity();
	mcopy(camera_to_screen, current_transform);
	mcopy(world_to_camera, current_transform);
	srand(time(NULL));
}

RtVoid RiEnd() {
	free_shader();
}

RtVoid RiFormat(RtInt xres, RtInt yres, RtFloat aspect) {
	Screen.xres = xres;
	Screen.yres = yres;
	Screen.pixelaspect = aspect;

	// create the screen-to-pixels transformation
	zero_matrix(screen_to_pixels);
	screen_to_pixels[0][0] = (RtFloat)xres / 2.0f;
	screen_to_pixels[1][1] = (RtFloat)yres / 2.0f;
	screen_to_pixels[2][2] = 1.0f;
	screen_to_pixels[3][3] = 1.0f;

	screen_to_pixels[0][3] = (RtFloat)xres / 2.0f;
	screen_to_pixels[1][3] = (RtFloat)yres / 2.0f;

}

RtVoid RiFrameAspectRatio(RtFloat aspect) {
	Screen.aspect = aspect;
}

RtVoid RiFrameBegin(RtInt frame) {
	mcopy(settings.current_transform, current_transform);
	mcopy(settings.camera_to_screen, camera_to_screen);
	mcopy(settings.world_to_camera, world_to_camera);

	settings.Camera = Camera;
	settings.Screen = Screen;
	currentframe = frame;
}

RtVoid RiFrameEnd() {
	// Here is where we render the image
#ifdef RI_DEBUG
	std::cout << "...................." << std::endl;
	std::cout << "Starting to render frame " << currentframe << std::endl;
	std::cout << "  Creating frame buffer..." << std::endl;
#endif
	// create the frame buffer
	fb = new FrameBuffer(Screen.xres, Screen.yres, (RtInt)Screen.xsamples, (RtInt)Screen.ysamples);
#ifdef RI_DEBUG
	std::cout << "  Created frame buffer with an initial size of " << fb->size() << " Mb" << std::endl;
	std::cout << "  Scene contains " << objects.size() << " object(s) to render" << std::endl;
#endif
	// loop through each object in the world
	RtPrimitive *obj;
	RtMatrix object_to_pixels;
	int count = 0;
#ifdef RI_RENDERING_PROGRESS
	int percent = 0;
#endif

	//RtMatrix transform;
	while (objects.size() > 0) {
		count++;
#ifdef RI_DEBUG
		std::cout << "    Rendering object " << count << ":" << std::endl;
#endif
		obj = objects.back();
		objects.pop_back();

		get_obj_to_pixels(object_to_pixels, obj->transform);
		//print_matrix(object_to_pixels);

		// dice the object according to the screen resolution
		obj->dice(object_to_pixels, Screen.xres, Screen.yres);

		if (obj->mesh == NULL) {

#ifdef RI_DEBUG
			std::cout << "      Object is off screen and will not be rendered" << std::endl;
#endif
			delete obj;
			continue;
		}

#ifdef RI_DEBUG
		std::cout << "      Diced object into a " << obj->mesh->width
			<< "x" << obj->mesh->height << " grid (" << obj->mesh->size() << " Mb)" << std::endl;
#endif

		obj->applyTransform(obj->transform); // object to world

		// objects are shaded in world-space
		obj->shade();

		obj->applyTransform(world_to_camera); // world to camera
		obj->applyTransform(camera_to_screen); // camera to screen
		obj->applyTransform(screen_to_pixels); // screen to pixels

#ifdef RI_DEBUG
		std::cout << "      Shaded object" << std::endl;
		std::cout << "      Rendering object in the frame buffer..." << std::endl;
#endif
		fb->render(obj);
		delete obj;
#ifdef RI_DEBUG
		std::cout << "    Object " << count << " rendered" << std::endl;
#endif
#ifdef RI_RENDERING_PROGRESS
		int p = 100 * count / (objects.size() + count);
		if (p > percent) {
			percent = p;
			std::cout << "Rendering " << percent << "% completed" << std::endl;
		}
#endif
	}

#ifdef RI_DEBUG
	std::cout << "  Finished rendering object(s). Frame buffer now has a size of " 
		<< fb->size() << " Mb" << std::endl;
	std::cout << "  Drawing final image..." << std::endl;
#endif

	fb->draw(filename, file_extension);
	delete fb;
	fb = NULL;

#ifdef RI_DEBUG
	std::cout << "Wrote final image to ";
	std::wcout << filename << std::endl;
	std::cout << "...................." << std::endl;
#endif
	// restore all settings

	mcopy(current_transform, settings.current_transform);
	mcopy(camera_to_screen, settings.camera_to_screen);
	mcopy(world_to_camera, settings.world_to_camera);

	Camera = settings.Camera;
	Screen = settings.Screen;


}

RtVoid RiWorldBegin() {
	objects = std::vector<RtPrimitive *>();
	mcopy(world_to_camera, current_transform);
	RiIdentity();

	// Determines the location of the camera in world space
	// Needed for light shading

	RtPoint origin = { 0.0f, 0.0f, 0.0f };
	RtPoint temp;
	point_transform(world_to_camera, origin, temp);
	Camera.location[0] = -temp[0];
	Camera.location[1] = -temp[1];
	Camera.location[2] = -temp[2];

}

RtVoid RiWorldEnd() {
	//objects.clear();
}

RtVoid RiPixelSamples(RtFloat xsamples, RtFloat ysamples) {
	Screen.xsamples = xsamples;
	Screen.ysamples = ysamples;
}


RtVoid RiIdentity() {
	RtMatrix id;
	identity_matrix(id);
	RiTransform(id);
}

RtVoid RiTransform(RtMatrix transform) {
	mcopy(current_transform, transform);
}

RtVoid RiConcatTransform(RtMatrix transform) {
	RtMatrix temp;
	matrix_mul(current_transform, transform, temp);
	RiTransform(temp);
}

RtVoid RiTransformBegin() {
	RiIdentity();
}

RtVoid RiTransformEnd() {
	RiIdentity();
}

RtVoid RiScale(RtFloat sx, RtFloat sy, RtFloat sz) {
	RtMatrix m;
	zero_matrix(m);
	m[0][0] = sx;
	m[1][1] = sy;
	m[2][2] = sz;
	m[3][3] = 1.0;
	RiConcatTransform(m);
}

RtVoid RiTranslate(RtFloat dx, RtFloat dy, RtFloat dz) {
	RtMatrix m;
	identity_matrix(m);
	m[0][3] = dx;
	m[1][3] = dy;
	m[2][3] = dz;
	RiConcatTransform(m);
}

RtVoid RiRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) {
	RtFloat rad = angle * MY_PI / 180.0f;
	RtMatrix R;
	zero_matrix(R);
	if (dz == 1.0) {
		R[0][0] = cos(rad);
		R[0][1] = -sin(rad);
		R[1][0] = sin(rad);
		R[1][1] = cos(rad);
		R[2][2] = 1.0;
		R[3][3] = 1.0;
	}
	else if (dy == 1.0) {
		R[0][0] = cos(rad);
		R[0][2] = sin(rad);
		R[2][0] = -sin(rad);
		R[2][2] = cos(rad);
		R[1][1] = 1.0;
		R[3][3] = 1.0;
	}
	else if (dx == 1.0) {
		R[1][1] = cos(rad);
		R[1][2] = -sin(rad);
		R[2][1] = sin(rad);
		R[2][2] = cos(rad);
		R[0][0] = 1.0;
		R[3][3] = 1.0;
	}

	RiConcatTransform(R);
}

RtVoid RiPerspective(RtFloat fov) {
	RtMatrix p;
	zero_matrix(p);
	float rad = fov * MY_PI / 180.0f;

	p[0][0] = 1.0f / tan(rad / 2.0f);
	p[1][1] = 1.0f / tan(rad / 2.0f / Screen.aspect);
	p[2][2] = Camera.yon / (Camera.yon - Camera.hither);
	p[2][3] = -Camera.yon * Camera.hither / (Camera.yon - Camera.hither);
	p[3][2] = 1.0f;
	RiConcatTransform(p);

}

RtVoid RiProjection(RtToken name, ...) {
	if (strcmp(name, RI_ORTHOGRAPHIC) == 0) {
		/*
		1. Build the orthographic matrix
		2. Concatenate it with the current transformation matrix
		3. Save the current transformation matrix as the camera-to-screen transformation
		4. Set the current transformation matrix to the identity matrix

		*/

		RtMatrix orth;
		zero_matrix(orth);
		orth[0][0] = 1.0f;
		orth[1][1] = 1.0f;
		orth[2][2] = Camera.yon / (Camera.yon - Camera.hither);
		orth[2][3] = -Camera.yon * Camera.hither / (Camera.yon - Camera.hither);
		orth[3][2] = 1.0f;

		RiConcatTransform(orth);
		mcopy(camera_to_screen, current_transform);
		RiIdentity();

	}
	else if (strcmp(name, RI_PERSPECTIVE) == 0) {
		va_list ap;
		char *param;
		va_start(ap, name);
		while ((param = va_arg(ap, char *)) != RI_NULL) {
			if (strcmp(param, "fov") == 0) {
				Camera.fov = *va_arg(ap, float *);
			}

		}
		
		/* 
		1. Build the perspective matrix
		2. Concatenate it with the current transformation matrix
		3. Save the current transformation matrix as the camera-to-screen transformation
		4. Set the current transformation matrix to the identity matrix

		*/

		RtMatrix p;
		zero_matrix(p);

		// angle must be in radians
		float rad = Camera.fov * MY_PI / 180.0f;

		p[0][0] = 1.0f / tan(rad / 2.0f);
		p[1][1] = 1.0f / tan(rad / 2.0f / Screen.aspect);
		p[2][2] = Camera.yon / (Camera.yon - Camera.hither);
		p[2][3] = -Camera.yon * Camera.hither / (Camera.yon - Camera.hither);
		p[3][2] = 1.0f;

		RiConcatTransform(p);
		mcopy(camera_to_screen, current_transform);
		RiIdentity();

	}
	else {
		
	}
}

RtVoid RiDisplay(char *name, RtToken type, RtToken mode, ...) {
	size_t name_size = strlen(name) + 1 < 63 ? strlen(name) + 1 : 63;
	mbstowcs_s(NULL, filename, name_size, name, _TRUNCATE);
	filename[name_size + 1] = 0;

	filetype = type;
	filemode = mode;

	while (*name != 0) {
		if (*name == '.') {
			strncpy_s(file_extension, sizeof(file_extension), name + 1, strlen(name + 1));
			break;
		}

		name++;
	}

	va_list ap;
	char *param;
	va_start(ap, name);
	while ((param = va_arg(ap, char *)) != RI_NULL) {
		
	}

}

RtVoid RiColor(RtColor color) {
	memcpy(currentColor, color, sizeof(RtColor));
}

RtVoid RiOpacity(RtColor opacity) {
	memcpy(currentOpacity, opacity, sizeof(RtColor));
}

RtVoid RiSphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, ...) {
	va_list ap;
	char *param;
	va_start(ap, thetamax);
	while ((param = va_arg(ap, char *)) != RI_NULL) {
		
	}

	RtSphere *s = new RtSphere(radius, zmin, zmax, thetamax);
	s->setTransform(current_transform);
	s->surfaceShader = surfaceShader;
	s->displacementShader = displacementShader;
	s->setColor(currentColor);
	s->setOpacity(currentOpacity);
	s->settings = shaderSettings;


	objects.push_back(s);
}

RtVoid RiTorus(RtFloat majorradius, RtFloat minorradius, RtFloat phimin, RtFloat phimax, RtFloat thetamax, ...) {
	va_list ap;
	char *param;
	va_start(ap, thetamax);
	while ((param = va_arg(ap, char *)) != RI_NULL) {
		
	}

	RtTorus *t = new RtTorus(majorradius, minorradius, phimin, phimax, thetamax);
	t->setTransform(current_transform);
	t->surfaceShader = surfaceShader;
	t->displacementShader = displacementShader;
	t->setColor(currentColor);
	t->setOpacity(currentOpacity);
	t->settings = shaderSettings;

	objects.push_back(t);
}

RtVoid RiCone(RtFloat height, RtFloat radius, RtFloat thetamax, ...) {
	va_list ap;
	char *param;
	va_start(ap, thetamax);
	while ((param = va_arg(ap, char *)) != RI_NULL) {
		
	}

	RtCone *c = new RtCone(height, radius, thetamax);
	c->setTransform(current_transform);
	c->surfaceShader = surfaceShader;
	c->displacementShader = displacementShader;
	c->setColor(currentColor);
	c->setOpacity(currentOpacity);
	c->settings = shaderSettings;

	objects.push_back(c);
}

RtVoid RiCylinder(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, ...) {
	va_list ap;
	char *param;
	va_start(ap, thetamax);
	while ((param = va_arg(ap, char *)) != RI_NULL) {
		
	}

	RtCylinder *c = new RtCylinder(radius, zmin, zmax, thetamax);
	c->setTransform(current_transform);
	c->surfaceShader = surfaceShader;
	c->displacementShader = displacementShader;
	c->setColor(currentColor);
	c->setOpacity(currentOpacity);
	c->settings = shaderSettings;

	objects.push_back(c);

}

RtVoid RiSurface(const void(*shader)(void)) {
	surfaceShader = shader;
}

RtVoid RiDisplacement(const void(*shader)(void)) {
	displacementShader = shader;
}

RtVoid RiMakeTexture(const char *name, unsigned int index) {
#ifdef RI_DEBUG
	std::cout << "Loading " << name << " into texture slot " << index << "..." << std::endl;
#endif
	wchar_t txname[64];
	size_t name_size = strlen(name) + 1 < 63 ? strlen(name) + 1 : 63;
	mbstowcs_s(NULL, txname, name_size, name, _TRUNCATE);
	txname[name_size + 1] = 0;

	CImage texture = CImage();
	texture.Load(txname);
	int width = texture.GetWidth();
	int height = texture.GetHeight();

	delete_texture(index);
	shaderInfo.textures[index].height = height;
	shaderInfo.textures[index].width = width;
	shaderInfo.textures[index].colors = new RtColor[width * height];

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			COLORREF c = texture.GetPixel(i, j);
			RtColor *color = &shaderInfo.textures[index].colors[j * width + i];
			(*color)[0] = (RtFloat)(c & 0xff) / 255.0f;
			(*color)[1] = (RtFloat)((c >> 8) & 0xff) / 255.0f;
			(*color)[2] = (RtFloat)((c >> 16) & 0xff) / 255.0f;
		}
	}
	
#ifdef RI_DEBUG
	std::cout << "Finished loading texture" << std::endl;
#endif
}

void get_obj_to_pixels(RtMatrix obj_to_pixels, RtMatrix obj_to_world) {
	RtMatrix t1, t2;
	RtMatrix i;
	identity_matrix(i);
	matrix_mul(screen_to_pixels, camera_to_screen, t1);
	matrix_mul(t1, world_to_camera, t2);
	matrix_mul(t2, obj_to_world, obj_to_pixels);
}

void matrix_mul(RtMatrix A, RtMatrix B, RtMatrix C) {
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			C[i][j] = (A[i][0] * B[0][j]) + (A[i][1] * B[1][j]) + (A[i][2] * B[2][j]) + (A[i][3] * B[3][j]);
		}
	}
}

RtFloat dot_product(RtVector a, RtVector b) {
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void cross_product(RtVector a, RtVector b, RtVector c) {
	c[0] = (a[1] * b[2]) - (a[2] * b[1]);
	c[1] = -((a[0] * b[2]) - (a[2] * b[0]));
	c[2] = (a[0] * b[1]) - (a[1] * b[0]);
}

void normalize(RtVector a) {
	RtFloat len = sqrtf(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] /= len;
	a[1] /= len;
	a[2] /= len;

}

void point_transform(RtMatrix A, RtPoint b, RtPoint c) {
	RtHpoint bh, ch;
	int i;
	p2h(b, bh);

	for (i = 0; i < 4; i++) {
		ch[i] = (bh[0] * A[i][0]) + (bh[1] * A[i][1]) + (bh[2] * A[i][2]) + (bh[3] * A[i][3]);
	}


	h2p(ch, c);
}

void zero_matrix(RtMatrix A) {
	for (int i = 0; i < 16; i++) {
		A[i / 4][i % 4] = 0.0;
	}
}

void identity_matrix(RtMatrix A) {
	zero_matrix(A);
	for (int i = 0; i < 4; i++) {
		A[i][i] = 1.0;
	}
}

void p2h(RtPoint p, RtHpoint h) {
	h[0] = p[0];
	h[1] = p[1];
	h[2] = p[2];
	h[3] = 1.0;
}

void h2p(RtHpoint h, RtPoint p) {
	p[0] = h[0] / h[3];
	p[1] = h[1] / h[3];
	p[2] = h[2] / h[3];
}

void print_matrix(RtMatrix m) {
	int i;
	for (i = 0; i < 4; i++) {
		std::cout << m[i][0] << " " << m[i][1] << " " 
			<< m[i][2] << " " << m[i][3] << std::endl;
	}
}

unsigned int make_color(RtColor c) {
	unsigned char r = (unsigned char)(c[0] * 255);
	unsigned char g = (unsigned char)(c[1] * 255);
	unsigned char b = (unsigned char)(c[2] * 255);

	unsigned int color = b;
	color = (color << 8) | g;
	return (color << 8) | r;
}

void mcopy(RtMatrix dst, RtMatrix src) {
	memcpy(dst, src, sizeof(RtMatrix));
}

void print_curent_transform() {
	print_matrix(current_transform);
}

void print_screen_trans() {
	print_matrix(camera_to_screen);
}

void print_point(RtPoint p) {
	std::cout << "(" << p[0] << ", " << p[1] << ", " << p[2] << ")" << std::endl;
}