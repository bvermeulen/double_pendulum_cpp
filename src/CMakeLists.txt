cmake_minimum_required(VERSION 3.12)

project(dp_app_cmake)
enable_language(C CXX)

set(sources
    doublependulum.cpp
    dp_app.cpp
    dp_frame.cpp
    drawing_panel.cpp
    gonio_funcs.cpp
    objects.cpp
)

set(wxWidgets_ROOT_DIR "d:/cpp/wxWidgets")
set(wxWidgets_LIB_DIR "d:/cpp/wxWidgets/lib/gcc_x64_dll")
set(wxWidgets_CONFIGURATION mswud)
find_package(wxWidgets REQUIRED COMPONENTS gl core base OPTIONAL_COMPONENTS net)

if (wxWidgets_FOUND)
    message("==> wxWidgets is found with configuration: ${wxWidgets_CONFIGURATION}")
endif()

include(UsewxWidgets)
include_directories(includes
    ../include
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/debug)
add_executable(dp_app_cmake ${sources} ${includes})

target_link_libraries(dp_app_cmake wxWidgets::wxWidgets)

set_target_properties(dp_app_cmake PROPERTIES LDFLAGS "-Wall")
set_target_properties(dp_app_cmake PROPERTIES COMPILE_FLAGS "-Wall -g -fvisibility=hidden")
