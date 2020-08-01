cmake_minimum_required(VERSION 3.10)
project(V3D VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)
find_package(VTK REQUIRED)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

if(VTK_FOUND)
	message(STATUS "vtk found")
endif()

find_package(Qt5 COMPONENTS Core Gui Widgets OpenGL PrintSupport REQUIRED)
if(Qt5_FOUND)
	message(STATUS "qt5 found")
endif()
include_directories(include)
add_subdirectory(V2D)
add_subdirectory(V3D)

