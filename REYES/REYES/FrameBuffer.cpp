#include "stdafx.h"
#include <iostream>
#include <atlimage.h>
#include <Gdiplusimaging.h>
#include <Gdiplus.h>
#include <assert.h>
#include "FrameBuffer.h"
#include "Micropolygon.h"

using namespace std;

FrameBuffer::FrameBuffer(int xres, int yres, RtInt xs, RtInt ys) {
	this->xres = xres;
	this->yres = yres;
	this->xsamples = xs;
	this->ysamples = ys;
	this->pixels = new Pixel_t[xres * yres];
	for (int j = 0; j < yres; j++) {
		for (int i = 0; i < xres; i++) {
			Pixel_t &pixel = this->pixelAt(i, j);
			pixel.samples = new Sample_t[xs * ys];
			for (int y = 0; y < this->ysamples; y++) {
				for (int x = 0; x < this->xsamples; x++) {
					Sample_t &s = pixel.samples[y * this->xsamples + x];
					s.x = (RtFloat)i + ((RtFloat)x + (RtFloat)rand() / RAND_MAX) / (RtFloat)this->xsamples;
					s.y = (RtFloat)j + ((RtFloat)y + (RtFloat)rand() / RAND_MAX) / (RtFloat)this->ysamples;
					s.list = NULL;
					
				}
			}
		}
	}

}

void FrameBuffer::draw(const wchar_t *file, const char *file_extension) {
	CImage image = CImage();
	image.Create(xres, yres, 24, 0);

	COLORREF rgb;
	RtColor color, temp;
	SampleList_t *list;

	for (int j = 0; j < yres; j++) {
		for (int i = 0; i < xres; i++) {
			Pixel_t &pixel = this->pixelAt(i, j);
			color[0] = 0.0f;
			color[1] = 0.0f;
			color[2] = 0.0f;
			for (int k = 0; k < this->xsamples * this->ysamples; k++) {
				extractColor(temp, pixel.samples[k].list);
				color[0] += temp[0];
				color[1] += temp[1];
				color[2] += temp[2];
			}

			color[0] /= (RtFloat)(xsamples * ysamples);
			color[1] /= (RtFloat)(xsamples * ysamples);
			color[2] /= (RtFloat)(xsamples * ysamples);

			rgb = make_color(color);
			image.SetPixel(i, yres - j - 1, rgb);
		}
	}

	GUID type = Gdiplus::ImageFormatJPEG;
	if (strcmp(file_extension, "png") == 0)
		type = Gdiplus::ImageFormatPNG;
	else if (strcmp(file_extension, "tif") == 0 || strcmp(file_extension, "tiff") == 0)
		type = Gdiplus::ImageFormatTIFF;
	else if (strcmp(file_extension, "bmp") == 0)
		type = Gdiplus::ImageFormatBMP;

	image.Save(file, type);
}


void FrameBuffer::extractColor(RtColor color, SampleList_t *list) {
	if (list == NULL) {
		color[0] = 0.0f;
		color[1] = 0.0f;
		color[2] = 0.0f;
		return;
	}

	RtColor temp;
	extractColor(temp, list->next);
	
	color[0] = list->opacity[0] * list->color[0] + (1.0f - list->opacity[0]) * temp[0];
	color[1] = list->opacity[1] * list->color[1] + (1.0f - list->opacity[1]) * temp[1];
	color[2] = list->opacity[2] * list->color[2] + (1.0f - list->opacity[2]) * temp[2];
}

void FrameBuffer::render(RtPrimitive *obj) {

	Mesh *mesh = obj->mesh;
	for (int j = 0; j < mesh->height - 1; j++) {
		for (int i = 0; i < mesh->width - 1; i++) {
			// clockwise from top-left
			MeshEntry_t &v1 = mesh->at(i, j);
			MeshEntry_t &v2 = mesh->at(i + 1, j);
			MeshEntry_t &v3 = mesh->at(i + 1, j + 1);
			MeshEntry_t &v4 = mesh->at(i, j + 1);
			Micropolygon mp = Micropolygon(v1, v2, v3, v4);
			if (mp.onScreen(this->xres, this->yres))
				sample(mp);
		}
	}
}

