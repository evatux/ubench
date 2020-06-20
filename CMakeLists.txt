cmake_minimum_required (VERSION 2.8.11)

project(ubench CXX)

# sources
add_subdirectory(src)

# main executable
get_property(ubench_deps GLOBAL PROPERTY UBENCH_DEPS)
add_executable (ubench ${ubench_deps})
