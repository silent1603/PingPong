cmake_minimum_required(VERSION 3.22.1)

project(pingpong VERSION 0.1 LANGUAGES CXX)

set(PLATFORMS "default" CACHE STRING "target platforms")
message(STATUS "PLATFORMS: ${PLATFORMS}")
#check package
if(PLATFORMS STREQUAL "android")
find_package(game-activity REQUIRED CONFIG)
endif()

if(PLATFORMS STREQUAL "linux")
find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

endif()
#set(CMAKE_CXX_FLAGS "-Wall -pedantic -MMD")
#set(CMAKE_C_FLAGS "-Wall -pedantic -MMD")
if(PLATFORMS STREQUAL "android")
add_library(pingpong SHARED)
elseif(PLATFORMS STREQUAL "windows")
add_executable(pingpong WIN32)
else()
add_executable(pingpong)
endif()


if(PLATFORMS STREQUAL "windows")
find_package(OpenGL REQUIRED)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/lib )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/lib )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/bin )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
endif()



include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
  set_target_properties(pingpong PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

file(GLOB_RECURSE CPP_SOURCES ${CMAKE_SOURCE_DIR}/sources/*.cpp)
file(GLOB_RECURSE HEADER_SOURCES ${CMAKE_SOURCE_DIR}/sources/*.h)
source_group(TREE "${CMAKE_SOURCE_DIR}" PREFIX "" FILES ${CPP_SOURCES} ${HEADER_SOURCES})
message(STATUS ${CPP_SOURCES})
target_sources(pingpong PUBLIC ${CPP_SOURCES} ${HEADER_SOURCES})

target_include_directories(pingpong PUBLIC ${CMAKE_SOURCE_DIR}/sources)
target_compile_features(pingpong PUBLIC cxx_std_17)
set_target_properties(pingpong PROPERTIES POSITION_INDEPENDENT_CODE ON)



if(PLATFORMS STREQUAL "android")
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

if(PLATFORMS STREQUAL "linux")
target_link_libraries(pingpong PUBLIC OpenGL::OpenGL PUBLIC OpenGL::GLX PUBLIC X11::X11)
endif()

if(PLATFORMS STREQUAL "windows")
target_link_libraries(pingpong PUBLIC OpenGL::GL )
endif()

if(PLATFORMS STREQUAL "macos")
endif()