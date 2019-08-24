#include "stdafx.h"
#include <cmath>
#include <time.h>
#include <iostream>
#include "Ri.h"
#include "shaders.h"

void SaturnAndRings() {
	const int n_particles = 1500000;
	const float psize = 0.006f;
	const float ring_radius = 6.0f;
	const float ring_width = 4.5f;
	const float ring_height = 0.1f;

	char name[64];

	RtColor o1 = { 1.0f, 1.0f, 1.0f };
	RtColor o2 = { 0.9f, 0.9f, 0.9f };
	RtColor c;

	srand(time(0));
	RtPoint *particles = new RtPoint[n_particles];
	for (int i = 0; i < n_particles; i++) {
		particles[i][0] = (float)rand() * 2.0f * MY_PI / RAND_MAX; // theta
		particles[i][1] = (float)rand() * ring_width / RAND_MAX; // distance from saturn
		particles[i][2] = (float)rand() * ring_height / RAND_MAX; // height deviation
	}

	RiBegin(RI_NULL);
		RiFormat(800, 600, 1.0f);
		RiFrameAspectRatio(4.0f / 3.0f);
		RiPixelSamples(2.0f, 2.0f);
		RiMakeTexture("textures/2k_saturn.jpg", 0);
		RiMakeTexture("textures/2k_saturn_ring_alpha.png", 1);
		LIGHT_SOURCE[0] = -1000.0f;
		LIGHT_SOURCE[1] = 500.0f;
		LIGHT_SOURCE[2] = -250.0f;
		Texture_t *ring_tx = &shaderInfo.textures[1];
		RiFrameBegin(0);
			RiDisplay("NiceScenes/SaturnAndRings/Saturn3.jpg", RI_NULL, RI_NULL);
			float fov = 45.0f;
			RiProjection(RI_PERSPECTIVE, "fov", &fov, RI_NULL);
			RiTranslate(0.0f, 0.0f, 20.0f);
			RiRotate(-15.0f, 1.0f, 0.0f, 0.0f);
			RiWorldBegin();
				RiOpacity(o1);
				RiTransformBegin(); // Saturn
					RiSurface(TEXTURE_SHADER);
					KA = 0.3f;
					KD = 0.9f;
					KS = 0.6f;
					TX = 0;
					RiRotate(90.0f, 1.0f, 0.0f, 0.0f);
					RiSphere(5.0f, -5.0f, 5.0f, 360.0f, RI_NULL);
				RiTransformEnd();
				RiOpacity(o2);
				RiSurface(NULL);
				KS = 1.0f;
				// Particles in Saturn's rings
				for (int i = 0; i < n_particles; i++) {
					float theta = particles[i][0];
					float d = ring_radius + particles[i][1];

					// HACK. We access the texture directly without invoking a shader
					int depth = ring_tx->width * particles[i][1] / ring_width;
					memcpy(c, ring_tx->colors[depth], sizeof(RtColor));
					RiTransformBegin();
						RiColor(c);
						RiTranslate(d * cos(theta),
									particles[i][2], 
									d * sin(theta));

						RiSphere(psize, -psize, psize, 360.0f, RI_NULL);

					RiTransformEnd();
				}
			RiWorldEnd();
		RiFrameEnd();
		
	RiEnd();

	delete[] particles;
}

