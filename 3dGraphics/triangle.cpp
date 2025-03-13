#include "triangle.h"
#include "display.h"
#include <algorithm>

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	float inv_slope1 = (float)(x1 - x0) / (y1 - y0);
	float inv_slope2 = (float)(x2 - x0) / (y2 - y0);

	float x_start = x0;
	float x_end = x0;

	for (int y = y0; y <= y2; y++) {
		draw_line(x_start, y, x_end, y, color);
		x_start += inv_slope1;
		x_end += inv_slope2;
	}
}
void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	float inv_slope1 = (float)(x2 - x0) / (y2 - y0);
	float inv_slope2 = (float)(x2 - x1) / (y2 - y1);
	float x_start = x2;
	float x_end = x2;

	for (int y = y2; y >= y0; y--) {
		draw_line(x_start, y, x_end, y, color);
		x_start -= inv_slope1;
		x_end -= inv_slope2;
	}

}
void triangle_t::draw_filled_triangle(uint32_t color)
{
	float y0 = points[0].y;
	float y1 = points[1].y;
	float y2 = points[2].y;
	float x0 = points[0].x;
	float x1 = points[1].x;
	float x2 = points[2].x;

	if (y0 > y1) {
		std::swap(y0, y1);
		std::swap(x0, x1);
	}
	if (y1 > y2) {
		std::swap(y1, y2);
		std::swap(x1, x2);
	}
	if (y0 > y1) {
		std::swap(y0, y1);
		std::swap(x0, x1);
	}


	if (y1 == y2) {
		// Draw flat-bottom triangle
		fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
	}
	else if (y0 == y1) {
		// Draw flat-top triangle
		fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, color);
	}
	else {
		// Calculate the new vertex (Mx,My) using triangle similarity
		int My = y1;
		int Mx = (((x2 - x0) * (y1 - y0)) / (y2 - y0)) + x0;

		// Draw flat-bottom triangle
		fill_flat_bottom_triangle(x0, y0, x1, y1, Mx, My, color);

		// Draw flat-top triangle
		fill_flat_top_triangle(x1, y1, Mx, My, x2, y2, color);
	}
}

void triangle_t::draw_triangle(uint32_t color)
{
	draw_line(points[0].x, points[0].y, points[1].x, points[1].y, color);
	draw_line(points[1].x, points[1].y, points[2].x, points[2].y, color);
	draw_line(points[2].x, points[2].y, points[0].x, points[0].y, color);
}
