cmake_minimum_required(VERSION 3.22.1)

project(pingpong VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Available build types" FORCE)

# Detect platform and include corresponding CMake file
if(PLATFORMS STREQUAL "android")
    include(${CMAKE_SOURCE_DIR}/cmake/platforms/android.cmake)
elseif(PLATFORMS STREQUAL "linux")
    include(${CMAKE_SOURCE_DIR}/cmake/platforms/linux.cmake)
elseif(PLATFORMS STREQUAL "windows")
    include(${CMAKE_SOURCE_DIR}/cmake/platforms/windows.cmake)
elseif(PLATFORMS STREQUAL "macos")
    include(${CMAKE_SOURCE_DIR}/cmake/platforms/macos.cmake)
else()
    message(FATAL_ERROR "Unsupported platform: ${PLATFORMS}")
endif()

# Detect compiler and include the corresponding CMake file
if(MSVC)
    include(${CMAKE_SOURCE_DIR}/cmake/compilers/msvc.cmake)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    include(${CMAKE_SOURCE_DIR}/cmake/compilers/clang.cmake)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    include(${CMAKE_SOURCE_DIR}/cmake/compilers/gcc.cmake)
else()
    message(FATAL_ERROR "Unsupported compiler: ${CMAKE_CXX_COMPILER_ID}")
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

target_include_directories(pingpong PUBLIC ${CMAKE_SOURCE_DIR}/sources/core)
target_include_directories(pingpong PUBLIC ${CMAKE_SOURCE_DIR}/sources/externals)
target_compile_features(pingpong PUBLIC cxx_std_17)
set_target_properties(pingpong PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT pingpong)
set_target_properties(pingpong PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)
