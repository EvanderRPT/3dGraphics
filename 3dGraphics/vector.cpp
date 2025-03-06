#include "vector.h"
#include <corecrt_math.h>


vec3_t vec3_t::vec3_rotate_x( float angle) {
	vec3_t rotated_vector;
	rotated_vector.x = this->x;
	rotated_vector.y = (this->y * cos(angle)) - (this->z * sin(angle));
	rotated_vector.z = (this->y * sin(angle)) + (this->z * cos(angle));
	return rotated_vector;
}

vec3_t vec3_t::vec3_rotate_y( float angle) {
	vec3_t rotated_vector;
	rotated_vector.x = (this->x * cos(angle)) - (this->z * sin(angle));
	rotated_vector.y = this->y;
	rotated_vector.z = (this->x * sin(angle)) + (this->z * cos(angle));
	return rotated_vector;
}

vec3_t vec3_t::vec3_rotate_z( float angle) {
	vec3_t rotated_vector;
	rotated_vector.x = (this->x * cos(angle)) - (this->y * sin(angle));
	rotated_vector.y = (this->x * sin(angle)) + (this->y * cos(angle));
	rotated_vector.z = this->z;
	return rotated_vector;
}