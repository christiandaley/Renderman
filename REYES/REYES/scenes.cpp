#include "stdafx.h"
#include <iostream>
#include "scenes.h"
#include "Ri.h"
#include "shaders.h"

void DifferentShaders() {
	RtColor green = { 0.0f, 1.0f, 0.0f };
	RtColor red = { 1.0f, 0.0f, 0.0f };
	RtColor blue = { 0.0f, 0.0f, 1.0f };
	RtColor yellow = { 1.0f, 1.0f, 0.0f };
	RtColor o1 = { 0.25f, 0.25f, 0.25f };
	RtColor o2 = { 1.0f, 1.0f, 1.0f };

	RiBegin(RI_NULL);
		RiFormat(400, 300, 1.0f);
		RiFrameAspectRatio(4.0f / 3.0f);
		RiDisplay("MyScenes/DifferentShaders.jpg", "", "");
		RiPixelSamples(2, 2);
		RiMakeTexture("textures/2k_earth_daymap.jpg", 0);
		RiFrameBegin(0);
			float fov = 45.0f;
			RiProjection(RI_PERSPECTIVE, "fov", &fov, RI_NULL);
			RiTranslate(0.0f, 0.0f, 25.0f);
			RiWorldBegin();
				RiTransformBegin();
					RiOpacity(o1);
					RiColor(blue);
					MAX_DISPLACEMENT = 0.025f;
					BUMPINESS = 6;
					RiDisplacement(BUMPY_SHADER);
					RiTranslate(0.0f, -1.00f, -24.5f);
					RiRotate(-100.0f, 1.0f, 0.0f, 0.0f);
					RiSphere(1.0f, 0.9f, 1.0f, 360.0f, RI_NULL);
				RiTransformEnd();
				RiTransformBegin();
					RiOpacity(o2);
					RiSurface(CHECKERBOARD);
					RiDisplacement(NULL);
					RiTranslate(-5.0f, 0.0f, 0.0f);
					RiRotate(60.0f, 1.0f, 0.0f, 0.0f);
					RiSphere(3.0f, -3.0f, 3.0f, 360.0f, RI_NULL);
				RiTransformEnd();
				RiTransformBegin();
					RiOpacity(o2);
					RiSurface(TEXTURE_SHADER);
					RiTranslate(2.0f, 2.0f, -10.0f);
					RiRotate(45.0f, 1.0f, 0.0f, 0.0f);
					RiRotate(180.0f, 0.0f, 0.0f, 1.0f);
					RiTorus(2.0f, 1.0f, 0.0f, 360.0f, 360.0f, RI_NULL);
				RiTransformEnd();
				RiTransformBegin();
					//RiOpacity(o1);
					RiColor(red);
					RiSurface(NULL);
					RiTranslate(1.9f, 2.5f, -11.0f);
					RiRotate(45.0f, 1.0f, 0.0f, 0.0f);
					RiTranslate(0.0f, 0.0f, -3.0f);
					RiCylinder(0.5f, 0.0f, 10.0f, 360.0f, RI_NULL);
				RiTransformEnd();
			RiWorldEnd();

		RiFrameEnd();
	RiEnd();
}

