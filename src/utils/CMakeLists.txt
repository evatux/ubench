file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/*.[ch]pp)

add_library(utils OBJECT ${sources})
set_property(GLOBAL APPEND PROPERTY UBENCH_DEPS $<TARGET_OBJECTS:utils>)
