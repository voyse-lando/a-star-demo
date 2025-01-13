cmake_minimum_required(VERSION 3.15...3.30)

project(
	AStarDemo
	VERSION 0.1
	LANGUAGES CXX
)

add_subdirectory(lib/glad)
add_subdirectory(lib/glfw)

find_package(OpenGL REQUIRED)

add_executable(Demo
	src/Main.cpp
	src/Engine.cpp
	# src/Rectangle.cpp
	# src/Renderer.cpp
	# src/PathFind/PathFinder.cpp
	# src/PathFind/Map.cpp
	src/Object.cpp
	src/Shader.cpp
	src/Common.cpp
	src/Map/Map.cpp
	src/Map/Rectangle.cpp
	src/World/World.cpp
	src/Config/Config.cpp
)
set_property(TARGET Demo PROPERTY CXX_STANDARD 20)
configure_file(config.toml config.toml COPYONLY)

target_link_directories(Demo PRIVATE
	PRIVATE lib/glad/src
	PRIVATE lib/glfw/src
)

target_include_directories(Demo
	PUBLIC include
	PUBLIC lib/glad/include
	PUBLIC lib/glfw/include
)

if (UNIX)
	find_package(X11 REQUIRED)
	find_package(Threads REQUIRED)
endif ()

if (WIN32)
	target_link_libraries(Demo
		glfw GLAD OpenGL::GL shell32 user32 gdi32 atomic
	)
elseif (UNIX)
	target_link_libraries(Demo
		glfw GLAD OpenGL::GL X11::X11 X11::Xrandr X11::Xi atomic dl
	)
endif ()

