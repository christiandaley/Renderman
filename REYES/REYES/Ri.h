#ifndef RI_H
#define RI_H

#define MY_PI 3.141592653589793238f
//#define RI_DEBUG
#define RI_RENDERING_PROGRESS
#define DO_LIGHT_SHADING 1

typedef short RtBoolean;
typedef int RtInt;
typedef float RtFloat;
typedef char *RtToken;
typedef RtFloat RtColor[3];
typedef RtFloat RtPoint[3];
typedef RtFloat RtVector[3];
typedef RtFloat RtNormal[3];
typedef RtFloat RtHpoint[4];
typedef RtFloat RtMatrix[4][4];
typedef RtFloat RtBasis[4][4];
typedef RtFloat RtBound[6];
typedef char *RtString;
typedef void *RtPointer;

#define RI_NULL NULL
#define RtVoid void

extern RtToken RI_PERSPECTIVE, RI_ORTHOGRAPHIC;

struct Screen_t {
	RtFloat aspect = 1.0, pixelaspect = 1.0;
	RtInt xres, yres;
	RtFloat xsamples, ysamples;
};

struct Camera_t {
	float fov = 90.0f;
	RtFloat hither = 0.1f;
	RtFloat yon = 1000.0f;
	RtPoint location; // the location of the camera in world space
};

struct Settings_t {
	Screen_t Screen;
	Camera_t Camera;
	RtMatrix current_transform;
	RtMatrix world_to_camera;
	RtMatrix camera_to_screen;
};

extern RtMatrix current_transform;
extern RtMatrix world_to_camera;
extern RtMatrix camera_to_screen;

extern Screen_t Screen;
extern Camera_t Camera;
extern Settings_t settings;

extern const void(*surfaceShader)(void);
extern const void(*displacementShader)(void);

//void print_tmatrix();
void print_matrix(RtMatrix m);
void print_screen_trans();
void print_curent_transform();
void print_point(RtPoint p);
void point_transform(RtMatrix A, RtPoint b, RtPoint c);
void matrix_mul(RtMatrix A, RtMatrix B, RtMatrix C);
RtFloat dot_product(RtVector a, RtVector b);
// computed a*b and stores the result in c
void cross_product(RtVector a, RtVector b, RtVector c);
void normalize(RtVector a);
unsigned int make_color(RtColor c);
void mcopy(RtMatrix dst, RtMatrix src);
void get_obj_to_pixels(RtMatrix obj_to_pixels, RtMatrix obj_to_world);

RtVoid RiBegin(RtToken name);
RtVoid RiEnd();
RtVoid RiFrameBegin(RtInt frame);
RtVoid RiFrameEnd();
RtVoid RiWorldBegin();
RtVoid RiWorldEnd();
RtVoid RiFormat(RtInt xres, RtInt yres, RtFloat aspect);
RtVoid RiProjection(RtToken name, ...);
RtVoid RiFrameAspectRatio(RtFloat aspect);
RtVoid RiPixelSamples(RtFloat xsamples, RtFloat ysamples);
RtVoid RiDisplay(char *name, RtToken type, RtToken mode, ...);

RtVoid RiTransformBegin();
RtVoid RiTransformEnd();
RtVoid RiIdentity();
RtVoid RiTransform(RtMatrix transform);
RtVoid RiPerspective(RtFloat fov);
RtVoid RiTranslate(RtFloat dx, RtFloat dy, RtFloat dz);
RtVoid RiRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz);
RtVoid RiScale(RtFloat sx, RtFloat sy, RtFloat sz);
RtVoid RiConcatTransform(RtMatrix transform);

RtVoid RiSphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, ...);
RtVoid RiTorus(RtFloat majorradius, RtFloat minorradius, RtFloat phimin, RtFloat phimax, RtFloat thetamax, ...);
RtVoid RiCone(RtFloat height, RtFloat radius, RtFloat thetamax, ...);
RtVoid RiCylinder(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, ...);

RtVoid RiColor(RtColor color);
RtVoid RiOpacity(RtColor opacity);

RtVoid RiSurface(const void(*shader)(void));
RtVoid RiDisplacement(const void(*shader)(void));

RtVoid RiMakeTexture(const char *name, unsigned int index);

#endif