add_library(hill SHARED hill.cpp)
target_include_directories(hill PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
set_target_properties(hill PROPERTIES PREFIX "lib" SUFFIX ".so")