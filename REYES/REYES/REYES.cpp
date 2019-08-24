// REYES.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "scenes.h"
#include "NiceScenes.h"
#include "Ri.h"

/*
I had to delete the individual frames of my animations because they 
took up too much space and Gauchospace only allows 20Mb files
*/

int main()
{/*
	RiBegin(RI_NULL);
		RiDisplay("sphere.jpg", NULL, NULL);
		RiFormat(320.0f, 240.0f, 1.0f);
		RiFrameAspectRatio(320.0f / 240.0f);
		RiPixelSamples(2, 2);
		float fov = 45.0f;
		RtColor blue = { 0.0f, 0.0f, 1.0f };
		RiProjection(RI_PERSPECTIVE, "fov", &fov, RI_NULL);
		RiTranslate(0.0f, 0.0f, 20.0f);
		RiRotate(90.0f, 1.0f, 0.0f, 0.0f);
		RiFrameBegin(0);
			RiWorldBegin();
				RiTransformBegin();
					RiColor(blue);
					RiSphere(5.0f, -5.0f, 5.0f, 360.0f, RI_NULL);
				RiTransformEnd();
			RiWorldEnd();
			
		RiFrameEnd();

	RiEnd();
	*/


	SaturnAndRings(); // takes an hour to render (1 million spheres to make the ring)
	//EarthAndMoon();
	//DifferentShaders();
	//FourPrimitives();
	//Earth();
	//Tunnel();
	//Rocket();
	//ShaderTest();
	//TransparencyTest();
	//SampleScene1();
    return 0;
}

