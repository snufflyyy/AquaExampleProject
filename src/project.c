#include "project.h"
#include "gfx/material/material.h"
#include "gfx/mesh/mesh.h"
#include "gfx/renderer/renderer.h"
#include "gfx/renderer/resource-manager.h"
#include "gfx/shader/shader.h"

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

	AquaShaderHandle shader = aqua_renderer_resource_manager_create_shader(&aqua_context->renderer->resource_manager, "../aqua-engine/shaders/color/color.vert", "../aqua-engine/shaders/color/color.frag");
	project_data->material = aqua_material_create(aqua_context->renderer, shader, (AquaColor) { 0.0f, 0.5f, 1.0f });

	project_data->mesh = aqua_renderer_resource_manager_create_mesh(&aqua_context->renderer->resource_manager, QUAD_VERTICES, sizeof(QUAD_VERTICES) / sizeof(AquaVertex), QUAD_INDICES, sizeof(QUAD_INDICES) / sizeof(GLuint));
}

void event(AquaEngineContext* aqua_context, AquaEvent* event) {
	AquaExampleProject* project_data = (AquaExampleProject*) aqua_context->project.project_data_ptr;

}

void tick(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = (AquaExampleProject*) aqua_context->project.project_data_ptr;

}

void update(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = (AquaExampleProject*) aqua_context->project.project_data_ptr;

}

void render(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = (AquaExampleProject*) aqua_context->project.project_data_ptr;

	aqua_renderer_clear();

	aqua_renderer_draw_mesh(aqua_context->renderer, project_data->mesh, project_data->material);
}

void destroy(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = (AquaExampleProject*) aqua_context->project.project_data_ptr;

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
