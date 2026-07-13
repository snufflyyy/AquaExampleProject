#include "project.h"
#include "event/event.h"
#include "gfx/camera/camera.h"
#include "gfx/material/material.h"
#include "gfx/renderer/renderer.h"
#include "gfx/shader/shader.h"
#include "input/input.h"

#include <aqua.h>

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static AquaVertex QUAD_VERTICES[4] = {
    { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } }, // bottom-left
    { {  0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } }, // bottom-right
    { {  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } }, // top-right
    { { -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } }  // top-left
};

static GLuint QUAD_INDICES[6] = {
    0, 1, 2,
    2, 3, 0
};

void initialize(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = (AquaExampleProject*) aqua_context->project.project_data_ptr;

	aqua_window_set_title(aqua_context->window, project_data->project.name);

	AquaShaderHandle shader = aqua_renderer_create_shader(aqua_context->renderer, "../AquaEngine/shaders/color/color.vert", "../AquaEngine/shaders/color/color.frag");
	project_data->material = aqua_material_create(aqua_context->renderer, shader, (AquaColor) { 0.0f, 0.5f, 1.0f });

	project_data->mesh = aqua_renderer_create_mesh(aqua_context->renderer, QUAD_VERTICES, sizeof(QUAD_VERTICES) / sizeof(AquaVertex), QUAD_INDICES, sizeof(QUAD_INDICES) / sizeof(GLuint));

	project_data->camera = aqua_camera_create((vec3) { 0.0f, 0.0f, 0.0f }, glm_rad(70.0f), ((f32) aqua_context->window->properties.width / aqua_context->window->properties.height));

	project_data->camera_pitch = 0.0f;
	project_data->camera_yaw = 0.0f;

	project_data->mouse_sensitivity = 0.2f;
}

void event(AquaEngineContext* aqua_context, AquaEvent* event) {
	AquaExampleProject* project_data = (AquaExampleProject*) aqua_context->project.project_data_ptr;

	switch (event->type) {
		case AQUA_EVENT_TYPE_KEY_DOWN: {
			switch (event->key.scancode) {
				case AQUA_SCANCODE_ESCAPE: {
					aqua_input_manager_set_mouse_captured(&aqua_context->input_manager, !aqua_context->input_manager.properties.mouse_captured);
				} break;
				default: break;
			}
		} break;
	    case AQUA_EVENT_TYPE_MOUSE_MOTION: {
			if (aqua_context->input_manager.properties.mouse_captured) {
				project_data->camera_yaw += (event->mouse_motion.x_relative * project_data->mouse_sensitivity);
				project_data->camera_pitch -= (event->mouse_motion.y_relative * project_data->mouse_sensitivity);

				if (project_data->camera_pitch > 89.0f) { project_data->camera_pitch = 89.0f; }
				if (project_data->camera_pitch < -89.0f) { project_data->camera_pitch = -89.0f; }

				vec3 direction = {
	                cosf(glm_rad(project_data->camera_yaw)) * cosf(glm_rad(project_data->camera_pitch)),
	                sinf(glm_rad(project_data->camera_pitch)),
	                sinf(glm_rad(project_data->camera_yaw)) * cosf(glm_rad(project_data->camera_pitch)),
	            };
	            glm_normalize(direction);
	            glm_vec3_copy(direction, project_data->camera.front);
			}
		} break;
		default: break;
	}
}

void tick(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = (AquaExampleProject*) aqua_context->project.project_data_ptr;

}

void update(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = (AquaExampleProject*) aqua_context->project.project_data_ptr;

	aqua_camera_update(&project_data->camera);
}

void render(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = (AquaExampleProject*) aqua_context->project.project_data_ptr;

	aqua_renderer_clear();

	aqua_renderer_draw_mesh(aqua_context->renderer, &project_data->camera, project_data->mesh, project_data->material);
}

void destroy(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = (AquaExampleProject*) aqua_context->project.project_data_ptr;

	aqua_material_destroy(project_data->material);
}

AquaExampleProject* aqua_example_project_create(void) {
	AquaExampleProject* project_data = (AquaExampleProject*) malloc(sizeof(AquaExampleProject));
	if (!project_data) {
		fprintf(stderr, "[ERROR] [Aqua] [Project] Failed to allocate memory for project data!\n");
		return NULL;
	}

	project_data->project = (AquaProject) {
		.name = "Aqua Example Project",
		.creator = "Braden",

		.project_data_ptr = (void*) project_data,

		.initialize = initialize,
		.event = event,
		.tick = tick,
		.update = update,
		.render = render,
		.destroy = destroy,
	};

	return project_data;
}

void aqua_example_project_destroy(AquaExampleProject* example_project) {
	assert(example_project != NULL);
	free(example_project);
}
