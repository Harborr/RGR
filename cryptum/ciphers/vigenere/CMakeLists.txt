add_library(vigenere SHARED vigenere.cpp)
target_include_directories(vigenere PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
set_target_properties(vigenere PROPERTIES PREFIX "lib" SUFFIX ".so")