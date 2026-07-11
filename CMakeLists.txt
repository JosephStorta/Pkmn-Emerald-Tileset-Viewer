# Minimum CMake version for build process
cmake_minimum_required(VERSION 4.2)

# Define the project and tell CMake it is a C/C++ project
project(
    Pkmn_Emerald_Tileset_Viewer
    VERSION 1.0
    DESCRIPTION ""
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory for the executable
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin")

# --- Load third-party libraries --- #

# Logger
add_library(Logger STATIC
    ${CMAKE_SOURCE_DIR}/vendor/logger/logger.cpp
)

target_include_directories(Logger PUBLIC ${CMAKE_SOURCE_DIR}/vendor)

# STB
add_library(Stb STATIC
    ${CMAKE_SOURCE_DIR}/vendor/stb/stb_image.cpp
)

target_include_directories(Stb PUBLIC ${CMAKE_SOURCE_DIR}/vendor)

# WxWidgets
find_package(wxWidgets REQUIRED CONFIG COMPONENTS net core base)

# Load subdirectories
add_subdirectory(src/data)
add_subdirectory(src/gui)

# Add the executable
add_executable(Main
    src/main.cpp
)

# Add include directories for the main executable
target_include_directories(Main PUBLIC "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(Main PRIVATE
    Gui

    # Vendor libraries
    ${wxWidgets_LIBRARIES}
)
