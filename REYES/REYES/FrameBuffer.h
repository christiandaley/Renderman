#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <string>
#include <vector>
#include "Ri.h"
#include "RtPrimitive.h"
#include "Micropolygon.h"

struct SampleList_t {
	RtFloat depth;
	RtColor color, opacity;
	SampleList_t *next, *prev;
};

struct Sample_t {
	RtFloat x, y;
	SampleList_t *list;
};

struct Pixel_t {
	Sample_t *samples;
};

class FrameBuffer
{
	Pixel_t *pixels;
	int xres, yres;
	RtInt xsamples, ysamples;
public:
	FrameBuffer(int xres, int yres, RtInt xs, RtInt ys);
	Pixel_t &pixelAt(int x, int y);
	void render(RtPrimitive *obj);
	void sample(Micropolygon &mp);
	void processSample(Micropolygon &mp, Sample_t &s);
	void draw(const wchar_t *file, const char *file_extension);
	void extractColor(RtColor color, SampleList_t *list);
	void addSampleEntry(Sample_t &s, RtFloat depth, RtColor color, RtColor opacity);
	void deleteList(SampleList_t *list);
	unsigned int size(); // gets the size of the FrameBuffer in Mb
	~FrameBuffer();
};

#endif