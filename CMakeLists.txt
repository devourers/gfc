cmake_minimum_required(VERSION 3.28)

project(gfc VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

set (gfc_SRC
    src/gfc.cpp
    src/gfc.h
    src/main.cpp
)

qt5_wrap_cpp(gfc_SRC_MOC
    src/gfc.h
)

qt5_wrap_ui(gfc_SRC_UI
    src/gfc.ui
)

add_executable(gfc
  ${gfc_SRC}
  ${gfc_SRC_MOC}
  ${gfc_SRC_UI}
  ${smart_batch_processor_SRC_RC}
)

target_link_libraries(gfc PRIVATE Qt5::Core Qt5::Widgets)

set_target_properties(gfc PROPERTIES
    WIN32_EXECUTABLE ON
)