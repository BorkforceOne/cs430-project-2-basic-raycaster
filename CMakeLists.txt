cmake_minimum_required(VERSION 3.6)
project(cs430_project_2_basic_raycaster)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES source/main.c source/imaging/ppm.c source/constants.h source/imaging/ppm.h source/imaging/imaging.h source/json/json.c source/json/json_parsers.c source/json/json_parsers.h source/json/json_helpers.c source/json/json_helpers.h source/helpers.h source/helpers.c source/imaging/ppm_helpers.h source/imaging/ppm_helpers.c source/json/json.h source/raycaster/raycaster.h source/raycaster/raycaster.c source/raycaster/3dmath.h source/raycaster/raycaster_helpers.c source/raycaster/raycaster_helpers.h)
add_executable(cs430_project_2_basic_raycaster ${SOURCE_FILES})