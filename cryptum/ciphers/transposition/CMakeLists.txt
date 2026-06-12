add_library(transposition SHARED transposition.cpp)
target_include_directories(transposition PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
set_target_properties(transposition PROPERTIES PREFIX "lib" SUFFIX ".so")