void EarthAndMoon() {
	int nf = 500;
	char name[64];
	const float EARTH_ORBIT = 16.5f;
	const float EARTH_RADIUS = 1.0f;
	const float MOON_ORBIT = 2.0f;
	const float MOON_RADIUS = 0.25f;
	const float MOON_FREQ = 2.0f;
	const float SUN_RADIUS = 3.0f;

	const float MERC_ORBIT = 5.0f;
	const float MERC_RADIUS = 0.6f;

	const float VENUS_ORBIT = 10.0f;
	const float VENUS_RADIUS = 0.8f;

	float earthX, earthZ, moonX, moonZ, mercX, mercZ, venusX, venusZ;
	RiBegin(RI_NULL);
		RiFormat(800, 600, 1.0f);
		RiFrameAspectRatio(4.0f / 3.0f);
		RiPixelSamples(2, 2);
		RiMakeTexture("textures/2k_earth_daymap.jpg", 0);
		RiMakeTexture("textures/2k_moon.jpg", 1);
		RiMakeTexture("textures/2k_mercury.jpg", 2);
		RiMakeTexture("textures/2k_venus_atmosphere.jpg", 3);
		RiMakeTexture("textures/2k_sun.jpg", 4);
		for (int i = 0; i < nf; i++) {
			RiFrameBegin(i);
				
				sprintf_s(name, "NiceScenes/EarthAndPlanets/frame%i.jpg", i);
				RiDisplay(name, RI_NULL, RI_NULL);
				float fov = 45.0f;
				RiProjection(RI_PERSPECTIVE, "fov", &fov, RI_NULL);
				RiTranslate(0.0f, 0.0f, 20.0f);
				earthX = EARTH_ORBIT * cos(MY_PI * 2.0f * i / nf);
				earthZ = EARTH_ORBIT * sin(MY_PI * 2.0f * i / nf);
				moonX = MOON_ORBIT * cos(270.0f + MY_PI * MOON_FREQ * 2.0f * i / nf);
				moonZ = MOON_ORBIT * sin(270.0f + MY_PI * MOON_FREQ * 2.0f * i / nf);

				mercX = MERC_ORBIT * cos(80.0f + MY_PI * 2.0f * i / nf);
				mercZ = MERC_ORBIT * sin(80.0f + MY_PI * 2.0f * i / nf);

				venusX = VENUS_ORBIT * cos(190.0f + MY_PI * 2.0f * i / nf);
				venusZ = VENUS_ORBIT * sin(190.0f + MY_PI * 2.0f * i / nf);

				RiWorldBegin();
					LIGHT_SOURCE[0] = 0.0f;
					LIGHT_SOURCE[1] = 0.0f;
					LIGHT_SOURCE[2] = 0.0f;
					KA = 0.3f;
					KD = 0.9f;
					KS = 0.8f;
					RiTransformBegin(); // earth
						RiSurface(TEXTURE_SHADER);
						RiDisplacement(NULL);
						RiTranslate(earthX, 0.0f, earthZ);
						RiRotate(360.0f * 3.0f * i / nf, 0.0f, 1.0f, 0.0f);
						RiRotate(90.0f, 1.0f, 0.0f, 0.0f);
						TX = 0;
						RiSphere(EARTH_RADIUS, -EARTH_RADIUS, EARTH_RADIUS, 360.0f, RI_NULL);


					RiTransformEnd();
					RiTransformBegin(); // moon
						RiTranslate(earthX + moonX, 0.0f, earthZ + moonZ);
						RiRotate(360.0f * MOON_FREQ * i / nf, 0.0f, 1.0f, 0.0f);
						RiRotate(90.0f, 1.0f, 0.0f, 0.0f);
						TX = 1;
						RiSphere(MOON_RADIUS, -MOON_RADIUS, MOON_RADIUS, 360.0f, RI_NULL);
					RiTransformEnd();
					RiTransformBegin(); // mercury
						RiRotate(10.0f, 0.0f, 0.0f, 1.0f);
						RiTranslate(mercX, 0.0f, mercZ);
						RiRotate(360.0f * 2.0f * i / nf, 0.0f, 1.0f, 0.0f);
						RiRotate(90.0f, 1.0f, 0.0f, 0.0f);
						TX = 2;
						RiSphere(MERC_RADIUS, -MERC_RADIUS, MERC_RADIUS, 360.0f, RI_NULL);
					RiTransformEnd();
					RiTransformBegin(); // venus
						RiRotate(-8.0f, 0.0f, 0.0f, 1.0f);
						RiTranslate(venusX, 0.0f, venusZ);
						RiRotate(360.0f * 2.0f * i / nf, 0.0f, 1.0f, 0.0f);
						RiRotate(90.0f, 1.0f, 0.0f, 0.0f);
						TX = 3;
						RiSphere(VENUS_RADIUS, -VENUS_RADIUS, VENUS_RADIUS, 360.0f, RI_NULL);


					RiTransformEnd();
					RiTransformBegin(); // sun
						RiRotate(360.0f * i / nf, 0.0f, 1.0f, 0.0f);
						RiRotate(90.0f, 1.0f, 0.0f, 0.0f);
						KA = 1.0f;
						KD = 0.0f;
						KS = 0.0f;
						TX = 4;
						RiSphere(SUN_RADIUS, -SUN_RADIUS, SUN_RADIUS, 360.0f, RI_NULL);
					RiTransformEnd();

				RiWorldEnd();
			RiFrameEnd();
		}

	RiEnd();
}