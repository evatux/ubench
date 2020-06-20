cmake_minimum_required (VERSION 2.8.11)

project(ubench CXX)

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    message(STATUS "CMAKE_BUILD_TYPE is unset, defaulting to Release")
    set(CMAKE_BUILD_TYPE "RelWithAssert" CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel RelWithAssert ...")
endif()


# sources
add_subdirectory(src)

# main executable
get_property(ubench_deps GLOBAL PROPERTY UBENCH_DEPS)
add_executable (ubench ${ubench_deps})
