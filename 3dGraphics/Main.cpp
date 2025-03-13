#include <SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <vector>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include <iostream>



std::vector<triangle_t> triangles_to_render;


vec3_t camera_position = { 0, 0, 0 };

float fov_factor = 640;

bool is_running = false;

int previous_frame_time = 0;

enum cull_method cull_method = CULL_NONE;
enum render_method render_method = RENDER_WIRE;
void setup(void) {

	// Initialize render mode and triangle culling method
	render_method = RENDER_WIRE;
	cull_method = CULL_BACKFACE;


	// Allocate the required memory in bytes to hold the color buffer
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

	// Creating a SDL texture that is used to display the color buffer
	color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width, window_height);

	//load_cube_mesh_data();
	load_obj_file("D:\\Codes\\Dev\\C_CPP\\3dGraphics\\3dGraphics\\cube.obj");

}

void process_input(void) {
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		is_running = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE)
			is_running = false;
		if (event.key.keysym.sym == SDLK_1)
			render_method = RENDER_WIRE_VERTEX;
		if (event.key.keysym.sym == SDLK_2)
			render_method = RENDER_WIRE;
		if (event.key.keysym.sym == SDLK_3)
			render_method = RENDER_FILL_TRIANGLE;
		if (event.key.keysym.sym == SDLK_4)
			render_method = RENDER_FILL_TRIANGLE_WIRE;
		if (event.key.keysym.sym == SDLK_c)
			cull_method = CULL_BACKFACE;
		if (event.key.keysym.sym == SDLK_d)
			cull_method = CULL_NONE;
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Function that receives a 3D vector and returns a projected 2D point
////////////////////////////////////////////////////////////////////////////////
vec2_t project(vec3_t point) {
	vec2_t projected_point = { (fov_factor * point.x) / point.z,
							  (fov_factor * point.y) / point.z };
	return projected_point;
}

void update(void) {
	// Wait some time to have a constant frame rate
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
		SDL_Delay(time_to_wait);
	}
	previous_frame_time = SDL_GetTicks();


	// Initialize the triangles to render



	mesh.rotation.y += 0.01;
	mesh.rotation.z += 0.01;
	mesh.rotation.x += 0.01;


	int num_faces = mesh.faces.size();
	for (int i = 0; i < num_faces; i++) {
		face_t mesh_face = mesh.faces[i];

		vec3_t face_vertices[3] = {
			mesh.vertices[mesh_face.a - 1],
			mesh.vertices[mesh_face.b - 1],
			mesh.vertices[mesh_face.c - 1]
		};

		vec3_t transformed_vertexs[3];

		// Loop all three vertices of this current face and apply transformations
		for (int j = 0; j < 3; j++) {
			vec3_t tranformed_vertex = face_vertices[j];
			tranformed_vertex = tranformed_vertex.vec3_rotate_x(mesh.rotation.x);
			tranformed_vertex = tranformed_vertex.vec3_rotate_y(mesh.rotation.y);
			tranformed_vertex = tranformed_vertex.vec3_rotate_z(mesh.rotation.z);

			// Translate the vertex away from the camera
			tranformed_vertex.z += 5;
			// Save the transformed vertex in the array of transformed vertexs
			transformed_vertexs[j] = tranformed_vertex;
		}
		if (cull_method == CULL_BACKFACE) {
			vec3_t vector_a = transformed_vertexs[0]; /*   A   */
			vec3_t vector_b = transformed_vertexs[1]; /*  / \  */
			vec3_t vector_c = transformed_vertexs[2]; /* B---C */

			vec3_t vector_ab = vector_b.sub(vector_a);
			vec3_t vector_ac = vector_c.sub(vector_a);
			vector_ab.normalize();
			vector_ac.normalize();

			// Compute the normal of the face
			vec3_t normal = vector_ab.cross(vector_ac);
			normal.normalize();
			// Find the vector between a point in the triangle and the camera
			vec3_t camera_ray = camera_position.sub(vector_a);

			// Calculate how aligned the camera ray is with the face normal 
			float dot_normal_camera = normal.dot(camera_ray);

			if (dot_normal_camera < 0) {
				continue;
			}
		}
		
	
		triangle_t projected_triangle;


		for (int j = 0; j < 3; j++) {
			// Project the current vertex
			vec2_t projected_point = project(transformed_vertexs[j]);

			// Scale the translate the projected points to the middle of the screen
			projected_point.x += window_width / 2;
			projected_point.y += window_height / 2;

			projected_triangle.points[j] = projected_point;
			
		}

		// Save the projected triangle in the array of triangles to render
		triangles_to_render.push_back(projected_triangle);

	}
}

void free_resources(void) {
	////free(color_buffer);
	//std::cout << "free(color_buffer);" << std::endl;

	mesh.faces.clear();
#ifdef DEBUG
	std::cout << "mesh.faces.clear();" << std::endl;
#endif // DEBUG


	mesh.vertices.clear();
#ifdef DEBUG
	std::cout << "mesh.vertices.clear();" << std::endl;
#endif // DEBUG
}

void render(void) {
	draw_grid();
	// Loop all projected triangles and render them
	for (int i = 0; i < triangles_to_render.size(); i++) {
		triangle_t triangle = triangles_to_render[i];
		if (render_method == RENDER_FILL_TRIANGLE || render_method == RENDER_FILL_TRIANGLE_WIRE) {
			triangle.draw_filled_triangle(0xFF555555);
		}

		// Draw triangle wireframe
		if (render_method == RENDER_WIRE || render_method == RENDER_WIRE_VERTEX || render_method == RENDER_FILL_TRIANGLE_WIRE) {
			triangle.draw_triangle(0xFFFFFFFF);
		}

		// Draw triangle vertex points
		if (render_method == RENDER_WIRE_VERTEX) {
			draw_rect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6, 0xFFFF0000); // vertex A
			draw_rect(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6, 0xFFFF0000); // vertex B
			draw_rect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6, 0xFFFF0000); // vertex C
		}
	}

	

	//draw_triangle(300, 100, 50, 400, 500, 700, 0xFF00FF00);

	// Clear the array of triangles to render
	triangles_to_render.clear();
	
	render_color_buffer();

	clear_color_buffer(0xFF000000);

	SDL_RenderPresent(renderer);
}

int main(int args, char* argr[]) {
	is_running = initialize_window();

	setup();

	while (is_running) {
		process_input();
		update();
		render();
	}

	destroy_window();
#ifdef DEBUG
	std::cout << "destroy_window" << std::endl;
#endif // DEBUG


	free_resources();
#ifdef DEBUG
	std::cout << "free_resources" << std::endl;
#endif // DEBUG



	return 0;
}