void FourPrimitives() {
	RtColor green = { 0.0f, 1.0f, 0.0f };
	RtColor red = { 1.0f, 0.0f, 0.0f };
	RtColor blue = { 0.0f, 0.0f, 1.0f };
	RtColor yellow = { 1.0f, 1.0f, 0.0f };
	RtColor opacity = { 0.7f, 0.7f, 0.7f };

	RiBegin(RI_NULL);
		//LIGHT_SOURCE[0] = 0.0f;
		//LIGHT_SOURCE[1] = 500.0f;
		RiFormat(800, 600, 1.0f);
		RiFrameAspectRatio(4.0f / 3.0f);
		RiDisplay("MyScenes/FourPrimitives.jpg", "", "");
		RiPixelSamples(2, 2);
		RiFrameBegin(0);
			float fov = 45.0f;
			RiProjection(RI_PERSPECTIVE, "fov", &fov, RI_NULL);
			RiTranslate(0.0f, 0.0f, 22.0f);

			RiWorldBegin();
			RiOpacity(opacity);
				RiTransformBegin();
					RiColor(green);
					RiTranslate(-5.0f, 3.0f, 0.0f);
					RiRotate(60.0f, 1.0f, 0.0f, 0.0f);
					RiRotate(90.0f, 0.0f, 0.0f, 1.0f);
					RiSphere(3.0f, -2.5f, 1.0f, 315.0f, RI_NULL);
				RiTransformEnd();
				RiTransformBegin();
					RiColor(red);
					RiTranslate(5.0f, 3.0f, 0.0f);
					RiRotate(60.0f, 1.0f, 0.0f, 0.0f);
					RiTorus(2.0f, 1.0f, 45.0f, 330.0f, 270.0f, RI_NULL);
				RiTransformEnd();
				RiTransformBegin();
					RiColor(blue);
					RiTranslate(-5.0f, -2.5f, 0.0f);
					RiRotate(60.0f, 1.0f, 0.0f, 0.0f);
					RiRotate(90.0f, 0.0f, 0.0f, 1.0f);
					RiCylinder(2.0f, -2.0f, 2.0f, 270.0f, RI_NULL);
				RiTransformEnd();
				RiTransformBegin();
					RiColor(yellow);
					RiTranslate(5.0f, -4.0f, 0.0f);
					RiRotate(-60.0f, 1.0f, 0.0f, 0.0f);
					//RiRotate(90.0f, 0.0f, 0.0f, 1.0f);
					RiCone(4.0f, 2.0f, 315.0f, RI_NULL);
				RiTransformEnd();
			RiWorldEnd();
		RiFrameEnd();

	RiEnd();
}

void Earth()
{
	RiBegin(RI_NULL);
	RiFormat(400, 300, 1.0);
	RiFrameAspectRatio(4.0f / 3.0f);
	RiDisplay("MoreScenes/Earth.png", "", "");
	RiPixelSamples(2, 2);
	RiMakeTexture("textures/2k_earth_daymap.jpg", 0);

	RiFrameBegin(0);
		/* set the perspective transformation */
		float fov = 45.0;
		RiProjection(RI_PERSPECTIVE, "fov", &fov, RI_NULL);

		RiWorldBegin();
			RiTransformBegin();
				RiSurface(TEXTURE_SHADER);
				RtColor blue = { 0, 0, 1 };
				RtColor opacity = { .9, .9, .9 };
				RiColor(blue);
				RiOpacity(opacity);
				MAX_DISPLACEMENT = 0.5f;
				BUMPINESS = 4;
				RiDisplacement(BUMPY_SHADER);
				RiTranslate(0, 0, 5.0);
				RiRotate(-175, 0, 1, 0);
				RiRotate(110, 1, 0, 0);
				RiSphere(1, -1, 1, 360, "texture", 0, RI_NULL);
			RiTransformEnd();
		RiWorldEnd();

	RiFrameEnd();

	RiEnd();
}

void Tunnel()
{
	RiBegin(RI_NULL);
	RiFormat(400, 300, 1.0);
	RiFrameAspectRatio(4.0f / 3.0f);
	RiDisplay("MoreScenes/Tunnel.png", "", "");
	RiPixelSamples(2, 2);

	RiFrameBegin(0);
		
		float fov = 45.0;
		RiProjection(RI_PERSPECTIVE, "fov", &fov, RI_NULL);

		RiWorldBegin();
			RiTransformBegin();
				RtColor color = { 1,0,0 };
				RiColor(color);
				RiTranslate(0, 0.5, 7.0);
				RiRotate(60, 1, 0, 0);
				RiTorus(1, .25, 0, 360, 360, RI_NULL);
			RiTransformEnd();
			RiTransformBegin();
				color[0] = 0; color[1] = 1;
				RiColor(color);
				RiTranslate(0, 0, 8.0);
				RiRotate(60, 1, 0, 0);
				RiRotate(30, 0, 1, 0);
				RiCylinder(1, -1, 1, 360, RI_NULL);
			RiTransformEnd();
			RiTransformBegin();
				color[1] = 0; color[2] = 1;
				RiColor(color);
				RiTranslate(0, 1, 9.0);
				RiRotate(60, 1, 0, 0);
				RiSphere(1.0, -1.0, 1.0, 360, RI_NULL);
			RiTransformEnd();
			RiTransformBegin();
				color[0] = 1; color[1] = .4; color[2] = .4;
				RiColor(color);
				CHECK_SIZE_X = 40;
				CHECK_SIZE_Y = 40;
				RiSurface(CHECKERBOARD);
				RiTranslate(0, -1, 8.5);
				RiRotate(-160, 1, 0, 0);
				RiRotate(30, 0, 1, 0);
				RiCone(2, 1, 360, RI_NULL);
			RiTransformEnd();
			RiTransformBegin();
				CHECK_SIZE_X = 40;
				CHECK_SIZE_Y = 40;
				RiTranslate(0, 0, 7.0);
				RiCylinder(3, 0, 10, 360, RI_NULL);
			RiTransformEnd();
		RiWorldEnd();

	RiFrameEnd();

	RiEnd();
}

