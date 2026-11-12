cmake_minimum_required(VERSION 3.16)
project(AquaExampleProject)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(AquaEngine)

include_directories(src)

add_executable(
	${PROJECT_NAME}
	src/main.c

	src/project.c
)

# This example opts into custom cimgui UI; ordinary projects only need AquaEngine.
target_link_libraries(${PROJECT_NAME} PRIVATE AquaEngine AquaEngineDebug)

target_compile_options(${PROJECT_NAME} PRIVATE
	-Wall -Wextra -Wpedantic
	-Wconversion -Wshadow -Wstrict-prototypes -Wformat=2 -Wnull-dereference
	-Wmissing-prototypes
	-Werror
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
	$<$<NOT:$<CONFIG:Debug>>:NDEBUG>
)
