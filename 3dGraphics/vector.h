#pragma once


//typedef struct {
//	float x, y;
//} vec2_t;
//
//typedef struct {
//	float x, y, z;
//} vec3_t;


class vec2_t {
public:
	float x, y;
	vec2_t() : x(0), y(0) {}
	vec2_t(float x, float y) : x(x), y(y) {}
};

class vec3_t {
public:
	float x, y, z;
	vec3_t() : x(0), y(0), z(0) {}
	vec3_t(float x, float y, float z) : x(x), y(y), z(z) {}
	vec3_t vec3_rotate_x( float angle);
	vec3_t vec3_rotate_y(float angle);

	vec3_t vec3_rotate_z( float angle);

};