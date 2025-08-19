cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(CustomObjectsAPI VERSION 1.0.0)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

# add lodepng library
CPMAddPackage(
  NAME lodepng
  GITHUB_REPOSITORY lvandeve/lodepng
  GIT_TAG 344b4b4
  DOWNLOAD_ONLY ON
)
add_library(lodepng STATIC ${lodepng_SOURCE_DIR}/lodepng.cpp)
target_include_directories(lodepng PUBLIC ${lodepng_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} lodepng)

# add rectpack2d library
CPMAddPackage(
  NAME rectpack2d
  GITHUB_REPOSITORY TeamHypersomnia/rectpack2D
  GIT_TAG 2f53fac
  DOWNLOAD_ONLY ON
)
target_include_directories(${PROJECT_NAME} PUBLIC ${rectpack2d_SOURCE_DIR}/src/rectpack2D)

setup_geode_mod(${PROJECT_NAME})