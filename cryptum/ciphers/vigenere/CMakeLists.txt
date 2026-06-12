add_library(vigenere SHARED vigenere.cpp)
target_include_directories(vigenere PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

# Установка библиотек
install(TARGETS vigenere LIBRARY DESTINATION lib)
