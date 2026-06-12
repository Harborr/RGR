add_executable(cryptum_test test_main.cpp)
target_include_directories(cryptum_test PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(cryptum_test PRIVATE ${CMAKE_SOURCE_DIR}/ciphers)

if(UNIX)
    target_link_libraries(cryptum_test PRIVATE dl)
endif()
