#ifndef SHADERS_H
#define SHADERS_H

#include "Ri.h"
#include "Mesh.h"

#define NUM_TEXTURES 10

struct Texture_t {
	int width, height;
	RtColor *colors = NULL;
};

struct ShaderSettings_t {
	unsigned int check_size_x = 20;
	unsigned int check_size_y = 40;
	unsigned int tx; // the current texture

	// light shading settings
	RtPoint light_source = { -1000.0f, 1000.0f, -1000.0f };
	RtColor ambient = { 1.0f, 1.0f, 1.0f };
	RtColor specular = { 1.0f, 1.0f, 1.0f };
	RtColor diffuse = { 1.0f, 1.0f, 1.0f };
	RtColor reflectivity = { 1.0f, 1.0f, 1.0f };

	RtFloat ka = 0.5f;
	RtFloat kd = 0.75f;
	RtFloat ks = 0.9f;
	RtFloat alpha = 64.0f;

	// bumpy shader settings
	RtFloat max_displacement = 1.0f;
	int grid_width = 20;
	int grid_height = 10;
	unsigned int bumpiness = 1;
	unsigned int scaling_level = 2;
};

struct ShaderInfo_t {
	RtPoint *p;
	RtColor *c, *o;
	RtFloat u, v;
	RtVector *N;
	Texture_t textures[NUM_TEXTURES];
	// checkboard shading settings
	

	RtVector **gradients = NULL;
};

extern ShaderInfo_t shaderInfo;
extern ShaderSettings_t shaderSettings;

#define KA shaderSettings.ka
#define KD shaderSettings.kd 
#define KS shaderSettings.ks
#define ALPHA shaderSettings.alpha

#define LIGHT_SOURCE shaderSettings.light_source
#define AMBIENT shaderSettings.ambient
#define DIFFUSE shaderSettings.diffuse
#define SPECULAR shaderSettings.specular
#define REFLECTIVITY shaderSettings.reflectivity

#define SHADER_COLOR (*shaderInfo.c)
#define SHADER_POINT (*shaderInfo.p)
#define SHADER_NORMAL (*shaderInfo.N)

#define CHECK_SIZE_X (shaderSettings.check_size_x)
#define CHECK_SIZE_Y (shaderSettings.check_size_y)

#define TX shaderSettings.tx

#define GRID_WIDTH shaderSettings.grid_width
#define GRID_HEIGHT shaderSettings.grid_height
#define MAX_DISPLACEMENT shaderSettings.max_displacement
#define BUMPINESS shaderSettings.bumpiness
#define SCALING_LEVEL shaderSettings.scaling_level

extern const void(*CHECKERBOARD)(void);
extern const void(*LIGHTING)(void);
extern const void(*TEXTURE_SHADER)(void);
extern const void(*BUMPY_SHADER)(void);

void init_shader();
void free_shader();
void delete_texture(int tx);

#endif
