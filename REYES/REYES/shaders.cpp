#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <assert.h>
#include "shaders.h"

#define SMOOTH_STEP(x) ((3 * (x) * (x)) - (2 * (x) * (x) * (x)))

ShaderInfo_t shaderInfo;
ShaderSettings_t shaderSettings;

void checkerboard_shader();
void phong_light_shader();
void texture_shader();
void bumpy_shader();
RtFloat interpolate(RtFloat a0, RtFloat a1, RtFloat w);
RtFloat dot_grid_gradient(int ix, int iy, RtFloat x, RtFloat y, int level);

const void(*CHECKERBOARD)(void) = (const void (*)(void))&checkerboard_shader;
const void(*LIGHTING)(void) = (const void(*)(void))&phong_light_shader;
const void(*TEXTURE_SHADER)(void) = (const void(*)(void))&texture_shader;
const void(*BUMPY_SHADER)(void) = (const void(*)(void))&bumpy_shader;

void delete_gradients();

void checkerboard_shader() {
	RtInt x = (int)(shaderInfo.u * CHECK_SIZE_X);
	RtInt y = (int)(shaderInfo.v * CHECK_SIZE_Y);
	if ((x + y) & 1) {
		SHADER_COLOR[0] = 0.0f;
		SHADER_COLOR[1] = 0.0f;
		SHADER_COLOR[2] = 0.0f;
	}
	else {
		SHADER_COLOR[0] = 1.0f;
		SHADER_COLOR[1] = 1.0f;
		SHADER_COLOR[2] = 1.0f;
	}
}

void phong_light_shader() {
	RtVector L, R, V;
	RtColor color;
	RtFloat dotLN, dotRV, spec;

	L[0] = LIGHT_SOURCE[0] - SHADER_POINT[0];
	L[1] = LIGHT_SOURCE[1] - SHADER_POINT[1];
	L[2] = LIGHT_SOURCE[2] - SHADER_POINT[2];
	normalize(L);

	V[0] = Camera.location[0] - SHADER_POINT[0];
	V[1] = Camera.location[1] - SHADER_POINT[1];
	V[2] = Camera.location[2] - SHADER_POINT[2];
	normalize(V);

	dotLN = dot_product(L, SHADER_NORMAL);
	R[0] = (2.0f * dotLN * SHADER_NORMAL[0]) - L[0];
	R[1] = (2.0f * dotLN * SHADER_NORMAL[1]) - L[1];
	R[2] = (2.0f * dotLN * SHADER_NORMAL[2]) - L[2];
	normalize(R);


	// ambient lighting calculation
	color[0] = KA * SHADER_COLOR[0] * AMBIENT[0];
	color[1] = KA * SHADER_COLOR[1] * AMBIENT[1];
	color[2] = KA * SHADER_COLOR[2] * AMBIENT[2];

	// diffuse lighting calculation. No need to do calculations when KD is 0
	if (KD == 0.0f)
		goto end;

	if (dotLN > 0.0f) {
		color[0] += KD * SHADER_COLOR[0] * DIFFUSE[0] * dotLN;
		color[1] += KD * SHADER_COLOR[1] * DIFFUSE[1] * dotLN;
		color[2] += KD * SHADER_COLOR[2] * DIFFUSE[2] * dotLN;

		//specular lighting calculation
		if (KS == 0.0f)
			goto end;

		dotRV = dot_product(R, V);
		if (dotRV > 0.0f) {
			spec = powf(dotRV, ALPHA);
			color[0] += KS * REFLECTIVITY[0] * SPECULAR[0] * spec;
			color[1] += KS * REFLECTIVITY[1] * SPECULAR[1] * spec;
			color[2] += KS * REFLECTIVITY[2] * SPECULAR[2] * spec;

		}
		
	}
end:
	// clamp intensities to [0, 1]
	SHADER_COLOR[0] = color[0] > 1.0f ? 1.0f : color[0];
	SHADER_COLOR[1] = color[1] > 1.0f ? 1.0f : color[1];
	SHADER_COLOR[2] = color[2] > 1.0f ? 1.0f : color[2];
}

