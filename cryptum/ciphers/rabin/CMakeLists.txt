add_library(rabin SHARED rabin.cpp)
target_include_directories(rabin PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
set_target_properties(rabin PROPERTIES PREFIX "lib" SUFFIX ".so")