void FrameBuffer::sample(Micropolygon &mp) {

	RtInt left, right, top, bottom;
	mp.pixelBounds(this->xres, this->yres, left, right, top, bottom);

	for (int j = bottom; j <= top; j++) {
		for (int i = left; i <= right; i++) {
			Pixel_t &pixel = this->pixelAt(i, j);
			for (int k = 0; k < this->xsamples * this->ysamples; k++) {
				Sample_t &s = pixel.samples[k];
				processSample(mp, s);
			}

		}
	}

}

void FrameBuffer::processSample(Micropolygon &mp, Sample_t &s) {
	RtPoint p;
	RtColor color, opacity;
	RtFloat z;
	if (mp.contains(s.x, s.y, z, color, opacity)) {
		p[0] = s.x;
		p[1] = s.y;
		p[2] = z;

		if (z < 0.0f || z > 1.0f)
			return; // cull points that are outside the range of [0, 1] in their z coordinate

		addSampleEntry(s, z, color, opacity);

	}
}

void FrameBuffer::addSampleEntry(Sample_t &s, RtFloat depth, RtColor color, RtColor opacity) {
	SampleList_t *newList = new SampleList_t;
	memcpy(newList->color, color, sizeof(RtColor));
	memcpy(newList->opacity, opacity, sizeof(RtColor));
	newList->depth = depth;
	newList->next = NULL;
	newList->prev = NULL;

	if (s.list == NULL) {
		s.list = newList;
	}
	else {
		SampleList_t *list = s.list;
		while (list != NULL) {

			newList->prev = list;

			if (list->depth > newList->depth)
				break; // stop if we find where newList needs to be inserted
			
			if (list->opacity[0] >= 1.0f && list->opacity[1] >= 1.0f && list->opacity[2] >= 1.0f) {
				delete newList; // completely hidden, so no need to store it in our z-buffer
				return;
			}

			list = list->next;
		}
		
		if (list == NULL) {
			newList->prev->next = newList;
		}
		else { // insertion
			newList->next = list;
			newList->prev = list->prev;
			if (list->prev != NULL) {
				list->prev->next = newList;
				list->prev = newList;
				
			}
			else {
				list->prev = newList;
				s.list = newList;
			}

			// if a completely opaque sample is inserted, delete all samples after it in the list
			if (newList->opacity[0] >= 1.0f && newList->opacity[1] >= 1.0f && newList->opacity[2] >= 1.0f) {
				deleteList(newList->next);
				newList->next = NULL;
			}
		}
	}

}

Pixel_t &FrameBuffer::pixelAt(int x, int y) {
	return pixels[y * this->xres + x];
}

void FrameBuffer::deleteList(SampleList_t *list) {
	SampleList_t *temp;
	while (list != NULL) {
		temp = list->next;
		delete list;
		list = temp;
	}
}

unsigned int FrameBuffer::size() {
	unsigned int size = 0;

	for (int i = 0; i < xres * yres; i++) {
		size += sizeof(Pixel_t);
		for (int j = 0; j < xsamples * ysamples; j++) {
			size += sizeof(Sample_t);
			SampleList_t *list = pixels[i].samples[j].list;
			while (list != NULL) {
				size += sizeof(SampleList_t);
				list = list->next;
			}
		}
	}

	return (unsigned int)(size / 1e6);
}

FrameBuffer::~FrameBuffer()
{
	for (int i = 0; i < xres * yres; i++) {
		for (int j = 0; j < xsamples * ysamples; j++) {
			deleteList(this->pixels[i].samples[j].list);
		}

		delete[] this->pixels[i].samples;
	}

	delete[] pixels;
}
