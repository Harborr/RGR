# Главный исполняемый файл
add_executable(cryptum
    main.cpp
    cli/args_parser.cpp
    core/library_loader.cpp
    core/key_generator.cpp
    io/file_io.cpp
    io/secure_memory.cpp
)

target_include_directories(cryptum PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Линковка с dl (для Linux)
if(UNIX)
    target_link_libraries(cryptum PRIVATE dl)
endif()

# Установка
install(TARGETS cryptum RUNTIME DESTINATION bin)
