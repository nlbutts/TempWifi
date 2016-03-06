cmake_minimum_required (VERSION 3.4)
project (TempWiFi)
include(ExternalProject)

option(BUILD_UT "Build the unit tests" ON)
file(GLOB_RECURSE CPPSRC SendEmail/JSONMessage.cpp)
file(GLOB_RECURSE UTSRC ut/*.cpp)

include_directories(include)
add_library(${CMAKE_PROJECT_NAME} ${CPPSRC})

if (BUILD_UT)
	include(CMakeLists_external.txt)

	add_executable(${CMAKE_PROJECT_NAME}_ut ${UTSRC})
	find_library(googletest ../googltest)
	target_link_libraries(${CMAKE_PROJECT_NAME}_ut TempWiFi pthread)
    target_include_directories(${CMAKE_PROJECT_NAME}_ut PUBLIC SendEmail)
endif ()
