#include "project.h"
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <aqua/debug.h>

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static bool initialize(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = aqua_engine_context_get_project_data(aqua_context);

	AquaWindow* window = aqua_engine_context_get_window(aqua_context);
	AquaWindowProperties window_properties = aqua_window_get_properties(window);
	window_properties.title = project_data->project.name;
	window_properties.fullscreen = false;
	if (!aqua_window_set_properties(window, window_properties)) { return false; }

	u32 width = 0, height = 0;
	aqua_window_get_pixel_size(window, &width, &height);
	AquaCameraProperties camera_properties = AQUA_CAMERA_DEFAULT_PROPERTIES;
	if (width > 0 && height > 0) { camera_properties.aspect = (f32) width / (f32) height; }
	project_data->fly_camera = aqua_fly_camera_create(camera_properties, AQUA_FLY_CAMERA_DEFAULT_PROPERTIES);
	if (!project_data->fly_camera) { return false; }

	project_data->model = aqua_renderer_create_model(aqua_engine_context_get_renderer(aqua_context), "assets/sh1-school.glb");

	for (u32 i = 0; i < AQUA_EXAMPLE_PROJECT_MODEL_INSTANCE_COUNT; i++) {
		AquaExampleModelInstance* model_instance = &project_data->model_instances[i];
		model_instance->instance = aqua_model_instance_create(project_data->model);
		glm_vec3_zero(model_instance->position);
		model_instance->position[0] = (f32) i * 50.0f;
		glm_vec3_zero(model_instance->rotation);
		glm_vec3_one(model_instance->scale);
		aqua_model_instance_set_transform(&model_instance->instance, model_instance->position, model_instance->rotation, model_instance->scale);
	}

	return true;
}

static void event(AquaEngineContext* aqua_context, const AquaEvent* event) {
	AquaExampleProject* project_data = aqua_engine_context_get_project_data(aqua_context);

	if (event->type == AQUA_EVENT_TYPE_WINDOW_PIXEL_SIZE_CHANGED && event->window.new_width > 0 && event->window.new_height > 0) {
		AquaCamera* camera = aqua_fly_camera_get_camera(project_data->fly_camera);
		(void) aqua_camera_update_aspect(camera, (f32) event->window.new_width / (f32) event->window.new_height);
	}
	if (event->type == AQUA_EVENT_TYPE_KEY_DOWN && event->key.scancode == AQUA_SCANCODE_ESCAPE) {
		AquaInputManagerProperties properties = aqua_input_get_properties(aqua_context);
		properties.mouse_captured = !properties.mouse_captured;
		(void) aqua_input_set_properties(aqua_context, properties);
	}
	if (aqua_input_is_mouse_captured(aqua_context)) {
		aqua_fly_camera_event(project_data->fly_camera, event);
	} else {
		aqua_fly_camera_reset_input(project_data->fly_camera);
	}
}

static void tick(AquaEngineContext* aqua_context) {
	(void) aqua_context;
}

static void update(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = aqua_engine_context_get_project_data(aqua_context);
	if (aqua_input_is_mouse_captured(aqua_context)) {
		aqua_fly_camera_update(project_data->fly_camera, (f32) aqua_engine_context_get_delta_time(aqua_context));
	} else {
		aqua_fly_camera_reset_input(project_data->fly_camera);
	}
}

static void render(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = aqua_engine_context_get_project_data(aqua_context);
	AquaRenderer* renderer = aqua_engine_context_get_renderer(aqua_context);
	aqua_renderer_clear();
	aqua_renderer_bind_camera(renderer, aqua_fly_camera_get_camera(project_data->fly_camera));
	for (u32 i = 0; i < AQUA_EXAMPLE_PROJECT_MODEL_INSTANCE_COUNT; i++) {
		aqua_renderer_draw_model_instance(renderer, &project_data->model_instances[i].instance);
	}
}

static void imgui_properties_window(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = aqua_engine_context_get_project_data(aqua_context);

	igBegin("Aqua Example Project Properties", &project_data->project.show_properties_window, ImGuiWindowFlags_None);
	igSeparatorText("Model Instance Transform");
	const char* instance_names[AQUA_EXAMPLE_PROJECT_MODEL_INSTANCE_COUNT] = { "Instance 1", "Instance 2" };
	igCombo_Str_arr("Instance", &project_data->selected_model_instance, instance_names, AQUA_EXAMPLE_PROJECT_MODEL_INSTANCE_COUNT, -1);
	AquaExampleModelInstance* model_instance = &project_data->model_instances[project_data->selected_model_instance];
	bool changed = false;
	changed |= igDragFloat3("Position", model_instance->position, 0.05f, 0.0f, 0.0f, "%0.2f", ImGuiSliderFlags_None);
	changed |= igDragFloat3("Rotation", model_instance->rotation, 0.5f, 0.0f, 0.0f, "%0.1f deg", ImGuiSliderFlags_None);
	changed |= igDragFloat3("Scale", model_instance->scale, 0.01f, 0.0f, 0.0f, "%0.2f", ImGuiSliderFlags_None);
	if (igButton("Reset", (ImVec2) { 0.0f, 0.0f })) {
		glm_vec3_zero(model_instance->position);
		glm_vec3_zero(model_instance->rotation);
		glm_vec3_one(model_instance->scale);
		changed = true;
	}
	if (changed) {
		aqua_model_instance_set_transform(&model_instance->instance, model_instance->position, model_instance->rotation, model_instance->scale);
	}
	igSeparatorText("Camera");
	AquaCamera* camera = aqua_fly_camera_get_camera(project_data->fly_camera);
	AquaCameraProperties properties = aqua_camera_get_properties(camera);
	if (igCheckbox("Show Camera Properties", &properties.show_properties_window)) {
		(void) aqua_camera_set_properties(camera, properties);
	}
	igEnd();
}

static void imgui_update(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = aqua_engine_context_get_project_data(aqua_context);

	if (project_data->project.show_properties_window) {
		imgui_properties_window(aqua_context);
	}
	aqua_camera_imgui_update(aqua_fly_camera_get_camera(project_data->fly_camera));
}

static void destroy(AquaEngineContext* aqua_context) {
	AquaExampleProject* project_data = aqua_engine_context_get_project_data(aqua_context);
	AquaRenderer* renderer = aqua_engine_context_get_renderer(aqua_context);

	for (u32 i = 0; i < AQUA_EXAMPLE_PROJECT_MODEL_INSTANCE_COUNT; i++) {
		project_data->model_instances[i].instance.model = AQUA_MODEL_HANDLE_INVALID;
	}
	aqua_renderer_destroy_model(renderer, project_data->model);
	project_data->model = AQUA_MODEL_HANDLE_INVALID;
	aqua_renderer_bind_camera(renderer, NULL);
	aqua_fly_camera_destroy(project_data->fly_camera);
	project_data->fly_camera = NULL;
}

AquaExampleProject* aqua_example_project_create(void) {
	AquaExampleProject* project_data = (AquaExampleProject*) calloc(1, sizeof(AquaExampleProject));
	if (!project_data) {
		fprintf(stderr, "[ERROR] [Aqua] [Project] Failed to allocate memory for project data!\n");
		return NULL;
	}

	project_data->model = AQUA_MODEL_HANDLE_INVALID;
	for (u32 i = 0; i < AQUA_EXAMPLE_PROJECT_MODEL_INSTANCE_COUNT; i++) {
		project_data->model_instances[i].instance = aqua_model_instance_create(AQUA_MODEL_HANDLE_INVALID);
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