void texture_shader() {
	Texture_t *texture = &shaderInfo.textures[TX];
	assert(texture != NULL);
	int x, y;
	x = (int)(shaderInfo.u * (texture->width - 1));
	y = (int)(shaderInfo.v * (texture->height - 1));
	memcpy(SHADER_COLOR, texture->colors[y * texture->width + x], sizeof(RtColor));
}

void bumpy_shader() {
	RtFloat x, y;
	int x0, x1, y0, y1;
	RtFloat n0, n1, ix0, ix1, value;
	RtFloat sx, sy, u, v;
	int width, height;

	for (int level = 0; level < BUMPINESS; level++) {
		width = GRID_WIDTH * (int)powf(SCALING_LEVEL, level);
		height = GRID_HEIGHT * (int)powf(SCALING_LEVEL, level);
		u = shaderInfo.u;
		v = shaderInfo.v;

		x = u * (width - 2);
		y = v * (height - 2);

		x0 = (int)x;
		x1 = x0 + 1;
		y0 = (int)y;
		y1 = y0 + 1;

		sx = x - (RtFloat)x0;
		sy = y - (RtFloat)y0;

		n0 = dot_grid_gradient(x0, y0, x, y, level);
		n1 = dot_grid_gradient(x1, y0, x, y, level);
		ix0 = interpolate(n0, n1, sx);
		n0 = dot_grid_gradient(x0, y1, x, y, level);
		n1 = dot_grid_gradient(x1, y1, x, y, level);
		ix1 = interpolate(n0, n1, sx);
		value = interpolate(ix0, ix1, sy);

		SHADER_POINT[0] += value * SHADER_NORMAL[0] * MAX_DISPLACEMENT / (1 << level);
		SHADER_POINT[1] += value * SHADER_NORMAL[1] * MAX_DISPLACEMENT / (1 << level);
		SHADER_POINT[2] += value * SHADER_NORMAL[2] * MAX_DISPLACEMENT / (1 << level);
	}
}

RtFloat interpolate(RtFloat a0, RtFloat a1, RtFloat w) {
	return a0 + w * (a1 - a0);
}

RtFloat dot_grid_gradient(int ix, int iy, RtFloat x, RtFloat y, int level) {

	RtVector *grad = &shaderInfo.gradients[level][iy * GRID_WIDTH + ix];
	RtFloat dx = x - (RtFloat)ix;
	RtFloat dy = y - (RtFloat)iy;

	return dx * (*grad)[0] + dy * (*grad)[1];

}

void init_shader() {
	

	delete_gradients();

	shaderInfo.gradients = new RtVector *[BUMPINESS+1];
	int numGradients;
	for (int i = 0; i < BUMPINESS; i++) {
		numGradients = GRID_WIDTH * GRID_HEIGHT * (int)powf(SCALING_LEVEL, 2*i);
		shaderInfo.gradients[i] = new RtVector[numGradients];
		RtVector rv;
		for (int j = 0; j < numGradients; j++) {
			rv[0] = (RtFloat)rand() / RAND_MAX;
			rv[1] = (RtFloat)rand() / RAND_MAX;
			rv[2] = 0.0f;
			normalize(rv);
			memcpy(shaderInfo.gradients[i][j], rv, sizeof(RtVector));
		}
	}

	shaderInfo.gradients[BUMPINESS] = NULL;
}

void delete_texture(int tx) {
	assert(tx >= 0 && tx < NUM_TEXTURES);
	if (shaderInfo.textures[tx].colors != NULL) {
		delete[] shaderInfo.textures[tx].colors;
		shaderInfo.textures[tx].colors = NULL;
		shaderInfo.textures[tx].width = 0;
		shaderInfo.textures[tx].height = 0;
	}
}

void delete_gradients() {
	if (shaderInfo.gradients != NULL) {
		int i = 0;
		while (shaderInfo.gradients[i] != NULL)
			delete[] shaderInfo.gradients[i++];
		
	}

	delete[] shaderInfo.gradients;
	shaderInfo.gradients = NULL;
}

void free_shader() {
	for (int i = 0; i < NUM_TEXTURES; i++)
		delete_texture(i);

	delete_gradients();
}