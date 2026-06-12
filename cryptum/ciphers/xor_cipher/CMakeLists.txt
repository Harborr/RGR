add_library(xor_cipher SHARED xor_cipher.cpp)
target_include_directories(xor_cipher PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
set_target_properties(xor_cipher PROPERTIES PREFIX "lib" SUFFIX ".so")