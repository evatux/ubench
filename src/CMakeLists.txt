set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/*.[ch]pp)

# options
option(USE_RDPMC "allow using rdpmc" ON)

# 3rd party
include_directories(${PROJECT_SOURCE_DIR}/3rd_party/xbyak)

if(USE_RDPMC)
    add_definitions(-DUSE_RDPMC)
endif()

add_subdirectory(utils)

add_library(srcs OBJECT ${sources})
set_property(GLOBAL APPEND PROPERTY UBENCH_DEPS $<TARGET_OBJECTS:srcs>)
