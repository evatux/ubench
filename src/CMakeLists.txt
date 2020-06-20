file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/*.[ch]pp)

# 3rd party
include_directories(${PROJECT_SOURCE_DIR}/3rd_party/xbyak)

add_library(srcs OBJECT ${sources})
set_property(GLOBAL APPEND PROPERTY UBENCH_DEPS $<TARGET_OBJECTS:srcs>)
