#pragma once


#include "vector.h"
#include <stdint.h>
class face_t {
public:
	int a;
	int b;
	int c;
	face_t() : a(0), b(0), c(0) {}
	face_t(int a, int b, int c) : a(a), b(b), c(c) {}
};

class triangle_t {
public:
	vec2_t points[3];
	triangle_t() {}
	triangle_t(vec2_t p1, vec2_t p2, vec2_t p3) {
		points[0] = p1;
		points[1] = p2;
		points[2] = p3;
	}
	
	void draw_filled_triangle(uint32_t color);
	void draw_triangle(uint32_t color);
};


