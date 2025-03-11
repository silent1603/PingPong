cmake_minimum_required(VERSION 3.22.1)

project(pingpong VERSION 0.1 LANGUAGES CXX)

set(PLATFORMS "default" CACHE STRING "target platforms")
message(STATUS "VARIABLE_NAME: ${VARIABLE_NAME}")

if(PLATFORMS STREQUAL "android")
add_library(pingpong SHARED)
else()
add_executable(pingpong)
endif()

#check package
if(PLATFORMS STREQUAL "android")
find_package(game-activity REQUIRED CONFIG)
endif()

include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
  set_target_properties(pingpong PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

file(GLOB_RECURSE CPP_SOURCES ${CMAKE_SOURCE_DIR}/sources/*.cpp)
message(STATUS ${CPP_SOURCES})
target_sources(pingpong PUBLIC ${CPP_SOURCES})

target_include_directories(pingpong PUBLIC ${CMAKE_SOURCE_DIR}/sources)
target_compile_features(pingpong PUBLIC cxx_std_17)
set_target_properties(pingpong PROPERTIES POSITION_INDEPENDENT_CODE ON)

#target_link_libraries(pingpong)

if(PLATFORMS STREQUAL "android")
# Configure libraries CMake uses to link your target library.
target_link_libraries(pingpong
        # The game activity
        game-activity::game-activity

        # EGL and other dependent libraries required for drawing
        # and interacting with Android system
        EGL
        GLESv3
        jnigraphics
        android
        log)

endif()