void Rocket()
{
	RiBegin(RI_NULL);
	RiFormat(400, 300, 1.0);
	RiFrameAspectRatio(4.0f / 3.0f);
	RiDisplay("MoreScenes/Rocket.png", "", "");
	RiPixelSamples(2, 2);

	RiFrameBegin(0);
		/* set the perspective transformation */
		float fov = 45.0;
		RiProjection(RI_PERSPECTIVE, "fov", &fov, RI_NULL);
		RiWorldBegin();
			RiTransformBegin();
		// water
				RtColor color = { 0,0,1 };
				RtColor opacity = { .6,.6,.6 };
				RiOpacity(opacity);
				RiColor(color);
				RiDisplacement(BUMPY_SHADER);
				RiScale(7, 1.15, 1);
				RiTranslate(0, -0.75, 4);
				RiRotate(180, 0, 1, 0);
				RiRotate(110, 1, 0, 0);
				RiTranslate(0, 0, 1);
				RiRotate(180, 1, 0, 0);
				BUMPINESS = 7;
				MAX_DISPLACEMENT = 0.25f;
				RiSphere(1, .9, 1, 360, RI_NULL);
	// mountain
				color[0] = 0.6; color[1] = 0.3; color[2] = 0;
				opacity[0] = 1; opacity[1] = 1; opacity[2] = 1;
				RiColor(color);
				RiOpacity(opacity);
				RiDisplacement(NULL);
				RiDisplacement(BUMPY_SHADER);
				RiTranslate(0, 4, -6);
				RiRotate(15, 1, 0, 0);
				BUMPINESS = 5;
				MAX_DISPLACEMENT = 1.0f;
				RiSphere(7, 6.8, 7, 360, RI_NULL);
			RiTransformEnd();
			RiTransformBegin();
				// rocket
				RiDisplacement(NULL);
				color[0] = .8; color[1] = 0.8; color[2] = .8;
				RiColor(color);
				RiTranslate(-.25, .25, 2);
				RiRotate(40, 0, 0, 1);
				RiScale(1, 2, 1);
				RiRotate(150, 0, 1, 0);
				RiRotate(-90, 1, 0, 0);
				RiSphere(.1, -.1, .1, 360, RI_NULL);

				RiTranslate(0, 0, .07);
				RiCone(.075, .075, 360, RI_NULL);

				RiSurface(NULL);
				RiDisplacement(BUMPY_SHADER);
				color[0] = 1; color[1] = 0.27; color[2] = 0;
				opacity[0] = .4; opacity[1] = .4; opacity[2] = .4;
				RiOpacity(opacity);
				RiColor(color);
				RiTranslate(0, 0, -.45);
				BUMPINESS = 6;
				MAX_DISPLACEMENT = 0.075f;
				RiCone(.3, .1, 360, RI_NULL);

				color[1] = .1;
				RiColor(color);
				opacity[0] = 1; opacity[1] = 1; opacity[2] = 1;
				RiOpacity(opacity);
				RiDisplacement(NULL);
				RiScale(20, .5, 1);
				RiTranslate(0, 0, .375);
				RiSphere(.025, -.025, .025, 360, RI_NULL);

				RiScale(1 / 40.0f, 16, 1);
				RiTranslate(0, -0.015, -.075);
				RiSphere(.025, -.025, .025, 360, RI_NULL);
			RiTransformEnd();
		RiWorldEnd();

	RiFrameEnd();

	RiEnd();
}

