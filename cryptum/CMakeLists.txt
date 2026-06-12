cmake_minimum_required(VERSION 3.10)
project(Cryptum CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

add_subdirectory(src)
add_subdirectory(ciphers)
add_subdirectory(tests)

# Установка (опционально)
install(TARGETS cryptum DESTINATION bin)