add_library(Data STATIC
    parser.cpp
    tileset.cpp
)

target_include_directories(Data PUBLIC "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(Data PUBLIC
    spdlog::spdlog
    Stb
)