void ShaderTest()
{
	RiBegin(RI_NULL);
		RiFormat(400, 300, 1.0);
		RiFrameAspectRatio(4.0f / 3.0f);
		RiDisplay("MoreScenes/ShaderTest.png", "", "");
		RiPixelSamples(2, 2);
		RiMakeTexture("textures/2k_earth_daymap.jpg", 0);

		RiFrameBegin(0);
		/* set the perspective transformation */
			float fov = 45.0;
			RiProjection(RI_PERSPECTIVE, "fov", &fov, RI_NULL);
			RiWorldBegin();
			KS = 0.7f;
				RiTransformBegin();
					RtColor color = { 1,0,0 };
					RtColor opacity = { .4,.4,.4 };
					RiOpacity(opacity);
					RiColor(color);
					CHECK_SIZE_X = 20;
					CHECK_SIZE_Y = 10;
					RiSurface(CHECKERBOARD);
					RiTranslate(0, 0.5, 11);
					RiRotate(70, 0, 1, 0);
					RiTorus(2.5, .5, 0, 360, 360, RI_NULL);
				RiTransformEnd();
				RiTransformBegin();
					color[0] = 0; color[1] = 1; color[2] = 0;
					opacity[0] = 1; opacity[1] = 1; opacity[2] = 1;
					RiOpacity(opacity);
					RiSurface(NULL);
					RiColor(color);
					RiTranslate(3, -1.5, 11);
					RiRotate(90, 1, 0, 0);
					RiCylinder(1, -1, 1, 360, RI_NULL);
				RiTransformEnd();
				RiTransformBegin();
					opacity[0] = 1; opacity[1] = 1; opacity[2] = 1;
					RiOpacity(opacity);
					RiSurface(TEXTURE_SHADER);
					MAX_DISPLACEMENT = 0.3f;
					RiDisplacement(BUMPY_SHADER);
					RiTranslate(3, 1, 11.0);
					RiRotate(-40, 0, 0, 1);
					RiRotate(-100, 0, 1, 0);
					RiRotate(110, 1, 0, 0);
					BUMPINESS = 3;
					MAX_DISPLACEMENT = 0.3f;
					RiSphere(1, -1.5, 1.5, 360, "texture", 0, RI_NULL);
				RiTransformEnd();
				RiTransformBegin();
					opacity[0] = .8; opacity[1] = .8; opacity[2] = .8;
					RiOpacity(opacity);
					RiSurface(TEXTURE_SHADER);
					RiDisplacement(NULL);
					RiTranslate(-2.5, 1, 11.0);
					RiRotate(-175, 0, 1, 0);
					RiRotate(110, 1, 0, 0);
					RiSphere(1.5, -1.5, 1.5, 360, "texture", 0, RI_NULL);
				RiTransformEnd();
				RiTransformBegin();
					color[0] = 0.4; color[1] = 0.2; color[2] = 0;
					RiColor(color);
					opacity[0] = 1; opacity[1] = 1; opacity[2] = 1;
					RiOpacity(opacity);
					RiSurface(NULL);
					RiDisplacement(BUMPY_SHADER);
					RiTranslate(-2.5, -2.5, 11);
					RiRotate(-90, 1, 0, 0);
					RiRotate(90, 0, 0, 1);
					BUMPINESS = 3;
					MAX_DISPLACEMENT = 0.3f;
					RiCone(2.5, 1, 360, RI_NULL);
				RiTransformEnd();
			RiWorldEnd();

		RiFrameEnd();

	RiEnd();
}

