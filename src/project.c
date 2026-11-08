#include "project.h"
#include "cimgui.h"
#include "event/event.h"
#include "fly-camera/fly-camera.h"
#include "gfx/renderer/renderer.h"
#include "gfx/window/window.h"
#include "input/input.h"
#include "gfx/model/model.h"

#include <aqua.h>

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool initialize(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = (AquaExampleProject*) aqua_context->project->project_data_ptr;

	aqua_window_set_title(aqua_context->window, project_data->project.name);
	aqua_window_set_fullscreen(aqua_context->window, true);

	project_data->fly_camera = aqua_fly_camera_create((vec3) { 0.0f, 0.0f, 1.0f }, 70.0f, ((f32) aqua_context->window->properties.width / (f32) aqua_context->window->properties.height));

	project_data->model = aqua_model_create(aqua_context->renderer, "assets/sh1-school.glb");

	glm_vec3_zero(project_data->model_position);
	glm_vec3_zero(project_data->model_rotation);
	glm_vec3_one(project_data->model_scale);
	aqua_model_set_transform(&project_data->model, project_data->model_position, project_data->model_rotation, project_data->model_scale);

	return true;
}

void event(AquaEngineContext* aqua_context, AquaEvent* event) {
	AquaExampleProject* project_data = (AquaExampleProject*) aqua_context->project->project_data_ptr;

	aqua_fly_camera_event(&project_data->fly_camera, event);

	switch (event->type) {
		case AQUA_EVENT_TYPE_KEY_DOWN: {
			switch (event->key.scancode) {
				case AQUA_SCANCODE_ESCAPE: {
					aqua_input_manager_set_mouse_captured(&aqua_context->input_manager, !aqua_context->input_manager.properties.mouse_captured);
				} break;
				default: break;
			}
		} break;
		default: break;
	}
}

void tick(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = (AquaExampleProject*) aqua_context->project->project_data_ptr;

}

void update(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = (AquaExampleProject*) aqua_context->project->project_data_ptr;

	if (aqua_context->input_manager.properties.mouse_captured) {
        aqua_fly_camera_update(&project_data->fly_camera, (f32) aqua_context->renderer->delta_time);
	}

}

void render(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = (AquaExampleProject*) aqua_context->project->project_data_ptr;

	aqua_renderer_clear();
	aqua_renderer_bind_camera(aqua_context->renderer, &project_data->fly_camera.camera);

	aqua_renderer_draw_static_model(aqua_context->renderer, &project_data->model);
}

void imgui_properties_window(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = (AquaExampleProject*) aqua_context->project->project_data_ptr;

	igBegin("Aqua Example Project Properties", &project_data->project.show_properties_window, ImGuiWindowFlags_None);
	igSeparatorText("Model Transform");
	bool changed = false;
	changed |= igDragFloat3("Position", project_data->model_position, 0.05f, 0.0f, 0.0f, "%0.2f", ImGuiSliderFlags_None);
	changed |= igDragFloat3("Rotation", project_data->model_rotation, 0.5f, 0.0f, 0.0f, "%0.1f deg", ImGuiSliderFlags_None);
	changed |= igDragFloat3("Scale", project_data->model_scale, 0.01f, 0.0f, 0.0f, "%0.2f", ImGuiSliderFlags_None);
	if (igButton("Reset", (ImVec2) { 0.0f, 0.0f })) {
		glm_vec3_zero(project_data->model_position);
		glm_vec3_zero(project_data->model_rotation);
		glm_vec3_one(project_data->model_scale);
		changed = true;
	}
	if (changed) {
		aqua_model_set_transform(&project_data->model, project_data->model_position, project_data->model_rotation, project_data->model_scale);
	}
	igEnd();
}

void imgui_update(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = (AquaExampleProject*) aqua_context->project->project_data_ptr;

	if (project_data->project.show_properties_window) {
		imgui_properties_window(aqua_context);
	}
}

void destroy(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = (AquaExampleProject*) aqua_context->project->project_data_ptr;

	aqua_model_destroy(aqua_context->renderer, &project_data->model);
	aqua_fly_camera_destroy(&project_data->fly_camera);
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

		.show_properties_window = false,

		.project_data_ptr = (void*) project_data,

		.initialize = initialize,
		.event = event,
		.tick = tick,
		.update = update,
		.render = render,
		.imgui_update = imgui_update,
		.destroy = destroy,
	};

	return project_data;
}

void aqua_example_project_destroy(AquaExampleProject* example_project) {
	assert(example_project != NULL);
	free(example_project);
}
