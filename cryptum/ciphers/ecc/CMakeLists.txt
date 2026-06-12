add_library(ecc SHARED ecc.cpp)
target_include_directories(ecc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
set_target_properties(ecc PROPERTIES PREFIX "lib" SUFFIX ".so")