void TransparencyTest()
{
	RiBegin(RI_NULL);
		RiFormat(400, 300, 1.0f);
		RiFrameAspectRatio(4.0f / 3.0f);
		RiDisplay("MoreScenes/TransparencyTest.png", "", "");
		RiPixelSamples(2, 2);

		RiFrameBegin(0);
			/* set the perspective transformation */
			float fov = 45.0f;
			RiProjection(RI_PERSPECTIVE, "fov", &fov, RI_NULL);
			RiWorldBegin();
				RiTransformBegin();
					RtColor color = { 1,0,0 };
					RtColor opacity = { .4,.4,.4 };
					RiOpacity(opacity);
					RiColor(color);
					RiTranslate(0, 0.5, 7.0);
					RiRotate(60, 1, 0, 0);
					RiTorus(1, .25, 0, 250, 270, RI_NULL);
				RiTransformEnd();
				RiTransformBegin();
					color[0] = 0; color[1] = 1;
					opacity[0] = 0.4; opacity[1] = 0.4; opacity[2] = 0.4;
					RiOpacity(opacity);
					RiColor(color);
					RiTranslate(0, 0, 8.0);
					RiRotate(60, 1, 0, 0);
					RiRotate(30, 0, 1, 0);
					RiCylinder(1, -1, 1, 300, RI_NULL);
				RiTransformEnd();
				RiTransformBegin();
					color[1] = 0; color[2] = 1;
					opacity[0] = .7; opacity[1] = .7; opacity[2] = .7;
					RiOpacity(opacity);
					RiColor(color);
					RiTranslate(0, 1, 9.0);
					RiRotate(60, 1, 0, 0);
					RiSphere(1.8, -1.0, 1.0, 250, RI_NULL);
				RiTransformEnd();
				RiTransformBegin();
					color[0] = 1; color[1] = .6; color[2] = .6;
					RiColor(color);
					opacity[0] = .6; opacity[1] = .6; opacity[2] = .6;
					RiOpacity(opacity);
					RiTranslate(0, -1, 8.5);
					RiRotate(-160, 1, 0, 0);
					RiRotate(30, 0, 1, 0);
					RiRotate(140, 0, 0, 1);
					RiCone(2.5f, 1.0f, 270.0f, RI_NULL);
				RiTransformEnd();
			RiWorldEnd();

		RiFrameEnd();

	RiEnd();
}

void SampleScene1() {
	int i;
	int nf;
	float slopex, slopey, slopez;
	char name[50];

	RtColor red = { 1,0,0 };
	RtColor green = { 0,1,0 };
	RtColor blue = { 0,0,1 };
	RtColor white = { 1,1,1 };


	RtPoint p1 = { 30,0,10 }; /* ball's initial position */
	RtPoint p2 = { 0,20,10 }; /* ball's final position  */


	RtFloat fov = 45;
	RtFloat intensity1 = 0.1;
	RtFloat intensity2 = 1.5;
	RtInt init = 0, end = 1;


	nf = 100; /* number of frames to output */
	slopex = (p2[0] - p1[0]) / nf;
	slopey = (p2[1] - p1[1]) / nf;
	slopez = (p2[2] - p1[2]) / nf;

	RiBegin(RI_NULL);
		RiFormat(320, 240, 1);
		RiPixelSamples(2, 2);

		/* loop through all the frames */
		for (i = 1; i <= nf; i++) {
			RiFrameBegin(i);
				sprintf_s(name, "SampleScene1/image_%02d.tif", i - 1);
				RiDisplay(name, "file", "rgb", RI_NULL);

				RiProjection("perspective", "fov", &fov, RI_NULL);
				RiTranslate(0, -5, 60);
				RiRotate(-120, 1, 0, 0);
				RiRotate(25, 0, 0, 1);

				RiWorldBegin();
					RiColor(blue);
					RiTransformBegin();
						RiCylinder(1, 0, 20, 360, RI_NULL);
						RiTranslate(0, 0, 20);
						RiCone(2, 2, 360, RI_NULL);
					RiTransformEnd();

					RiColor(green);
					RiTransformBegin();
						RiRotate(-90, 1, 0, 0);
						RiCylinder(1, 0, 20, 360, RI_NULL);
						RiTranslate(0, 0, 20);
						RiCone(2, 2, 360, RI_NULL);
					RiTransformEnd();

					RiColor(red);
					RiTransformBegin();
						RiRotate(90, 0, 1, 0);
						RiCylinder(1, 0, 20, 360, RI_NULL);
						RiTranslate(0, 0, 20);
						RiCone(2, 2, 360, RI_NULL);
					RiTransformEnd();


					RiColor(white);
					RiTransformBegin();
						RiTranslate(p1[0] + slopex * (i - 1), p1[1] + slopey * (i - 1), p1[2] + slopez * (i - 1));
						RiSphere(5, -5, 5, 360, RI_NULL);
					RiTransformEnd();
				RiWorldEnd();

		/* when you hit this command you should output the final image for this frame */
			RiFrameEnd();
		}
	RiEnd();
};