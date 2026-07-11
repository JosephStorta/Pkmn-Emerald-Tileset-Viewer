add_library(Gui STATIC
    app.cpp
)

target_include_directories(Gui PUBLIC "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(Gui PUBLIC
    Data

    Logger
    ${wxWidgets_LIBRARIES}
)
